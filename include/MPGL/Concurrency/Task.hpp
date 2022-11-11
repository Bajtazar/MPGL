/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include <MPGL/Traits/Concepts.hpp>

#include <functional>
#include <algorithm>
#include <coroutine>
#include <memory>
#include <future>
#include <vector>
#include <thread>
#include <atomic>

namespace mpgl::async {

    /**
     * Manages a threadpool. Automaticly distributes tasks between
     * working threads and allows to get task future
     */
    class Threadpool;

    namespace details {

        /**
         * Interface for promise types used by task class.
         * Stores generalized informations about task and allows
         * to awake parent with different template specializations
         */
        class PromiseTypeInterface {
        protected:
            /**
             * optimization of virtual void awake(void) const = 0
             * [the only virtual function so reduces number of
             * function pointer calls from 2 to 1 without
             * memory size overhead]. Awakes this coroutine
             */
            typedef void(*Awaker)(PromiseTypeInterface const&);

            /**
             * Signalizes that a coroutine has ended its work.
             * Inform parent coroutine [if that coroutine exists]
             * that its child has ended work. If all of parent's
             * children has ended its work then awakes the parent
             * coroutine. If parent does not exist then does
             * nothing
             */
            void finish(void);

            std::atomic<size_t>         childrenCounter = {};
            Threadpool*                 threadpool = nullptr;
            PromiseTypeInterface*       parent = nullptr;
            Awaker                      awake;
        };

    }

    /**
     * Tag used in co_await expression to preempt coroutine
     * and wait for all child coroutines to execute
     */
    struct synchronize_t{};

    inline constexpr synchronize_t      synchronize;

    /**
     * Asynchronic task wrapper used to put coroutine inside a
     * threadpool. Allows to implement very versatile asynchronious
     * code using modified co_yield calls. Using co_yield expression
     * on the other mpgl::async::task coroutine wont suspend the
     * execution of the current coroutine and adds it to the
     * threadpool currently used by the parent coroutine. That
     * allows to schedudle multiple coroutines before preemting
     * current coroutine. Moreover, co_yield expression returns
     * std::future objects, that can be used to acquire result
     * of the children coroutines excecution. To maximize performance
     * potential the co_await mpgl::async::synchronize call can be
     * used to wait for the children coroutines to end. Using
     * this construction guarantee that all of the std::futures comming
     * from previous co_yield expressions will be ready. This call
     * is also faster that manually synchronizing std::futures
     * because it asleeps and preemts current coroutine and
     * wait for execution of the latest children to be awaken.
     * Coroutine is being destroyed only after complete execution
     *
     * @tparam ReturnTp a type returned by the coroutine
     * @tparam Alloc a type of the stateless allocator that allocs
     * the coroutine on the heap
     *
     * @warning the allocator type must be stateless, otherwise
     * using task class will result in undefined behaviour
     * @warning the coroutine returning non-void type must return
     * before the end of the coroutine, otherwise falling through
     * coroutine will result in undefined behaviour
     * @warning yielding children coroutines and not synchronizing
     * them before the coroutine returns may result in undefined
     * behaviour if children coroutines finishes after the parent.
     * (Manual synchronization using for example futures returned
     * by co_yield rather than calling co_await mpgl::async::synchronize
     * is OK if all children coroutines has ended its job before
     * the end of the parent)
     * @warning abandoning coroutine in the middle of its execution
     * requires explicit destruction. Coroutine is destroyed automatically
     * only after full execution
     *
     * @note example valid usage of task
     * @code{.cpp}
     *  template <
     *      std::random_access_iterator Iter,
     *      std::sentinel_for<Iter> Sent>
     *  mpgl::async::task<void> merge_sort(
     *      Iter iter,
     *      Sent const& sent)
     *  {
     *      if (std::next(iter) == sent)
     *          co_return;
     *
     *      auto const middle = iter + std::distance(iter, sent) / 2;
     *      co_yield merge_sort(iter, middle);
     *      co_yield merge_sort(middle);
     *      co_await mpgl::async::synchronize;
     *
     *      std::vector<std::iter_value_t<Iter>> buffer(std::distance(iter, sent));
     *      std::merge(iter, middle, middle, sent, buffer.begin());
     *      std::ranges::copy(buffer, iter);
     *      // fallthrough is OK because its returning void
     *  }
     * @endcode
     */
    template <
        PureType ReturnTp,
        Allocator<std::byte> Alloc = std::allocator<std::byte>>
    class Task {
    public:
        /// Task's promise type
        class promise_type;

        using return_type = ReturnTp;
        using allocator_type = Alloc;
        using future_type = std::future<return_type>;

        /**
         * Constructs a new empty task object. It should
         * be used only as a placeholder
         */
        explicit Task(void) noexcept = default;

        /**
         * Returns whether a coroutine has ended its job
         *
         * @return if a coroutine has ended its job
         */
        [[nodiscard]] operator bool() const;

        /**
         * Invokes a coroutine
         */
        void operator() (void) const;

        /**
         * Checks whether a task object holds a real coroutine
         * or placeholder used by default constructor
         *
         * @return if a task holds a real coroutine
         */
        [[nodiscard]] bool isValid(void) const noexcept;

        /**
         * Returns a future returned by the handled coroutine
         *
         * @return the coroutine's future object
         */
        [[nodiscard]] future_type getFuture(void);

        /**
         * Manually destroys coroutine if it has not set future
         * yet
         */
        void terminate(void) const;

        friend class Threadpool;
    private:
        using handle_t = std::coroutine_handle<promise_type>;

        /**
         * Construct a new task object from a coroutine
         * handle
         *
         * @param handle a coroutine handle object
         */
        explicit Task(handle_t handle) noexcept
            : handle{handle} {}

        /**
         * Sets a pointer to the threadpool currenly used by
         * a coroutine
         *
         * @param pool a pointer to the threadpool
         */
        void setThreadpool(Threadpool* pool) noexcept;

        handle_t                        handle = nullptr;

        /// Thats why allocator has to be stateless
        static Alloc                    allocator;
    };

    /**
     * Promise type of a coroutine handled by mpgl::task
     *
     * @tparam ReturnTp a type returned by the coroutine
     * @tparam Alloc a type of the stateless allocator that allocs
     * the coroutine on the heap
     */
    template <
        PureType ReturnTp,
        Allocator<std::byte> Alloc>
    class Task<ReturnTp, Alloc>::promise_type
        : private details::PromiseTypeInterface
    {
    public:
        /**
         * Constructs a new promise type object
         */
        explicit promise_type(void) noexcept;

        /**
         * Awaiter that manages a co_yield expression call
         *
         * @tparam Up the type holded by other task's future
         */
        template <PureType Up>
        struct YieldAwaiter {
            /**
             * Informs that awaiter is not ready
             *
             * @return false
             */
            [[nodiscard]] bool await_ready(void) const noexcept;

            /**
             * Informs that awaiter should not interrupt
             * coroutine execution
             *
             * @return false
             */
            [[nodiscard]] bool await_suspend(
                [[maybe_unused]] std::coroutine_handle<>
                ) const noexcept;

            /**
             * Returns a future to the registered coroutine
             *
             * @return the future to the registered coroutine
             */
            [[nodiscard]] std::future<Up> await_resume(
                void) noexcept;

            std::future<Up> future;
        };

        /**
         * Awaiter that manages a co_await expression call
         */
        struct SynchronizeAwaiter {
            /**
             * Checks whether there are any children coroutines
             * that are still running. If so return false and
             * the suspension proces is continued. If not it
             * permanenly abandons preempting proces
             *
             * @return if there are not any children coroutines
             */
            [[nodiscard]] bool await_ready(void) const noexcept;

            /**
             * Preempts coroutine
             */
            void await_suspend(
                 [[maybe_unused]] std::coroutine_handle<>
                 ) const noexcept{}

            /**
             * Resumes coroutine
             */
            void await_resume(void) const noexcept {}

            std::atomic<size_t>*    counter;
        };

        /**
         * Allocates a coroutine using a static stateless allocator
         * object
         *
         * @param size a size of an allocated memory
         * @return a pointer to the allocated memory
         */
        [[nodiscard]] static void* operator new(std::size_t size);

        /**
         * Deallocates a coroutine with a static stateless allocator
         * object
         *
         * @param ptr a pointer to the allocated memory
         * @param size a size of an allocated memory
         */
        static void operator delete(void* ptr, std::size_t size);

        /**
         * Creates a task object from this object
         *
         * @return a task object
         */
        [[nodiscard]] Task get_return_object(void) noexcept;

        /**
         * Always suspends a coroutine after initialization
         *
         * @return std::suspend_always
         */
        [[nodiscard]] std::suspend_always initial_suspend(
            void) const noexcept
                { return {}; }

        /**
         * Never suspends coroutine after its end and so
         * coroutine is always implicitly destroyed after
         * full execution
         *
         * @return std::suspend_always
         */
        [[nodiscard]] std::suspend_never final_suspend(
            void) const noexcept
                { return {}; }

        /**
         * Adds an other task to the threadpool and returns
         * an awaiter that manages the co_yield call
         *
         * @tparam Up the type holded by other task's future
         * @param other a rvalue reference to the other type
         * @return an yield awaiter
         */
        template <PureType Up>
        [[nodiscard]] YieldAwaiter<Up> yield_value(
            Task<Up>&& other);

        /**
         * Preemts this coroutine and returns an awaiter
         * that manages the co_await call
         *
         * @param synchronizeTag a synchronizer tag
         * @return a synchronize awaiter
         */
        [[nodiscard]] SynchronizeAwaiter await_transform(
            [[maybe_unused]] synchronize_t synchronizeTag) noexcept;

        /**
         * Saves a value returned by a coroutine
         *
         * @tparam Vp a type of retured value
         * @param value a returned value object
         */
        template <std::convertible_to<ReturnTp> Vp>
        void return_value(Vp&& value);

        /**
         * Handles exception thrown by a coroutine [propagates
         * its back to std::future given by it]
         */
        void unhandled_exception(void) noexcept;

        /**
         * Returns a future of the current coroutine
         *
         * @return the future of the current coroutine
         */
        [[nodiscard]] std::future<ReturnTp> getFuture(void);

        friend class Task;
    private:
        std::promise<ReturnTp>          promise;
        bool mutable                    asleep = false;

        /**
         * Coroutine handler that does not create a future
         * type to the current coroutine. It is being used
         * only to wrap the awakened coroutines
         *
         */
        class AwakenedCoroutine {
        public:
            /**
             * Constructs a new awakened coroutine object
             *
             * @param handle a handle to the coroutine
             */
            explicit AwakenedCoroutine(handle_t handle) noexcept
                : handle{handle} {}

            /**
             * Invokes a coroutine
             */
            void operator() (void) const;

            /**
             * Placeholder used only to trigger valid concept
             *
             * @param pool a pointer to the threadpool
             */
            void setThreadpool(
                [[maybe_unused]] Threadpool* pool) const noexcept {}
        private:
            handle_t                    handle;
        };
    };

}

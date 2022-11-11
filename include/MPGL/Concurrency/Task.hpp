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
     * this construction guarantee that all std::futures comming
     * from previous co_yield expressions will be ready. This call
     * is also faster that manually synchronizing std::futures
     * because its asleeps and preemts current coroutine and
     * wait for execution of the latest children to be awaken
     *
     * @tparam ReturnTp a type returned by the coroutine
     * @tparam Alloc a type of the stateless allocator that allocs
     * the coroutine on the heap
     *
     * @warning the allocator type must be stateless, otherwise
     * using task will result in undefined behaviour
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
        class promise_type;

        using return_type = ReturnType;
        using allocator_type = Alloc;
        using future_type = std::future<return_type>;

        [[nodiscard]] operator bool() const;

        void operator() (void) const;

        [[nodiscard]] isValid(void) const noexcept;

        [[nodiscard]] future_type getFuture(void);
    private:
        using handle_t = std::coroutine_handle<promise_type>;

        explicit Task(handle_t handle) noexcept
            : handle{handle} {}

        handle_t                        handle;

        static Alloc                    allocator;
    };

}

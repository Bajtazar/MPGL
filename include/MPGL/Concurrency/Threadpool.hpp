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

#include <MPGL/Collections/CircularList.hpp>
#include <MPGL/Concurrency/QueueMonitor.hpp>
#include <MPGL/Concurrency/Task.hpp>

namespace mpgl::async::details {

    /**
     * Checks whether the given task is a coroutine that
     * registers threadpool and does not provie any way
     * to get its future
     *
     * @tparam Task a task type
     */
    template <class Task>
    concept CoroutineTask =
        requires (Task& task, Threadpool* pool)
    {
        task.setThreadpool(pool);
    };

    /**
     * Checks whether the given task is a coroutine that
     * registers threadpool and provies its future thus allowing
     * to check if its execution has been completed
     *
     * @tparam Task a task type
     */
    template <class Task>
    concept CoroutineWorker = CoroutineTask<Task> &&
        requires (Task& task)
    {
        { task.getFuture() } -> SpecializationOf<std::future>;
    };

    /**
     * Helper struct that allows to determine the result
     * of computing given task inside threadpool. The type
     * is a std::future object parametrized with the result
     * of invoking given task
     *
     * @tparam Task a task type
     */
    template <std::invocable Task>
    struct FutureOfHelper {
        using Type = std::future<std::invoke_result_t<Task>>;
    };

    /**
     * Specialization for the coroutine tasks. This tasks does not
     * return any informations about their future thus the type
     * is simply void
     *
     * @tparam Task a task type
     */
    template <std::invocable Task>
        requires CoroutineTask<Task>
    struct FutureOfHelper<Task> {
        using Type = void;
    };

    /**
     * Specialization for the coroutine workers. This task does
     * return their future and so it is the type defined by this
     * class
     *
     * @tparam Task a task type
     */
    template <std::invocable Task>
        requires CoroutineWorker<Task>
    struct FutureOfHelper<Task> {
        using Type = decltype(std::declval<Task>().getFuture());
    };

}

namespace mpgl::async {

    /**
     * Manages a threadpool. Automaticly distributes tasks between
     * working threads and allows to get task future
     */
    class Threadpool {
    public:
        template <std::invocable Task>
        using FutureOf = typename details::FutureOfHelper<Task>::Type;

        template <std::ranges::input_range Range>
        using ResultOfRange = std::invoke_result_t<
            std::ranges::range_value_t<Range>>;

        template <std::ranges::input_range Range>
        using ResultVector = std::vector<ResultOfRange<Range>>;

        /**
         * Construct a new Threadpool object. Creates the
         * given number of working threads. If the given number
         * is equal 0 then it determines the number of working
         * threads itself
         *
         * @param size the number of working threads
         */
        explicit Threadpool(uint32 size = 0);

        Threadpool(Threadpool&&) = delete;
        Threadpool(Threadpool const&) = delete;

        Threadpool& operator=(Threadpool&&) = delete;
        Threadpool& operator=(Threadpool const&) = delete;

        /**
         * Appends task to the queue. Tries to distribute tasks
         * equally. Returns the future of the given invocable
         *
         * @tparam Func the type of the task
         * @param task the invocable to be executed
         * @return the future of the given invocable
         */
        template <std::invocable Func>
        [[nodiscard]] FutureOf<Func>
            appendTask(Func&& task);

        /**
         * Appends coroutine task to the queue. Tries to
         * distribute tasks equally
         *
         * @tparam Task a coroutine task's type
         * @param task the coroutine task object
         */
        template <details::CoroutineTask Task>
        void appendTask(Task&& task);

        /**
         * Appends coroutine worker to the queue. Tries to
         * distribute tasks equally. Returns the future of
         * the given worker
         *
         * @tparam Task a coroutine task's type
         * @param task the coroutine task object
         */
        template <details::CoroutineWorker Task>
        [[nodiscard]] FutureOf<Task>
            appendTask(Task&& task);

        /**
         * Executes given tasks in the threadpool and waits unitl
         * all of them are executed
         *
         * @tparam Range the type of tasks range
         * @param tasks the range with tasks
         */
        template <std::ranges::input_range Range>
            requires (std::invocable<std::ranges::range_value_t<Range>>
                && std::same_as<ResultOfRange<Range>, void>)
        void performTasks(Range&& tasks);

        /**
         * Executes given tasks in the threadpool and waits unitl
         * all of them are executed. Returns results of tasks
         * in std::vector in the same order they were given
         *
         * @tparam Range the type of range with tasks
         * @param tasks the range with tasks
         * @return the vector with tasks results
         */
        template <std::ranges::input_range Range>
            requires std::invocable<std::ranges::range_value_t<Range>>
        [[nodiscard]] ResultVector<Range>
            performTasks(Range&& tasks);

        /**
         * Destroy the Threadpool object. Requests all working
         * threads to stop and waits until there is no running
         * working thread
         */
        ~Threadpool(void) noexcept
            { stopSource.request_stop(); }
    private:
        template <std::invocable Func>
        using Package = std::packaged_task<
            std::invoke_result_t<Func>()>;

        template <std::ranges::input_range Range>
        using FutureOfRange = FutureOf<
            std::ranges::range_value_t<Range>>;

        template <std::ranges::input_range Range>
        using FutureVector = std::vector<FutureOfRange<Range>>;

        /**
         * Wraps tasks and serves as an unified task container
         */
        class TaskWrapper {
        public:
            /**
             * Construct a new Task Wrapper object. Wraps the given
             * task package
             *
             * @tparam Func the type of package
             * @param function the package
             */
            template <std::invocable Func>
            explicit TaskWrapper(Func&& function) noexcept
                : workerPtr{ std::make_unique<TaskWorker<Func>>(
                    std::move(function)) } {}

            /**
             * Construct a new Task Wrapper object
             */
            TaskWrapper(void) noexcept = default;

            /**
             * Calls the operator() on the wrapped object
             */
            void operator() (void) noexcept
                { workerPtr->operator()(); }

            TaskWrapper(TaskWrapper&&) noexcept = default;
            TaskWrapper(TaskWrapper const&) = delete;

            TaskWrapper& operator=(TaskWrapper&&) noexcept = default;
            TaskWrapper& operator=(TaskWrapper const&) = delete;

            /**
             * Destroy the Task Wrapper object
             */
            ~TaskWrapper(void) = default;
        private:
            /**
             * Interface of the working task
             */
            struct Worker {
                /**
                 * Construct a new Worker object
                 */
                explicit Worker(void) noexcept = default;

                /**
                 * Pure virtual operator() which has to be
                 * implemented in derived class. Should
                 * call the wrapped function operator()
                 */
                virtual void operator() (void) noexcept = 0;

                /**
                 * Destroy the Worker object
                 */
                virtual ~Worker(void) noexcept = default;
            };

            /**
             * Wraper which allows to save multiple different
             * invocables in the same collection using
             * Worker interface
             *
             * @tparam Func the wrapped invocable type
             */
            template <std::invocable Func>
            class TaskWorker : public Worker {
            public:
                /**
                 * Construct a new Task Worker object which
                 * wrapps the given invocable
                 *
                 * @param function the given invocable object
                 */
                explicit TaskWorker(Func&& function) noexcept
                    : function{ std::move(function) } {}

                /**
                 * Class the operator() on the wrapped object
                 */
                void operator() (void) noexcept
                    { function(); }

                /**
                 * Destroy the Task Worker object
                 */
                ~TaskWorker(void) noexcept = default;
            private:
                Func                                function;
            };

            typedef std::unique_ptr<Worker>         WorkerPtr;

            WorkerPtr                               workerPtr;
        };

        typedef std::vector<std::jthread>           Threads;
        typedef QueueMonitor<TaskWrapper>           TaskQueue;
        typedef typename TaskQueue::Optional        OptionalTask;
        typedef CircularList<TaskQueue>             ThreadsQueues;
        typedef typename ThreadsQueues::iterator    Attachment;
        typedef std::stop_token                     StopToken;
        typedef std::stop_source                    StopSource;

        /**
         * Wraps the link to the working task queue and provides
         * threadsafety
         */
        class QueueLink{
        public:
            /**
             * Construct a new Queue Link object
             */
            explicit QueueLink(void) noexcept = default;

            /**
             * Set the link object
             *
             * @param attachment the new link attachment
             */
            void setLink(Attachment attachment) noexcept;

            /**
             * Emplace task into one of the working queues
             *
             * @tparam Task a task type
             * @param task the task object
             */
            template <class Task>
            void emplaceTask(Task&& task);
        private:
            typedef std::mutex                      Mutex;
            typedef std::lock_guard<Mutex>          Guard;

            Attachment                              link;
            Mutex                                   mutex;
        };

        /**
         * The working function executed inside working thread
         *
         * @param stopToken the stop token
         * @param queueLink link to the workers task queue
         */
        void worker(
            StopToken stopToken,
            Attachment queueLink) noexcept;

        /**
         * Acquires task from workers queue and tries to steal
         * task from other queues when its local queue is empty
         *
         * @param localQueue link to the workers task queue
         * @return optional object with task if task was
         * acquired successfully or empty object
         */
        OptionalTask acquireTask(
            Attachment const& localQueue) noexcept;

        /**
         * Returns the size of the threadpool. Generates size
         * automatically if size is equal 0
         *
         * @param size the size given by user
         * @return the true size of the threadpool
         */
        static uint32 threadpoolSize(uint32 size) noexcept;

        Threads                                     threads;
        ThreadsQueues                               queues;
        QueueLink                                   link;
        StopSource                                  stopSource;
    };

}

#include <MPGL/Concurrency/Threadpool.tpp>

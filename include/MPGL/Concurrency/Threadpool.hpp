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

#include <functional>
#include <algorithm>
#include <memory>
#include <future>
#include <vector>
#include <thread>

namespace mpgl {

    /**
     * Manages a threadpool. Automaticly distributes tasks between
     * working threads and allows to get task future
     */
    class Threadpool {
    public:
        template <std::invocable Func>
        using FutureOf = std::future<std::result_of_t<Func()>>;

        template <std::ranges::input_range Range>
        using ResultOfRange = std::result_of_t<
            std::ranges::range_value_t<Range>()>;

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
         * equally. Returns future of the given invocable
         *
         * @tparam Func the type of the task
         * @param task the invocable to be executed
         * @return the future of the given invocable
         */
        template <std::invocable Func>
        [[nodiscard]] FutureOf<Func>
            appendTask(Func&& task);

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
        using Package = std::packaged_task<std::result_of_t<Func()>()>;

        template <std::ranges::input_range Range>
        using FutureOfRange = FutureOf<
            std::ranges::range_value_t<Range>>;

        template <std::ranges::input_range Range>
        using FutureVector = std::vector<FutureOfRange<Range>>;

        class TaskWrapper;

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
             * @tparam Func the invocable type
             * @param package the packaged invocable
             */
            template <std::invocable Func>
            void emplaceTask(Package<Func>&& package);
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
        void worker(StopToken stopToken,
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

    /**
     * Wraps tasks and serves as an unified task container
     */
    class Threadpool::TaskWrapper {
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
            : workerPtr{std::make_unique<TaskWorker<Func>>(
                std::move(function))} {}

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
                : function{std::move(function)} {}

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

    template <std::invocable Func>
    [[nodiscard]] Threadpool::FutureOf<Func>
        Threadpool::appendTask(Func&& task)
    {
        Package<Func> package{std::move(task)};
        FutureOf<Func> future = package.get_future();
        link.emplaceTask<Func>(std::move(package));
        return future;
    }

    template <std::invocable Func>
    void Threadpool::QueueLink::emplaceTask(
        Package<Func>&& package)
    {
        Guard guard{mutex};
        link->emplace(std::move(package));
        ++link;
    }

    template <std::ranges::input_range Range>
        requires (std::invocable<std::ranges::range_value_t<Range>>
            && std::same_as<Threadpool::ResultOfRange<Range>, void>)
    void Threadpool::performTasks(Range&& tasks) {
        std::vector<std::future<void>> futures;
        if constexpr (std::ranges::sized_range<Range>)
            futures.reserve(tasks.size());
        for (auto&& task : tasks)
            futures.emplace_back(appendTask(task));
        std::ranges::for_each(futures, &std::future<void>::get);
    }

    template <std::ranges::input_range Range>
        requires std::invocable<std::ranges::range_value_t<Range>>
    [[nodiscard]] Threadpool::ResultVector<Range>
        Threadpool::performTasks(Range&& tasks)
    {
        FutureVector<Range> futures;
        ResultVector<Range> results;
        if constexpr (std::ranges::sized_range<Range>) {
            futures.reserve(tasks.size());
            results.reserve(tasks.size());
        }
        for (auto&& task : tasks)
            futures.emplace_back(appendTask(task));
        std::ranges::transform(futures, std::back_inserter(results),
            &FutureOfRange<Range>::get);
        return results;
    }

}

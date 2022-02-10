#pragma once

#include "QueueMonitor.hpp"

#include <functional>
#include <memory>
#include <future>
#include <vector>
#include <thread>

namespace ge {

    class Threadpool {
    public:
        template <std::invocable Func>
        using FutureResult = std::future<std::result_of_t<Func()>>;

        explicit Threadpool(uint32 size = 0);

        Threadpool(Threadpool&&) = delete;
        Threadpool(Threadpool const&) = delete;

        Threadpool& operator=(Threadpool&&) = delete;
        Threadpool& operator=(Threadpool const&) = delete;

        template <std::invocable Func>
        [[no_discard]] FutureResult<Func>
            append(Func&& task);

        ~Threadpool(void) noexcept = default;
    private:
        class TaskWrapper;

        typedef std::vector<std::jthread>           Threads;
        typedef QueueMonitor<TaskWrapper>           TaskQueue;
        typedef std::stop_token                     StopToken;

        void worker(StopToken) noexcept;
        static uint32 threadpoolSize(uint32 size) noexcept;

        Threads                                     threads;
        TaskQueue                                   taskQueue;
    };

    class Threadpool::TaskWrapper {
    public:
        template <std::invocable Func>
        explicit TaskWrapper(Func&& function) noexcept
            : workerPtr{std::make_unique<WrappingWorker>(
                std::move(function))} {}

        void operator() (void) noexcept
            { workerPtr->operator()(); }

        TaskWrapper(TaskWrapper&&) noexcept = default;
        TaskWrapper(TaskWrapper const&) = delete;

        TaskWrapper& operator=(TaskWrapper&&) noexcept = default;
        TaskWrapper& operator=(TaskWrapper const&) = delete;

        ~TaskWrapper(void) = default;
    private:
        struct Worker {
            explicit Worker(void) noexcept = default;

            virtual void operator() (void) noexcept = 0;

            virtual ~Worker(void) noexcept = default;
        };

        template <std::invocable Func>
        class WrappingWorker {
        public:
            explicit WrappingWorker(Func&& function) noexcept
                : function{std::move(function)} {}

            void operator() (void) noexcept
                { function(); }

            ~WrappingWorker(void) noexcept = default;
        private:
            Func                                function;
        };

        typedef std::unique_ptr<Worker>         WorkerPtr;

        WorkerPtr                               workerPtr;
    };

    template <std::invocable Func>
    [[no_discard]] Threadpool::FutureResult<Func>
        Threadpool::append(Func&& task)
    {
        std::packaged_task<FutureResult<Func>(void)> package
            {std::move(task)};
        std::future<FutureResult<Func>> future{package.get_future()};
        taskQueue.emplace(std::move(package));
        return future;
    }

}

#pragma once

#include "../Collections/CircularList.hpp"
#include "QueueMonitor.hpp"

#include <functional>
#include <algorithm>
#include <memory>
#include <future>
#include <vector>
#include <thread>

namespace ge {

    class Threadpool {
    public:
        template <std::invocable Func>
        using FutureOf = std::future<std::result_of_t<Func()>>;

        template <std::ranges::input_range Range>
        using ResultOfRange = std::result_of_t<
            std::ranges::range_value_t<Range>()>;

        template <std::ranges::input_range Range>
        using ResultVector = std::vector<ResultOfRange<Range>>;

        explicit Threadpool(uint32 size = 0);

        Threadpool(Threadpool&&) = delete;
        Threadpool(Threadpool const&) = delete;

        Threadpool& operator=(Threadpool&&) = delete;
        Threadpool& operator=(Threadpool const&) = delete;

        template <std::invocable Func>
        [[nodiscard]] FutureOf<Func>
            append(Func&& task);

        template <std::ranges::input_range Range>
            requires (std::invocable<std::ranges::range_value_t<Range>>
                && std::same_as<ResultOfRange<Range>, void>)
        void performTasks(Range&& tasks);

        template <std::ranges::input_range Range>
            requires std::invocable<std::ranges::range_value_t<Range>>
        [[nodiscard]] ResultVector<Range>
            performTasks(Range&& tasks);

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

        class QueueLink{
        public:
            explicit QueueLink(void) noexcept = default;

            void setLink(Attachment attachment) noexcept;

            template <std::invocable Func>
            void emplaceTask(Package<Func>&& package);
        private:
            typedef std::mutex                      Mutex;
            typedef std::lock_guard<Mutex>          Guard;

            Attachment                              link;
            Mutex                                   mutex;
        };

        void worker(StopToken stopToken,
            Attachment queueLink) noexcept;

        OptionalTask acquireTask(
            Attachment const& localQueue) noexcept;

        static uint32 threadpoolSize(uint32 size) noexcept;

        Threads                                     threads;
        ThreadsQueues                               queues;
        QueueLink                                   link;
        StopSource                                  stopSource;
    };

    class Threadpool::TaskWrapper {
    public:
        template <std::invocable Func>
        explicit TaskWrapper(Func&& function) noexcept
            : workerPtr{std::make_unique<TaskWorker<Func>>(
                std::move(function))} {}

        TaskWrapper(void) noexcept = default;

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
        class TaskWorker : public Worker {
        public:
            explicit TaskWorker(Func&& function) noexcept
                : function{std::move(function)} {}

            void operator() (void) noexcept
                { function(); }

            ~TaskWorker(void) noexcept = default;
        private:
            Func                                function;
        };

        typedef std::unique_ptr<Worker>         WorkerPtr;

        WorkerPtr                               workerPtr;
    };

    template <std::invocable Func>
    [[nodiscard]] Threadpool::FutureOf<Func>
        Threadpool::append(Func&& task)
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
            futures.emplace_back(append(task));
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
            futures.emplace_back(append(task));
        std::ranges::transform(futures, std::back_inserter(results),
            &FutureOfRange<Range>::get);
        return results;
    }

}

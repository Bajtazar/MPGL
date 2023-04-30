/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

namespace mpgl::async {

    template <std::invocable Func>
    [[nodiscard]] Threadpool::FutureOf<Func>
        Threadpool::appendTask(Func&& task)
    {
        Package<Func> package{std::move(task)};
        auto future = package.get_future();
        link.emplaceTask(std::move(package));
        return future;
    }

    template <details::CoroutineTask Task>
    void Threadpool::appendTask(Task&& task) {
        task.setThreadpool(this);
        link.emplaceTask(std::move(task));
    }

    template <details::CoroutineWorker Task>
    [[nodiscard]] Threadpool::FutureOf<Task>
        Threadpool::appendTask(Task&& task)
    {
        task.setThreadpool(this);
        auto future = task.getFuture();
        link.emplaceTask(std::move(task));
        return future;
    }

    template <class Task>
    void Threadpool::QueueLink::emplaceTask(Task&& task) {
        Guard guard{mutex};
        link->emplace(std::move(task));
        ++link;
    }

    template <std::ranges::input_range Range>
        requires (std::invocable<std::ranges::range_value_t<Range>>
            && std::same_as<typename Threadpool::ResultOfRange<Range>, void>)
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

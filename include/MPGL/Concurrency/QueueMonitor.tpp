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

namespace mpgl {

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    [[nodiscard]] QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::popHelper(Queue& queue)
    {
        if (queue.empty())
            return {};
        Tp value = std::move(queue.front());
        queue.pop();
        return { std::move(value) };
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    [[nodiscard]] QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::pop(void)
    {
        return queue.protect(&popHelper);
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    [[nodiscard]] QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::waitAndPop(void)
    {
        return queue.wait(&popHelper);
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    void QueueMonitor<Tp, Container>::emplace(Args&&... args) {
        queue.protect((void(Queue::*)(Tp&&))&Queue::push,
            Tp{std::forward<Args>(args)...});
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::push(Tp const& value) {
        queue.protect((void(Queue::*)(Tp const&))&Queue::push,
            value);
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::push(Tp&& value) {
        queue.protect((void(Queue::*)(Tp&&))&Queue::push,
            std::move(value));
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    void QueueMonitor<Tp, Container>::emplaceAndNotify(Args&&... args) {
        queue.notify((void(Queue::*)(Tp&&))&Queue::push,
            Tp{std::forward<Args>(args)...});
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::pushAndNotify(Tp const& value) {
        queue.notify((void(Queue::*)(Tp const&))&Queue::push,
            value);
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::pushAndNotify(Tp&& value) {
        queue.notify((void(Queue::*)(Tp&&))&Queue::push,
            std::move(value));
    }

    template <PureType Tp,
        std::ranges::bidirectional_range Container>
    [[nodiscard]] bool
        QueueMonitor<Tp, Container>::empty(void) const noexcept
    {
        return queue.protect(&Queue::empty);
    }

}

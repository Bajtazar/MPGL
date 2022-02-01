#pragma once

#include "Monitor.hpp"

#include <optional>
#include <queue>

namespace ge {

    template <NotReference Tp,
        std::ranges::bidirectional_range Container = std::deque<Tp>>
    class QueueMonitor {
    public:
        typedef std::optional<Tp>                   Optional;
        typedef Container                           Underlying;
        typedef std::queue<Tp, Container>           Queue;

        explicit QueueMonitor(void) noexcept(
            NothrowDefaultConstructible<Queue>) = default;

        template <typename... Args>
            requires std::constructible_from<Queue, Args...>
        explicit QueueMonitor(Args&&... args) noexcept(
            NothrowConstructible<Queue, Args...>)
                : queue{std::forward<Args>(args)...} {}

        explicit QueueMonitor(Queue queue) noexcept(
            NothrowMoveConstructible<Queue>)
                : queue{std::move(queue)} {}

        Optional pop(void);

        template <typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplace(Args&&... args);

        void push(Tp const& value);
        void push(Tp&& value);

        void pushAndNotify(Tp const& value);
        void pushAndNotify(Tp&& value);

        template <typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplaceAndNotify(Args&&... args);

        Optional waitAndPop(void);

        bool empty(void) const noexcept;
    private:
        Monitor<Queue>                              queue;

        static Optional popHelper(Queue& queue);
    };

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::popHelper(Queue& queue)
    {
        if (queue.empty())
            return {};
        Tp value = std::move(queue.front());
        queue.pop();
        return { value };
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::pop(void)
    {
        return queue.protect(&popHelper);
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    QueueMonitor<Tp, Container>::Optional
        QueueMonitor<Tp, Container>::waitAndPop(void)
    {
        return queue.wait(&popHelper);
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    void QueueMonitor<Tp, Container>::emplace(Args&&... args) {
        queue.protect((void(Queue::*)(Tp&&))&Queue::push,
            Tp{std::forward<Args>(args)...});
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::push(Tp const& value) {
        queue.protect((void(Queue::*)(Tp const&))&Queue::push,
            value);
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::push(Tp&& value) {
        queue.protect((void(Queue::*)(Tp&&))&Queue::push,
            std::move(value));
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    void QueueMonitor<Tp, Container>::emplaceAndNotify(Args&&... args) {
        queue.notify((void(Queue::*)(Tp&&))&Queue::push,
            Tp{std::forward<Args>(args)...});
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::pushAndNotify(Tp const& value) {
        queue.notify((void(Queue::*)(Tp const&))&Queue::push,
            value);
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    void QueueMonitor<Tp, Container>::pushAndNotify(Tp&& value) {
        queue.notify((void(Queue::*)(Tp&&))&Queue::push,
            std::move(value));
    }

    template <NotReference Tp,
        std::ranges::bidirectional_range Container>
    bool QueueMonitor<Tp, Container>::empty(void) const noexcept {
        return queue.protect(&Queue::empty);
    }

}

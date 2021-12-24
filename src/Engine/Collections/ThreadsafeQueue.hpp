#pragma once

#include "../Traits/Concepts.hpp"

#include <memory>
#include <mutex>

namespace ge {

    template <NotReference Tp>
    class ThreadsafeQueue {
    public:
        typedef std::unique_ptr<Tp>                 DataPtr;

        explicit ThreadsafeQueue(void);

        template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
        explicit ThreadsafeQueue(Iter iter, Sent const& end);

        template <std::ranges::input_range Range>
        explicit ThreadsafeQueue(Range&& range);

        ThreadsafeQueue(ThreadsafeQueue&& queue) noexcept;
        ThreadsafeQueue(ThreadsafeQueue const&) = delete;

        ThreadsafeQueue& operator= (ThreadsafeQueue&& queue) noexcept;
        ThreadsafeQueue& operator= (ThreadsafeQueue const&) = delete;

        DataPtr pop(void);

        template <typename... Args>
            requires std::is_constructible_v<Tp, Args...>
        void emplace(Args&&... args);

        void push(Tp value);
        void push(DataPtr ptr);

        bool empty(void) const noexcept;
    private:
        struct Node {
            typedef std::unique_ptr<Node>           NodePtr;

            NodePtr                                 nextNode;
            DataPtr                                 data;
        };

        typedef typename Node::NodePtr              NodePtr;
        typedef Node*                               NodeTrace;
        typedef std::mutex                          Mutex;
        typedef std::lock_guard<Mutex>              LockGuard;
        typedef std::scoped_lock<Mutex, Mutex>      ScopedLock;

        Mutex                                       headMutex;
        Mutex                                       tailMutex;
        NodePtr                                     head;
        NodeTrace                                   tail;

        NodeTrace getTail(void) noexcept;
    };

    template <NotReference Tp>
    ThreadsafeQueue<Tp>::NodeTrace
        ThreadsafeQueue<Tp>::getTail(void) noexcept
    {
        LockGuard lock{tailMutex};
        return tail;
    }

    template <NotReference Tp>
    ThreadsafeQueue<Tp>::ThreadsafeQueue(void)
        : head{new Node}, tail{head.get()} {}

    template <NotReference Tp>
    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    ThreadsafeQueue<Tp>::ThreadsafeQueue(Iter iter, Sent const& end)
        : ThreadsafeQueue{}
    {
        for (; iter != end; ++iter)
            push(*iter);
    }

    template <NotReference Tp>
    template <std::ranges::input_range Range>
    ThreadsafeQueue<Tp>::ThreadsafeQueue(Range&& range)
        : ThreadsafeQueue{std::ranges::begin(range),
            std::ranges::end(range)} {}

    template <NotReference Tp>
    ThreadsafeQueue<Tp>::ThreadsafeQueue(
        ThreadsafeQueue&& queue) noexcept
    {
        ScopedLock lock{headMutex, tailMutex};
        head = std::move(queue.head);
        tail = queue.tail;
    }

    template <NotReference Tp>
    ThreadsafeQueue<Tp>&
        ThreadsafeQueue<Tp>::operator=(
            ThreadsafeQueue&& queue) noexcept
    {
        {
            ScopedLock lock{headMutex, tailMutex};
            head = std::move(queue.head);
            tail = queue.tail;
        }
        return *this;
    }

    template <NotReference Tp>
    void ThreadsafeQueue<Tp>::push(DataPtr ptr) {
        auto node = std::make_unique<Node>();
        LockGuard lock{tailMutex};
        tail->data = std::move(ptr);
        tail->nextNode = std::move(node);
        tail = tail->nextNode.get();
    }

    template <NotReference Tp>
    void ThreadsafeQueue<Tp>::push(Tp value) {
        push(std::make_unique<Tp>(std::move(value)));
    }

    template <NotReference Tp>
    template <typename... Args>
        requires std::is_constructible_v<Tp, Args...>
    void ThreadsafeQueue<Tp>::emplace(Args&&... args) {
        push(std::make_unique<Tp>(std::forward<Args>(args)...));
    }

    template <NotReference Tp>
    ThreadsafeQueue<Tp>::DataPtr
        ThreadsafeQueue<Tp>::pop(void)
    {
        LockGuard lock{headMutex};
        if (getTail() == head.get())
            return {};
        auto old = std::move(head);
        head = std::move(old->nextNode);
        return std::move(old->data);
    }

    template <NotReference Tp>
    bool ThreadsafeQueue<Tp>::empty(void) const noexcept {
        LockGuard lock{headMutex};
        return getTail() == head.get();
    }

}

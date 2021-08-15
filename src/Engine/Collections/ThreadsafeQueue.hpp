#pragma once

#include "../Utility/Concepts.hpp"

#include <memory>
#include <mutex>

namespace ge {

    template <NotReference T>
    class ThreadsafeQueue {
    public:
        typedef std::shared_ptr<T>  DataPtr;

        explicit ThreadsafeQueue(void) : head{std::make_unique<Node>()}, tail{std::ref(head)} {}
        template <std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
        explicit ThreadsafeQueue(Iter begin, Sent end);
        template <std::ranges::bidirectional_range Range>
        explicit ThreadsafeQueue(Range&& range);

        ThreadsafeQueue(ThreadsafeQueue&& queue) noexcept;
        ThreadsafeQueue(ThreadsafeQueue const& queue) = delete;

        ThreadsafeQueue& operator= (ThreadsafeQueue&& queue) noexcept;
        ThreadsafeQueue& operator= (ThreadsafeQueue const& queue) = delete;

        DataPtr pop(void);
        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        void emplace(Args&&... args);
        void push(T&& value);
        void push(T value);

        bool empty(void) const;
    private:
        struct Node {
            std::shared_ptr<T> data;
            std::unique_ptr<Node> nextNode;
            explicit Node(void) : data{nullptr}, nextNode{nullptr} {}
        };

        typedef std::unique_ptr<Node>               NodePtr;
        typedef std::reference_wrapper<NodePtr>     NodeRef;

        mutable std::mutex headMutex;
        mutable std::mutex tailMutex;

        NodePtr head;
        NodeRef tail;

        void addNode(DataPtr&& dataNode);
    };

    template <NotReference T>
    void ThreadsafeQueue<T>::push(T&& value) {
        addNode(std::move(std::make_shared<T>(std::move(value))));
    }

    template <NotReference T>
    void ThreadsafeQueue<T>::push(T value) {
        push(std::move(value));
    }

    template <NotReference T>
    template <typename... Args>
        requires std::is_constructible_v<T, Args...>
    void ThreadsafeQueue<T>::emplace(Args&&... args) {
        addNode(std::move(std::make_shared<T>(std::forward<Args>(args)...)));
    }

    template <NotReference T>
    void ThreadsafeQueue<T>::addNode(DataPtr&& dataNode) {
        auto node = std::make_unique<Node>();
        std::lock_guard<std::mutex> tailLock{tailMutex};
        tail.get()->data = dataNode;
        tail.get()->nextNode = std::move(node);
        tail = std::ref(tail.get()->nextNode);
    }

    template <NotReference T>
    std::shared_ptr<T> ThreadsafeQueue<T>::pop(void) {
        std::lock_guard<std::mutex> headLock{headMutex};
        if (head->nextNode.get() == nullptr)
            return {nullptr};
        auto oldHead = std::move(head);
        head = std::move(oldHead->nextNode);
        return oldHead->data;
    }

    template <NotReference T>
    bool ThreadsafeQueue<T>::empty(void) const {
        std::lock_guard<std::mutex> headLock{headMutex};
        return head->nextNode.get() == nullptr;
    }

    template <NotReference T>
    template <std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
    ThreadsafeQueue<T>::ThreadsafeQueue(Iter iter, Sent sentinel) : ThreadsafeQueue{} {
        std::scoped_lock<std::mutex, std::mutex> lock{headMutex, tailMutex};
        for (; iter != sentinel; ++iter) {
            auto node = std::make_unique<Node>();
            tail.get()->data = std::make_shared<T>(*iter);
            tail.get()->nextNode = std::move(node);
            tail = std::ref(tail.get()->nextNode);
        }
    }

    template <NotReference T>
    template <std::ranges::bidirectional_range Range>
    ThreadsafeQueue<T>::ThreadsafeQueue(Range&& range) : ThreadsafeQueue{
        std::ranges::begin(range), std::ranges::end(range)} {}

    template <NotReference T>
    ThreadsafeQueue<T>::ThreadsafeQueue(ThreadsafeQueue&& queue) noexcept
        : head{std::move(queue.head)}, tail{queue.tail} {}

    template <NotReference T>
    ThreadsafeQueue<T>& ThreadsafeQueue<T>::operator= (ThreadsafeQueue&& queue) noexcept {
        std::scoped_lock<std::mutex, std::mutex, std::mutex, std::mutex>
            lock{headMutex, tailMutex, queue.headMutex, queue.tailMutex};
        head = std::move(queue.head);
        tail = std::move(queue.tail);
    }



}

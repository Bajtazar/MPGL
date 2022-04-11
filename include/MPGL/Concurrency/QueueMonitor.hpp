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

#include <MPGL/Concurrency/Monitor.hpp>

#include <optional>
#include <queue>

namespace mpgl {

    /**
     * Threadsafe wrapper around queue object
     *
     * @tparam Tp the type of element stored in queue
     * @tparam Container the queue's type of underlying container
     */
    template <PureType Tp,
        std::ranges::bidirectional_range Container = std::deque<Tp>>
    class QueueMonitor {
    public:
        typedef std::optional<Tp>                   Optional;
        typedef Container                           Underlying;
        typedef std::queue<Tp, Container>           Queue;

        /**
         * Construct a new Queue Monitor object
         */
        explicit QueueMonitor(void) noexcept(
            NothrowDefaultConstructible<Queue>) = default;

        /**
         * Construct a new Queue Monitor object with given
         * arguments which are passed to queue object
         * constructor
         *
         * @tparam Args the types of arguments
         * @param args the arguments for queue object
         * constructor
         */
        template <typename... Args>
            requires std::constructible_from<Queue, Args...>
        explicit QueueMonitor(Args&&... args) noexcept(
            NothrowConstructible<Queue, Args...>)
                : queue{std::forward<Args>(args)...} {}

        /**
         * Construct a new Queue Monitor object from the given
         * queue object
         *
         * @param queue the given queue object
         */
        explicit QueueMonitor(Queue queue) noexcept(
            NothrowMoveConstructible<Queue>)
                : queue{std::move(queue)} {}

        /**
         * Returns optional object with popped element if
         * queue is not empty or empty optional object
         * if queue is empty
         *
         * @return optional object containing popped element
         * if operation was succesfull
         */
        [[nodiscard]] Optional pop(void);

        /**
         * Constructs object at the end of the queue from the given
         * arguments
         *
         * @tparam Args the arguments types
         * @param args the arguments passed to objects constructor
         */
        template <typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplace(Args&&... args);

        /**
         * Pushes values copy at the end of the queue
         *
         * @param value the value to be pushed
         */
        void push(Tp const& value);

        /**
         * Moves value at the end of the queue
         *
         * @param value the value to be pushed
         */
        void push(Tp&& value);

        /**
         * Pushes values copy at the end of the queue and
         * notifies other threads
         *
         * @param value the value to be pushed
         */
        void pushAndNotify(Tp const& value);

        /**
         * Moves value at the end of the queue and notifies
         * other threads
         *
         * @param value the value to be pushed
         */
        void pushAndNotify(Tp&& value);

        /**
         * Constructs object at the end of the queue from the given
         * arguments and notifies other threads
         *
         * @tparam Args the arguments types
         * @param args the arguments passed to objects constructor
         */
        template <typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplaceAndNotify(Args&&... args);

        /**
         * Waits for signal from notifing methods and tries to
         * pop value from queue
         *
         * @return optional object containing popped element
         * if operation was succesfull
         */
        [[nodiscard]] Optional waitAndPop(void);

        /**
         * Returns whether inner queue is empty
         *
         * @return whether queue is empty
         */
        [[nodiscard]] bool empty(void) const noexcept;
    private:
        Monitor<Queue>                              queue;

        /**
         * Helper used to pop value from queue. Gets value from
         * front of the queue and removes its first element.
         * Returns empty optional object if queue is empty
         *
         * @param queue the queue object
         * @return optional object containing popped element
         * if operation was succesfull
         */
        [[nodiscard]] static Optional popHelper(Queue& queue);
    };

}

#include <MPGL/Concurrency/QueueMonitor.tpp>

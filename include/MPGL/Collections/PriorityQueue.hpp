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

#include <MPGL/Traits/Concepts.hpp>

#include <vector>

namespace mpgl {

    /**
     * Improved version of the std::priority_queue that conects
     * access to the top element and its removal thus allowing
     * to move popped element. Works as minimum queue by default
     *
     * @tparam Tp the element type
     * @tparam Comparator the comparator functor
     * @tparam Sequence the underlying sequence
     */
    template <typename Tp,
        std::predicate<Tp, Tp> Comparator = std::greater<Tp>,
        std::ranges::forward_range Sequence = std::vector<Tp>>
    class PriorityQueue {
    public:
        using container_type = Sequence;
        using value_compare = Comparator;
        using size_type = std::size_t;
        using value_type = Tp;
        using reference = Tp&;
        using const_reference = Tp const&;

        /**
         * Constructs a new priority queue object
         *
         * @param compare the constant reference to the comparator
         * object
         */
        explicit PriorityQueue(
            value_compare const& comparator = value_compare{})
                : comparator{comparator} {}

        /**
         * Pushes the element into the priority queue
         *
         * @param element the constant reference to the element
         */
        void push(const_reference element) requires std::copyable<Tp>;

        /**
         * Pushes the element into the priority queue
         *
         * @param element the rvalue reference to the element
         */
        void push(value_type&& element) requires std::movable<Tp>;

        /**
         * Emplaces an element in the priority queue
         *
         * @tparam Args the arguments types
         * @param args the value's constructor arguments
         */
        template <typename... Args>
        void emplace(Args&&... args) requires
            std::constructible_from<value_type, Args...>;

        /**
         * Pops the element from the priority queue
         *
         * @return the popped element
         */
        [[nodiscard]] value_type pop(
            void) requires std::movable<value_type>;

        /**
         * Returns the size of the priority queue
         *
         * @return the size of the priority queue
         */
        [[nodiscard]] std::size_t size(void) const noexcept requires
            std::ranges::sized_range<container_type>
                { return sequence.size(); }

        /**
         * Returns whether the priority queue is empty
         *
         * @return if the priority queue is empty
         */
        [[nodiscard]] bool empty(void) const noexcept
            { return sequence.empty(); }
    private:
        container_type                              sequence;
        [[no_unique_address]] value_compare         comparator;
    };

}

#include <MPGL/Collections/PriorityQueue.tpp>

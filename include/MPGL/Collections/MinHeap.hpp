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

#include <MPGL/Traits/Concepts.hpp>

#include <algorithm>
#include <vector>
#include <memory>

namespace mpgl {

    /**
     * Checks whether the given type is the smart pointer
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept SmartPtr = std::is_object_v<Tp> &&
        (InstanceOf<Tp, std::shared_ptr> ||
        InstanceOf<Tp, std::unique_ptr>);

    /**
     * Compares two smart pointers with each other
     *
     * @tparam Tp the checked type
     */
    template <SmartPtr Tp>
    struct ComparePtr {
        using value_type =          std::remove_cvref_t<Tp>;
        using const_reference =     value_type const&;

        /**
         * Compares two smart pointers payload with each other
         *
         * @param left the constant reference to the left pointer
         * @param right the constant reference to the right pointer
         * @return if the payloads are equal
         */
        [[nodiscard]] constexpr bool operator() (
            const_reference left,
            const_reference right) const noexcept
                { return (*left) > (*right); }
    };

    /**
     * Returns the comparing method based on type
     *
     * @tparam Tp the type
     * @tparam Comparator the default comparator
     */
    template <typename Tp, class Comparator = std::greater<Tp>>
    using PtrComparator = std::conditional_t<SmartPtr<Tp>,
        ComparePtr<Tp>, Comparator>;

    /**
     * Represents the min heap. Allows to move the popped values
     *
     * @tparam Tp the element type
     * @tparam Compare the comparation method
     * @tparam Sequence the underlying sequence
     */
    template <typename Tp,
        std::predicate<Tp, Tp> Compare = PtrComparator<Tp>,
        std::ranges::forward_range Sequence = std::vector<Tp>>
    class MinHeap {
    public:
        typedef Sequence             container_type;
        typedef Compare              value_compare;
        typedef std::size_t          size_type;
        typedef Tp                   value_type;
        typedef value_type&          reference;
        typedef value_type const&    const_reference;

        /**
         * Constructs a new Min Heap object from the given
         * comparing object
         *
         * @param compare the constant reference to the comparing
         * object
         */
        explicit MinHeap(
            Compare const& compare = Compare{})
                : compare{compare} {}

        /**
         * Pushes the element into the heap
         *
         * @param element the constant reference to the element
         */
        template <std::enable_if_t<std::copyable<Tp>, int32> = 0>
        void push(const_reference element);

        /**
         * Pushes the element into the heap
         *
         * @param element the rvalue reference to the element
         */
        template <std::enable_if_t<std::movable<Tp>, int32> = 0>
        void push(value_type&& element);

        /**
         * Emplaces the element in the heap
         *
         * @tparam Args the arguments types
         * @param args the value's constructor arguments
         */
        template <typename... Args>
        void emplace(Args&&... args) requires (
            std::constructible_from<value_type, Args...>);

        /**
         * Pops the element from the heap
         *
         * @return the popped element
         */
        [[nodiscard]] value_type popBack(
            void) requires std::movable<value_type>;

        /**
         * Returns the size of the heap
         *
         * @return the size of the heap
         */
        [[nodiscard]] std::size_t size(void) const noexcept requires (
            std::ranges::sized_range<Sequence>)
                { return sequence.size(); }

        /**
         * Returns whether the heap is empty
         *
         * @return if the cheap is empty
         */
        [[nodiscard]] bool empty(void) const noexcept
            { return sequence.empty(); }
    private:
        Sequence                    sequence;
        Compare                     compare;
    };

}

#include <MPGL/Collections/MinHeap.tpp>

template<class Tp, std::predicate<Tp, Tp> Compare,
    std::ranges::forward_range Container, class Alloc>
struct std::uses_allocator<
    mpgl::MinHeap<Tp, Compare, Container>, Alloc> :
        std::uses_allocator<Container, Alloc>::type { };

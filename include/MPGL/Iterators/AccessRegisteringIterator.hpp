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

#include <iterator>

namespace mpgl {

    /**
     * Iterator that registers access to its elements by setting up
     * the referenced flag
     *
     * @tparam Iter the underlying iterator's type
     */
    template <std::forward_iterator Iter>
    class AccessRegisteringIterator {
    public:
        using value_type = typename Iter::value_type;
        using reference = typename Iter::reference;
        using pointer = typename Iter::pointer;
        using difference_type = typename Iter::difference_type;
        using iterator_category = typename Iter::iterator_category;
        using flag_wrapper = std::reference_wrapper<bool>;
    private:
        static constexpr bool               Bidirectional
            = std::bidirectional_iterator<Iter>;
        static constexpr bool               RandomAccess
            = std::random_access_iterator<Iter>;
    public:
        /**
         * Constructs a new access registering iterator object
         */
        explicit AccessRegisteringIterator(void) noexcept;

        /**
         * Constructs a new access registering iterator object
         *
         * @param iterator a constant reference to the wrapped iterator
         * object
         * @param flag a reference to the informed flag
         */
        explicit AccessRegisteringIterator(
            Iter const& iterator,
            bool& flag) noexcept;

        /**
         * Returns an iterator holded by this object
         *
         * @return the iterator holded by this object
         */
        [[nodiscard]] Iter const& get(void) const noexcept
            { return iter; }

        /**
         * Pre-increments the inner iterator by one
         *
         * @return the reference to object
         */
        AccessRegisteringIterator& operator++(void) noexcept
            { ++iter; return *this; }

        /**
         * Post-increments the inner iterator by one
         *
         * @return the copy of the iterator
         */
        [[nodiscard]] AccessRegisteringIterator operator++(int) noexcept
            { auto tmp = *this; ++iter; return tmp; }

        /**
         * Pre-decrements the inner iterator by one
         *
         * @return the reference to object
         */
        AccessRegisteringIterator& operator--(
            void) noexcept requires Bidirectional
                { --iter; return *this; }

        /**
         * Post-decrements the inner iterator by one
         *
         * @return the copy of the iterator
         */
        [[nodiscard]] AccessRegisteringIterator operator--(
            int) noexcept requires Bidirectional
                { auto tmp = *this; --iter; return tmp; }

        /**
         * Returns the iterator's value and sets up the flag
         *
         * @return the iterator's value
         */
        [[nodiscard]] reference operator*(void) const noexcept
            { flag.get() = true; return *iter; }

        /**
         * Returns a pointer to the iterator's value and sets
         * up the flag
         *
         * @return a pointer to the iterator's value
         */
        [[nodiscard]] pointer operator->(void) const noexcept
            { flag.get() = true; return iter.operator->(); }

        /**
         * Increments iterator by the given distance
         *
         * @param offset the incremented distance
         * @return reference to this object
         */
        AccessRegisteringIterator& operator+=(
            difference_type offset) noexcept requires RandomAccess
                { iter += offset; return *this; }

        /**
         * Decrements iterator by the given distance
         *
         * @param offset the decremented distance
         * @return reference to this object
         */
        AccessRegisteringIterator& operator-=(
            difference_type offset) noexcept requires RandomAccess
                { iter -= offset; return *this; }

        /**
         * Returns an iterator's value from further position
         * designated by given offset
         *
         * @param offset the incremented distance
         * @return the iterator's value from further position
         */
        [[nodiscard]] reference operator[](
            difference_type offset) const noexcept requires RandomAccess
                { flag.get() = true; return iter[offset]; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted iterator
         */
        [[nodiscard]] friend AccessRegisteringIterator operator+ (
            AccessRegisteringIterator const& left,
            difference_type right) noexcept requires RandomAccess
                { auto temp = left; temp += right; return temp; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the distance
         * @param right the iterator
         * @return the shifted iterator
         */
        [[nodiscard]] friend AccessRegisteringIterator operator+ (
            difference_type left,
            AccessRegisteringIterator const& right
            ) noexcept requires RandomAccess
                { auto temp = right; temp += left; return temp; }

        /**
         * Subtracts given distance from iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted operator
         */
        [[nodiscard]] friend AccessRegisteringIterator operator- (
            AccessRegisteringIterator const& left,
            difference_type right) noexcept requires RandomAccess
                { auto temp = left; temp -= right; return temp; }

        /**
         * Returns distance between iterators
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return difference_type
         */
        [[nodiscard]] friend difference_type operator- (
            AccessRegisteringIterator const& left,
            AccessRegisteringIterator const& right
            ) noexcept requires RandomAccess
                { return left.iter - right.iter; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend auto operator<=> (
            AccessRegisteringIterator const& left,
            AccessRegisteringIterator const& right
            ) noexcept requires RandomAccess
                { return left.iter <=> right.iter; }

        /**
         * Returns whether two iterators are equal
         *
         * @param left the left iterator object
         * @param right the right iterator object
         * @return if the iterators are equal
         */
        [[nodiscard]] friend bool operator== (
            AccessRegisteringIterator const& left,
            AccessRegisteringIterator const& right) noexcept
                { return left.iter == right.iter; }
    private:
        Iter                                iter;
        flag_wrapper                        flag;

        static bool                         DefaultFlag;
    };

}

#include <MPGL/Iterators/AccessRegisteringIterator.tpp>

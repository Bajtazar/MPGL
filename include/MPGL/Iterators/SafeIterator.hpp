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

#include <algorithm>
#include <iterator>
#include <compare>

#include <MPGL/Exceptions/SafeIteratorOutOfRangeException.hpp>

namespace mpgl {

    /**
     * Declaration of the safe iterator class
     *
     * @tparam Iter the wrapped iterator type
     * @tparam Sent the sentinel type of the iterator
     */
    template <class Iter, std::sentinel_for<Iter> Sent = Iter>
        requires (std::random_access_iterator<Iter> ||
            std::same_as<std::istreambuf_iterator<char>, Iter>)
    class SafeIterator;

    /**
     * Declaration of the random access iterator version
     * of the safe iterator
     *
     * @tparam Iter the wrapped iterator type
     * @tparam Sent the sentinel type of the iterator
     */
    template <std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    class SafeIterator<Iter, Sent>
        : public std::iterator<std::random_access_iterator_tag,
            typename std::iterator_traits<Iter>::value_type>
    {
        typedef std::iterator_traits<Iter>          traits;
    public:
        typedef Iter                                iterator_type;
        typedef Sent                                sentinel_type;
        typedef typename traits::difference_type    difference_type;
        typedef typename traits::value_type         value_type;
        typedef typename traits::pointer            pointer;
        typedef typename traits::reference          reference;
        typedef std::random_access_iterator_tag     iterator_category;
        typedef std::compare_three_way_result_t<
            iterator_type, iterator_type>           compare;

        /**
         * Constructs a new safe iterator object from the given
         * iterator and its sentinel
         *
         * @param iter the constant reference to the iterator
         * @param sent the constant reference to the sentinel
         */
        constexpr explicit SafeIterator(
            iterator_type const& iter,
            sentinel_type const& sent) noexcept
                : iter{iter}, begin{iter}, sentinel{sent} {}

        /**
         * Constructs a new safe iterator object
         */
        constexpr explicit SafeIterator(void) noexcept = default;

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        constexpr SafeIterator& operator++(void) noexcept
            { ++iter; return *this; }

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr SafeIterator operator++(int) noexcept
            { auto temp = *this; ++iter; return temp; }

        /**
         * Decrements iterator by one
         *
         * @return reference to this object
         */
        constexpr SafeIterator& operator--(void) noexcept
            { --iter; return *this; }

        /**
         * Post-decrements iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr SafeIterator operator--(int) noexcept
            { auto temp = *this; --iter; return temp; }

        /**
         * Returns the value of the wrapped iterator
         *
         * @return the value of the wrapped iterator
         */
        [[nodiscard]] constexpr reference operator*(void) const;

        /**
         * Returns the pointer to the value of the wrapped iterator
         *
         * @return the pointer to the value of the wrapped iterator
         */
        [[nodiscard]] constexpr pointer operator->(void) const;

        /**
         * Increments iterator by the given distance
         *
         * @param offset the incremented distance
         * @return reference to this object
         */
        constexpr SafeIterator& operator+=(
            difference_type offset) noexcept
                { iter += offset; return *this; }

        /**
         * Decrements iterator by the given distance
         *
         * @param offset the decremented distance
         * @return reference to this object
         */
        constexpr SafeIterator& operator-=(
            difference_type offset) noexcept
                { iter -= offset; return *this; }

        /**
         * Returns an element shifted by the given offset
         *
         * @param offset the incremented distance
         * @return the element shifted by the given offse
         */
        [[nodiscard]] constexpr value_type operator[](
            difference_type offset)
                { auto tmp = *this + offset; return *tmp; }

        /**
         * Returns whether the iterator is in the safe range
         *
         * @return if the iterator is in the safe range
         */
        [[nodiscard]] constexpr bool isSafe(void) const noexcept
            { return iter > sentinel && iter <= begin; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted iterator
         */
        [[nodiscard]] friend constexpr SafeIterator
            operator+ (
                SafeIterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp += right; return temp; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the distance
         * @param right the iterator
         * @return the shifted iterator
         */
        [[nodiscard]] friend constexpr SafeIterator
            operator+ (
                difference_type left,
                SafeIterator const& right) noexcept
            { auto temp = right; temp += left; return temp; }

        /**
         * Subtracts given distance from iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted operator
         */
        [[nodiscard]] friend constexpr SafeIterator
            operator- (
                SafeIterator const& left,
                difference_type right) noexcept
            { return left.iter - right; }

        /**
         * Returns distance between iterators
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return difference_type
         */
        [[nodiscard]] friend constexpr difference_type
            operator- (
                SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter - right.iter; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool
            operator== (
                SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter == right.iter; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend constexpr compare
            operator<=> (SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter <=> right.iter; }
    private:
        iterator_type                               iter;
        iterator_type                               begin;
        sentinel_type                               sentinel;
    };

    /**
     * Declaration of the char istreambuf iterator version
     * of the safe iterator
     */
    template <>
    class SafeIterator<std::istreambuf_iterator<char>,
        std::istreambuf_iterator<char>>
        : public std::iterator<std::input_iterator_tag, char> {
    public:
        typedef std::istreambuf_iterator<char>      iterator_type;
        typedef std::istreambuf_iterator<char>      sentinel_type;
        typedef std::ptrdiff_t                      difference_type;
        typedef char                                value_type;
        typedef std::input_iterator_tag             iterator_category;

        /**
         * Constructs a new safe iterator object from the given
         * iterator and its sentinel
         *
         * @param iter the constant reference to the iterator
         * @param sent the constant reference to the sentinel
         */
        constexpr explicit SafeIterator(
            iterator_type const& iter,
            sentinel_type const& sent) noexcept
                : iter{iter}, sentinel{sent} {}

        /**
         * Constructs a new safe iterator object
         */
        constexpr explicit SafeIterator(void) noexcept = default;

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        SafeIterator& operator++(void) noexcept
            { ++iter; return *this; }

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] SafeIterator operator++(int) noexcept
            { auto temp = *this; temp.iter = iter++; return temp; }

        /**
         * Returns the value of the wrapped iterator
         *
         * @return the value of the wrapped iterator
         */
        [[nodiscard]] value_type operator*(void) const;

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend bool operator== (SafeIterator const& left,
            SafeIterator const& right) noexcept
                { return left.iter == right.iter; }

        /**
         * Returns whether the iterator is in the safe range
         *
         * @return if the iterator is in the safe range
         */
        [[nodiscard]] bool isSafe(void) const noexcept
            { return iter != sentinel; }
    private:
        iterator_type                               iter;
        sentinel_type                               sentinel;
    };

}

#include <MPGL/Iterators/SafeIterator.tpp>

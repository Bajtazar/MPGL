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

#include <MPGL/Exceptions/SafeIteratorOutOfRangeException.hpp>
#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Traits/Concepts.hpp>

#include <algorithm>
#include <iterator>
#include <compare>

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
    class SafeIterator<Iter, Sent> {
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
         * Returns an underlying iterator
         */
        [[nodiscard]] constexpr iterator_type getIter(
            void) const noexcept
                { return iter; }

        /**
         * Returns an underlying iterator to the begining of the range
         */
        [[nodiscard]] constexpr iterator_type getBegin(
            void) const noexcept
                { return begin; }

        /**
         * Returns an underlying iterator to the end of the range
         */
        [[nodiscard]] constexpr sentinel_type getSent(
            void) const noexcept
                { return sentinel; }

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
            { return iter < sentinel && iter >= begin; }

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
         * Returns distance between iterators
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return difference_type
         */
        [[nodiscard]] friend constexpr difference_type
            operator- (
                SafeIterator const& left,
                iterator_type const& right) noexcept
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
                sentinel_type const& left,
                SafeIterator const& right) noexcept
            { return left - right.iter; }

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
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool
            operator== (
                iterator_type const& left,
                SafeIterator const& right) noexcept
            { return left == right.iter; }

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
                iterator_type const& right) noexcept
            { return left.iter == right; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend constexpr compare
            operator<=> (
                SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter <=> right.iter; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend constexpr compare
            operator<=> (
                SafeIterator const& left,
                iterator_type const& right) noexcept
            { return left.iter <=> right; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend constexpr compare
            operator<=> (
                iterator_type const& left,
                SafeIterator const& right) noexcept
            { return left <=> right.iter; }
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
    {
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
         * Returns an underlying iterator
         */
        [[nodiscard]] constexpr iterator_type getIter(
            void) const noexcept
                { return iter; }

        /**
         * Returns an underlying iterator to the end of the range
         */
        [[nodiscard]] constexpr sentinel_type getSent(
            void) const noexcept
                { return sentinel; }

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
        [[nodiscard]] friend bool operator== (
            SafeIterator const& left,
            SafeIterator const& right) noexcept
                { return left.iter == right.iter; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend bool operator== (
            iterator_type const& left,
            SafeIterator const& right) noexcept
                { return left == right.iter; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend bool operator== (
            SafeIterator const& left,
            iterator_type const& right) noexcept
                { return left.iter == right; }

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

    /**
     * Creates an iterator suitable for the given policy. If the
     * policy is secure then returns the safe iterator, otherwise
     * returns range's iterator
     *
     * @throws SecurityUnknownPolicyException when the given security
     * type is unknown
     * @tparam Policy the security policy type
     * @tparam Range the range type
     * @param range the universal reference to the range object
     * @return the iterator suitable for the given policy
     */
    template <
        security::SecurityPolicy Policy,
        std::ranges::random_access_range Range>
    [[nodiscard]] constexpr auto makeIterator(Range&& range);

    /**
     * Creates an iterator suitable for the given policy. If the
     * policy is secure then returns the safe iterator, otherwise
     * returns unmodified iterator
     *
     * @throws SecurityUnknownPolicyException when the given security
     * type is unknown
     * @tparam Policy the security policy type
     * @tparam Iter the iterator's type
     * @tparam Sent the sentinel's type
     * @param iter the constant reference to the iterator object
     * @param sent the constant reference to the sentinel object
     * @return the iterator suitable for the given policy
     */
    template <
        security::SecurityPolicy Policy,
        std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    [[nodiscard]] constexpr auto makeIterator(
        Iter const& iter,
        [[maybe_unused]] Sent const& sent);

    /**
     * Creates an iterator suitable for the given policy. If the
     * policy is secure then returns the safe iterator, otherwise
     * returns unmodified iterator
     *
     * @throws SecurityUnknownPolicyException when the given security
     * type is unknown
     * @tparam Policy the security policy type
     * @param iter the constant reference to the iterator object
     * @param sent the constant reference to the sentinel object
     * @return the iterator suitable for the given policy
     */
    template <security::SecurityPolicy Policy>
    [[nodiscard]] auto makeIterator(
        std::istreambuf_iterator<char> const& iter,
        [[maybe_unused]] std::istreambuf_iterator<char> const& sent);

    /**
     * Defines an iterator suitable for the given security policy
     *
     * @tparam Policy the security policy type
     * @tparam Iter the iterator's type
     * @tparam Sent the sentinel's type
     */
    template <
        security::SecurityPolicy Policy,
        typename Iter,
        typename Sent>
    using PolicyIterIT = decltype(makeIterator<Policy>(
        std::declval<Iter>(), std::declval<Sent>()));

    /**
     * Defines an iterator suitable for the given security policy
     *
     * @tparam Policy the security policy type
     * @tparam Range the range's type
     */
    template <
        security::SecurityPolicy Policy,
        std::ranges::random_access_range Range>
    using PolicyIterRT = decltype(makeIterator<Policy>(
        std::declval<Range>()));

    /**
     * Defines a safe iterator suitable for the given range
     *
     * @tparam Range the range's type
     */
    template <std::ranges::range Range>
        requires (std::random_access_iterator<
                std::ranges::iterator_t<Range>> ||
            std::same_as<std::istreambuf_iterator<char>,
                std::ranges::iterator_t<Range>>)
    using RangeSafeIter = SafeIterator<
        std::ranges::iterator_t<Range>,
        std::ranges::sentinel_t<Range>>;

    /**
     * Erases the subrange indicated by the iterators from the given
     * range
     *
     * @throws SecurityUnknownPolicyException when the given security
     * type is unknown
     * @tparam Range the range's type
     * @param range an universal reference to the range object
     * @param begin a constant reference to the begining of the
     * erased subrange
     * @param end a constant reference to the end of the
     * erased subrange
     */
    template <ErasableRange Range>
    void erase(
        Range&& range,
        RangeSafeIter<Range> const& begin,
        RangeSafeIter<Range> const& end);

    /**
     * Erases the subrange indicated by the iterators from the given
     * range
     *
     * @throws SecurityUnknownPolicyException when the given security
     * type is unknown
     * @tparam Policy the iterator's policy type
     * @tparam Range the range's type
     * @param range an universal reference to the range object
     * @param begin a constant reference to the begining of the
     * erased subrange
     * @param end a constant reference to the end of the
     * erased subrange
     */
    template <
        security::SecurityPolicy Policy,
        ErasableRange Range>
    void erase(
        Range&& range,
        PolicyIterRT<Policy, Range> const& begin,
        PolicyIterRT<Policy, Range> const& end);

}

#include <MPGL/Iterators/SafeIterator.tpp>

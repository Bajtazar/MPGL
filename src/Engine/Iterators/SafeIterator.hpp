#pragma once

#include <algorithm>
#include <iterator>
#include <compare>

#include "../Exceptions/SafeIteratorOutOfRangeException.hpp"

namespace mpgl {

    template <class Iter, std::sentinel_for<Iter> Sent = Iter>
        requires (std::random_access_iterator<Iter> ||
            std::same_as<std::istreambuf_iterator<char>, Iter>)
    class SafeIterator;

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

        constexpr explicit SafeIterator(
            iterator_type const& iter,
            sentinel_type const& sent) noexcept
                : iter{iter}, begin{iter}, sentinel{sent} {}
        constexpr explicit SafeIterator(void) noexcept = default;

        constexpr SafeIterator& operator++(void) noexcept
            { ++iter; return *this; }
        constexpr SafeIterator  operator++(int)  noexcept
            { auto temp = *this; ++iter; return temp; }

        constexpr SafeIterator& operator--(void) noexcept
            { --iter; return *this; }
        constexpr SafeIterator  operator--(int)  noexcept
            { auto temp = *this; --iter; return temp; }

        constexpr reference operator*(void) const;
        constexpr pointer operator->(void) const;

        constexpr SafeIterator& operator+=(
            difference_type offset) noexcept
                { iter += offset; return *this; }

        constexpr SafeIterator& operator-=(
            difference_type offset) noexcept
                { iter -= offset; return *this; }

        constexpr SafeIterator operator[](
            difference_type offset) noexcept
                { auto temp = *this; temp += offset; return temp; }

        constexpr bool isSafe(void) const noexcept
            { return iter > sentinel && iter <= begin; }

        friend constexpr SafeIterator
            operator+ (SafeIterator const& right,
                difference_type left) noexcept
            { auto temp = right; temp += left; return temp; }

        friend constexpr SafeIterator
            operator+ (difference_type right,
                SafeIterator const& left) noexcept
            { auto temp = left; temp += right; return temp; }

        friend constexpr SafeIterator
            operator- (SafeIterator const& right,
                difference_type left) noexcept
            { return right.iter - left; }

        friend constexpr difference_type
            operator- (SafeIterator const& right,
                SafeIterator const& left) noexcept
            { return right.iter - left.iter; }

        friend constexpr bool
            operator== (SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter == right.iter; }

        friend constexpr compare
            operator<=> (SafeIterator const& left,
                SafeIterator const& right) noexcept
            { return left.iter <=> right.iter; }
    private:
        iterator_type iter;
        iterator_type begin;
        sentinel_type sentinel;
    };

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

        constexpr explicit SafeIterator(
            iterator_type const& iter,
            sentinel_type const& sent) noexcept
                : iter{iter}, sentinel{sent} {}
        constexpr explicit SafeIterator(void) noexcept = default;

        SafeIterator& operator++(void) noexcept
            { ++iter; return *this; }
        SafeIterator  operator++(int)  noexcept
            { auto temp = *this; temp.iter = iter++; return temp; }

        value_type operator*(void) const;

        friend bool operator== (SafeIterator const& left,
            SafeIterator const& right) noexcept
                { return left.iter == right.iter; }

        bool isSafe(void) const noexcept
            { return iter != sentinel; }
    private:
        iterator_type iter;
        sentinel_type sentinel;
    };

    template <std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    constexpr SafeIterator<Iter, Sent>::reference
        SafeIterator<Iter, Sent>::operator*(void) const
    {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return *iter;
    }

    template <std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    constexpr SafeIterator<Iter, Sent>::pointer
        SafeIterator<Iter, Sent>::operator->(void) const
    {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return std::addressof(*iter);
    }

}

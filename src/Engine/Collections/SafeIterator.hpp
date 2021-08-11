#pragma once

#include <algorithm>
#include <iterator>

#include "../Exceptions/SafeIteratorOutOfRangeException.hpp"

namespace ge {

    template <std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
    class SafeIterator
        : public std::iterator<std::random_access_iterator_tag,
            typename std::iterator_traits<Iter>::value_type>
    {
        typedef std::iterator_traits<Iter>          traits;
    public:
        typedef Iter                                iterator_type;
        typedef typename traits::difference_type    difference_type;
        typedef typename traits::value_type         value_type;
        typedef typename traits::pointer            pointer;
        typedef typename traits::reference          reference;

        constexpr explicit SafeIterator(Iter const& iter, Sent const& sent) noexcept : iter{iter}, begin{iter}, sent{sent} {}
        constexpr explicit SafeIterator(Iter iter, Sent sent) noexcept : iter{iter}, begin{iter}, sentinel{sent} {}
        constexpr explicit SafeIterator(void) noexcept = default;

        constexpr SafeIterator& operator++(void) noexcept { ++iter; return *this; }
        constexpr SafeIterator  operator++(int)  noexcept { auto temp = *this; ++iter; return temp; }

        constexpr SafeIterator& operator--(void) noexcept { --iter; return *this; }
        constexpr SafeIterator  operator--(int)  noexcept { auto temp = *this; --iter; return temp; }

        constexpr reference operator*(void) const;
        constexpr pointer operator->(void) const;

        constexpr SafeIterator& operator+=(difference_type offset) noexcept { iter += offset; return *this; }
        constexpr SafeIterator& operator-=(difference_type offset) noexcept { iter -= offset; return *this; }
        constexpr SafeIterator operator[](difference_type offset) noexcept { auto temp = *this; temp += offset; return temp; }

        friend constexpr bool operator== (SafeIterator const& left, SafeIterator const& right) noexcept { return left.iter == right.iter; }

        friend constexpr SafeIterator operator+ (SafeIterator const& right, difference_type left) noexcept { auto temp = right; temp += left; return temp; }
        friend constexpr SafeIterator operator+ (difference_type right, SafeIterator const& left) noexcept { auto temp = left; temp += right; return temp; }
        friend constexpr SafeIterator operator- (SafeIterator const& right, difference_type left) noexcept { return right.iter - left; }
        friend constexpr difference_type operator- (SafeIterator const& right, SafeIterator const& left) noexcept { return right.iter - left.iter; }

        friend constexpr bool operator> (SafeIterator const& left, SafeIterator const& right) noexcept { return left.iter > right.iter; }
        friend constexpr bool operator< (SafeIterator const& left, SafeIterator const& right) noexcept { return left.iter < right.iter; }

        friend constexpr bool operator>= (SafeIterator const& left, SafeIterator const& right) noexcept { return left.iter >= right.iter; }
        friend constexpr bool operator<= (SafeIterator const& left, SafeIterator const& right) noexcept { return left.iter <= right.iter; }
    private:
        Iter iter;
        Iter const begin;
        Sent const sentinel;
    };

    template <std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
    constexpr SafeIterator<Iter, Sent>::reference SafeIterator<Iter, Sent>::operator*(void) const {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return *iter;
    }

    template <std::random_access_iterator Iter, std::sentinel_for<Iter> Sent>
    constexpr SafeIterator<Iter, Sent>::pointer SafeIterator<Iter, Sent>::operator->(void) const {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return std::addressof(*iter);
    }

}

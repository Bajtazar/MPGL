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

#include <MPGL/Exceptions/SecurityUnknownPolicyException.hpp>

namespace mpgl {

    template <std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    [[nodiscard]] constexpr SafeIterator<Iter, Sent>::reference
        SafeIterator<Iter, Sent>::operator*(void) const
    {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return *iter;
    }

    template <std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    [[nodiscard]] constexpr SafeIterator<Iter, Sent>::pointer
        SafeIterator<Iter, Sent>::operator->(void) const
    {
        if (iter >= sentinel || iter < begin)
            throw SafeIteratorOutOfRangeException{};
        return std::addressof(*iter);
    }

    template <
        security::SecurityPolicy Policy,
        std::ranges::random_access_range Range>
    [[nodiscard]] constexpr auto makeIterator(Range&& range) {
        return makeIterator<Policy>(
            std::ranges::begin(range),
            std::ranges::end(range));
    }

    template <
        security::SecurityPolicy Policy,
        std::random_access_iterator Iter,
        std::sentinel_for<Iter> Sent>
    [[nodiscard]] constexpr auto makeIterator(
        Iter const& iter,
        [[maybe_unused]] Sent const& sent)
    {
        if constexpr (security::isSecurePolicy<Policy>) {
            return SafeIterator<Iter, Sent>{ iter, sent };
        } else if constexpr (security::isUnsecuredPolicy<Policy>) {
            return Iter{ iter };
        } else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    [[nodiscard]] auto makeIterator(
        std::istreambuf_iterator<char> const& iter,
        [[maybe_unused]] std::istreambuf_iterator<char> const& sent)
    {
        if constexpr (security::isSecurePolicy<Policy>) {
            return SafeIterator<std::istreambuf_iterator<char>>{
                iter, sent };
        } else if constexpr (security::isUnsecuredPolicy<Policy>) {
            return std::istreambuf_iterator<char>{ iter };
        } else
            throw SecurityUnknownPolicyException{};
    }

    template <ErasableRange Range>
    void erase(
        Range&& range,
        RangeSafeIter<Range> const& begin,
        RangeSafeIter<Range> const& end)
    {
        range.erase(begin.getIter(), end.getIter());
    }

    template <
        security::SecurityPolicy Policy,
        ErasableRange Range>
    void erase(
        Range&& range,
        PolicyIterRT<Policy, Range> const& begin,
        PolicyIterRT<Policy, Range> const& end)
    {
        if constexpr (security::isSecurePolicy<Policy>) {
            erase(std::forward<Range>(range), begin, end);
        } else if constexpr (security::isUnsecuredPolicy<Policy>) {
            range.erase(begin, end);
        } else
            throw SecurityUnknownPolicyException{};
    }

}

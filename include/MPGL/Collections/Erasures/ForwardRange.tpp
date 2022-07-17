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

namespace mpgl::exp {

    namespace details {

        template <std::ranges::forward_range Range>
        [[nodiscard]] WrappedRangeIterator<Range>::reference
            WrappedRangeIterator<Range>::next(
                void) noexcept requires (
                    WrappedRangeIterator<Range>::PlainType)
        {
            auto& ref = *iter;
            ++iter;
            return ref;
        }

        template <std::ranges::forward_range Range>
        [[nodiscard]] WrappedRangeIterator<Range>::const_reference
            WrappedRangeIterator<Range>::next(
                void) const noexcept
        {
            auto const& cref = *iter;
            ++iter;
            return cref;
        }

        template <std::ranges::forward_range Range>
        [[nodiscard]] WrappedRangeIterator<Range>*
            WrappedRangeIterator<Range>::clone(void) 
        {
            return new WrappedRangeIterator{iter, sent};
        }

    }

    template <PureType Tp>
    template <std::ranges::forward_range Rg>
    [[nodiscard]] ForwardRange<Tp>::WrappedRange<Rg>::IterPtr
        ForwardRange::WrappedRange::iterator(void) noexcept 
    {
        return std::make_unique<WIter>(
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    template <PureType Tp>
    template <std::ranges::forward_range Rg>
    [[nodiscard]] ForwardRange<Tp>::WrappedRange<Rg>::ConstIterPtr
        ForwardRange::WrappedRange::iterator(void) const noexcept 
    {
        return std::make_unique<WConstIter>(
            std::ranges::cbegin(range),
            std::ranges::cend(range)
        );
    }

    template <PureType Tp>
    template <std::ranges::forward_range Range>
        requires std::same_as<Tp, std::ranges::range_value_t<Range>>
    ForwardRange<Tp>::ForwardRange(Range&& range)
        : rangePointer{std::make_unique<WrappedRange<Range>>(
            std::forward(range))} {}

    template <PureType Tp>
    ForwardRange<Tp>::ForwardRange(
        ForwardRange const& forwardRange)
            : rangePointer{forwardRange.rangePointer->clone()} {}

    template <PureType Tp>
    ForwardRange<Tp>& ForwardRange<Tp>::operator=(
        ForwardRange const& forwardRange) 
    {
        rangePointer = std::unique_ptr{
            forwardRange.rangePointer->clone()};
        return *this;
    }

}

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

#include <utility>

namespace mpgl {

    namespace details {

        template <std::ranges::input_range Range>
        [[nodiscard]] WrappedRangeIterator<Range>::Base*
            WrappedRangeIterator<Range>::clone(void) 
        {
            return new WrappedRangeIterator{iter, sent};
        }

    }

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] RangeErasurer<Tp>::RangeInterface::IterPtr
        RangeErasurer<Tp>::WrappedRange<Rg>::iterator(void) noexcept 
    {
        return std::make_unique<WIter>(
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    template <PureType Tp>
    template <std::ranges::input_range Range>
        requires std::same_as<Tp, std::ranges::range_value_t<Range>>
    RangeErasurer<Tp>::RangeErasurer(Range&& range)
        : rangePointer{std::make_unique<WrappedRange<Range>>(
            std::forward<Range>(range))} {}

    template <PureType Tp>
    RangeErasurer<Tp>::RangeErasurer(
        RangeErasurer const& rangeErasurer)
            : rangePointer{rangeErasurer.rangePointer->clone()} {}

    template <PureType Tp>
    RangeErasurer<Tp>& RangeErasurer<Tp>::operator=(
        RangeErasurer const& rangeErasurer) 
    {
        rangePointer = std::unique_ptr{
            rangeErasurer.rangePointer->clone()};
        return *this;
    }

    template <PureType Tp>
    template <std::ranges::input_range Range>
        requires std::same_as<Tp, std::ranges::range_value_t<Range>>
    RangeErasurer<Tp>& RangeErasurer<Tp>::operator=(Range&& range) {
        rangePointer = std::make_unique<WrappedRange<Range>>(
            std::forward<Range>(range));
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    RangeErasurer<Tp>::Iterator<BaseTp>::Iterator(
        Iterator const& iterator)
            : iterPtr{iterator.iterPtr ? 
                iterator.iterPtr->clone() : nullptr} {}

    template <PureType Tp>
    template <typename BaseTp>
    RangeErasurer<Tp>::Iterator<BaseTp>&
        RangeErasurer<Tp>::Iterator<BaseTp>::operator=(
            Iterator const& iterator)
    {
        iterPtr = UnderlyingPtr{
            iterator.iterPtr ? 
            iterator.iterPtr->clone() : nullptr
        };
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    [[nodiscard]] RangeErasurer<Tp>::Iterator<BaseTp>
        RangeErasurer<Tp>::Iterator<BaseTp>::operator++(int)
    {
        auto copy = *this;
        iterPtr->increment();
        return *this;
    }

}

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

#include <MPGL/Utility/Ranges.hpp>

namespace mpgl::any {

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] InputRange<Tp>::WrappedRange<Rg>::IIter*
        InputRange<Tp>::WrappedRange<Rg>::Iterator::clone(void)
    {
        return new Iterator{*this};
    }

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] InputRange<Tp>::RangeInterface::IterPtr
        InputRange<Tp>::WrappedRange<Rg>::iterator(void) noexcept
    {
        return std::make_unique<Iterator>(
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    template <PureType Tp>
    InputRange<Tp>::InputRange(InputRangeCompatible<Tp> auto&& range)
        : rangePointer{std::make_unique<WrappedRange<Range>>(
            std::forward<Range>(range))} {}

    template <PureType Tp>
    InputRange<Tp>::InputRange(
        InputRange const& inputRange)
            : rangePointer{inputRange.rangePointer->clone()} {}

    template <PureType Tp>
    InputRange<Tp>& InputRange<Tp>::operator=(
        InputRange const& inputRange)
    {
        // input range has to have the valid pointer
        rangePointer = std::unique_ptr{
            inputRange.rangePointer->clone()};
        return *this;
    }

    template <PureType Tp>
    InputRange<Tp>& InputRange<Tp>::operator=(
        InputRangeCompatible<Tp> auto&& range)
    {
        rangePointer = std::make_unique<WrappedRange<Range>>(
            std::forward<Range>(range));
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    InputRange<Tp>::Iterator<BaseTp>::Iterator(
        Iterator const& iterator)
            : iterPtr{clone(iterator.iterPtr)} {}

    template <PureType Tp>
    template <typename BaseTp>
    InputRange<Tp>::Iterator<BaseTp>&
        InputRange<Tp>::Iterator<BaseTp>::operator=(
            Iterator const& iterator)
    {
        iterPtr = UnderlyingPtr{clone(iterator.iterPtr)};
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    [[nodiscard]] InputRange<Tp>::Iterator<BaseTp>
        InputRange<Tp>::Iterator<BaseTp>::operator++(int)
    {
        auto copy = *this;
        iterPtr->increment();
        return *this;
    }

}

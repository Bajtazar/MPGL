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

        template <std::ranges::input_range Range>
        [[nodiscard]] WrappedRangeIterator<Range>::Base*
            WrappedRangeIterator<Range>::clone(void) 
        {
            return new WrappedRangeIterator{iter, sent};
        }

    }

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] InputRange<Tp>::RangeInterface::IterPtr
        InputRange<Tp>::WrappedRange<Rg>::iterator(void) noexcept 
    {
        return std::make_unique<WIter>(
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] InputRange<Tp>::RangeInterface::ConstIterPtr
        InputRange<Tp>::WrappedRange<Rg>::citerator(void) const noexcept 
    {
        return std::make_unique<WConstIter>(
            std::ranges::cbegin(range),
            std::ranges::cend(range)
        );
    }

    template <PureType Tp>
    template <std::ranges::input_range Range>
        requires std::same_as<Tp, std::ranges::range_value_t<Range>>
    InputRange<Tp>::InputRange(Range&& range)
        : rangePointer{std::make_unique<WrappedRange<Range>>(
            std::forward(range))} {}

    template <PureType Tp>
    InputRange<Tp>::InputRange(
        InputRange const& inputRange)
            : rangePointer{inputRange.rangePointer->clone()} {}

    template <PureType Tp>
    InputRange<Tp>& InputRange<Tp>::operator=(
        InputRange const& inputRange) 
    {
        rangePointer = std::unique_ptr{
            inputRange.rangePointer->clone()};
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    InputRange<Tp>::Iterator<BaseTp>::Iterator(
        Iterator const& iterator)
            : iterPtr{iterator.iterPtr ? 
                iterator.iterPtr->clone() : nullptr} {}

    template <PureType Tp>
    template <typename BaseTp>
    InputRange<Tp>::Iterator<BaseTp>&
        InputRange<Tp>::Iterator<BaseTp>::operator=(
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
    [[nodiscard]] InputRange<Tp>::Iterator<BaseTp>
        InputRange<Tp>::Iterator<BaseTp>::operator++(int)
    {
        auto copy = *this;
        iterPtr->increment();
        return *this;
    }

}

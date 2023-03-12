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
#include <new>

#include <MPGL/Utility/Ranges.hpp>

namespace mpgl::any {

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] constexpr InputRange<Tp>::WrappedRange<Rg>::IIter*
        InputRange<Tp>::WrappedRange<Rg>::Iterator::clone(void)
    {
        return new Iterator{*this};
    }

    template <PureType Tp>
    template <std::ranges::input_range Rg>
    [[nodiscard]] constexpr InputRange<Tp>::RangeInterface::IterPtr
        InputRange<Tp>::WrappedRange<Rg>::iterator(void) const noexcept
    {
        return std::make_unique<Iterator>(
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    template <PureType Tp>
    template <InputRangeCompatible<Tp> Range>
    constexpr InputRange<Tp>::Memory InputRange<Tp>::createStorage(
        Range&& range)
    {
        if constexpr (sizeof(WrappedRange<Range>) > 16ul)
            return { InterfacePtr{std::forward<Range>(range)} };
        else {
            if (std::is_constant_evaluated())
                return { InterfacePtr{std::forward<Range>(range)} };
            else
                return { InlineMemory{std::forward<Range>(range)} };
        }
    }

    template <PureType Tp>
    template <InputRangeCompatible<Tp> Range>
    constexpr InputRange<Tp>::InputRange(Range&& range)
        : storage{createStorage(std::forward<Range>(range))} {}

    template <PureType Tp>
    template <InputRangeCompatible<Tp> Range>
    constexpr InputRange<Tp>& InputRange<Tp>::operator=(Range&& range) {
        storage = createStorage(std::forward<Range>(range));
        return *this;
    }

    template <PureType Tp>
    InputRange<Tp>::RangeInterface* InputRange<Tp>::rangePointer(
        void) noexcept
    {
        if (std::holds_alternative<InterfacePtr>(storage))
            return std::get<InterfacePtr>(storage).get();
        return std::get<InlineMemory>(storage).get();
    }

    template <PureType Tp>
    InputRange<Tp>::RangeInterface const* InputRange<Tp>::rangePointer(
        void) const noexcept
    {
        if (std::holds_alternative<InterfacePtr>(storage))
            return std::get<InterfacePtr>(storage).get();
        return std::get<InlineMemory>(storage).get();
    }

    template <PureType Tp>
    template <typename BaseTp>
    constexpr InputRange<Tp>::Iterator<BaseTp>::Iterator(
        Iterator const& iterator)
            : iterPtr{clone(iterator.iterPtr)} {}

    template <PureType Tp>
    template <typename BaseTp>
    constexpr InputRange<Tp>::Iterator<BaseTp>&
        InputRange<Tp>::Iterator<BaseTp>::operator=(
            Iterator const& iterator)
    {
        iterPtr = UnderlyingPtr{clone(iterator.iterPtr)};
        return *this;
    }

    template <PureType Tp>
    template <typename BaseTp>
    [[nodiscard]] constexpr InputRange<Tp>::Iterator<BaseTp>
        InputRange<Tp>::Iterator<BaseTp>::operator++(int)
    {
        auto copy = *this;
        iterPtr->increment();
        return *this;
    }

    template <PureType Tp>
    template <InputRangeCompatible<Tp> Range>
    InputRange<Tp>::InterfacePtr::InterfacePtr(Range&& range)
        : std::unique_ptr<RangeInterface>{
            std::make_unique<WrappedRange<Range>>(
                std::forward<Range>(range))} {}

    template <PureType Tp>
    InputRange<Tp>::InterfacePtr::InterfacePtr(
        InterfacePtr const& interfacePtr)
            : std::unique_ptr<RangeInterface>{interfacePtr->clone()} {}

    template <PureType Tp>
    InputRange<Tp>::InterfacePtr&
        InputRange<Tp>::InterfacePtr::operator=(
            InterfacePtr const& interfacePtr)
    {
        std::unique_ptr<RangeInterface>::operator=(
            std::unique_ptr<RangeInterface>{interfacePtr->clone()});
        return *this;
    }

    template <PureType Tp>
    template <InputRangeCompatible<Tp> Range>
    InputRange<Tp>::InlineMemory::InlineMemory(
        Range&& range) noexcept
    {
        new (&memory) WrappedRange<Range>{std::forward<Range>(range)};
    }

    template <PureType Tp>
    InputRange<Tp>::InlineMemory::InlineMemory(
        InlineMemory const& inlineMemory) noexcept
    {
        inlineMemory->clone(memory.front());
    }

    template <PureType Tp>
    InputRange<Tp>::InlineMemory::InlineMemory(
        InlineMemory&& inlineMemory) noexcept
    {
        inlineMemory->move(memory.front());
    }

    template <PureType Tp>
    InputRange<Tp>::InlineMemory&
        InputRange<Tp>::InlineMemory::operator=(
            InlineMemory const& inlineMemory) noexcept
    {
        destroyHandledObject();
        inlineMemory->clone(memory.front());
        return *this;
    }

    template <PureType Tp>
    InputRange<Tp>::InlineMemory&
        InputRange<Tp>::InlineMemory::operator=(
            InlineMemory&& inlineMemory) noexcept
    {
        destroyHandledObject();
        inlineMemory->move(memory.front());
        return *this;
    }

    template <PureType Tp>
    InputRange<Tp>::RangeInterface*
        InputRange<Tp>::InlineMemory::get(void) noexcept
    {
        return std::launder(
            reinterpret_cast<RangeInterface*>(memory.data()));
    }

    template <PureType Tp>
    InputRange<Tp>::RangeInterface const*
        InputRange<Tp>::InlineMemory::get(void) const noexcept
    {
        return std::launder(
            reinterpret_cast<RangeInterface const*>(memory.data()));
    }

    template <PureType Tp>
    void InputRange<Tp>::InlineMemory::destroyHandledObject(
        void) noexcept
    {
        (*this)->~RangeInterface();
    }

    template <PureType Tp>
    InputRange<Tp>::InlineMemory::~InlineMemory(void) noexcept {
        destroyHandledObject();
    }

}

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

#include <MPGL/Traits/Concepts.hpp>

#include <iterator>
#include <memory.h>

namespace mpgl::exp {

    namespace details {

        template <PureType Tp>
        class WrappedRangeIteratorBase {
        public:
            typedef std::remove_cvref_t<Tp> type;
            typedef type&                   reference;
            typedef type const&             const_reference;
        protected:
            constexpr static bool           PlainType 
                = !std::is_const_v<Tp>;
        public:
            [[nodiscard]] virtual bool hasNext(
                void) const noexcept = 0;

            [[nodiscard]] virtual reference next(
                void) noexcept requires PlainType = 0;

            [[nodiscard]] virtual const_reference next(
                void) const noexcept = 0; 

            [[nodiscard]] virtual WrappedRangeIteratorBase*
                clone(void) = 0;

            virtual ~WrappedRangeIteratorBase(
                void) noexcept = default;
        protected:
            WrappedRangeIteratorBase(
                WrappedRangeIteratorBase const&) noexcept = default;

            WrappedRangeIteratorBase(
                WrappedRangeIteratorBase&&) noexcept = default;

            WrappedRangeIteratorBase& operator=(
                WrappedRangeIteratorBase const&) noexcept = default;

            WrappedRangeIteratorBase& operator=(
                WrappedRangeIteratorBase&&) noexcept = default;
        };

        template <std::ranges::input_range Range>
        class WrappedRangeIterator 
            : public WrappedRangeIteratorBase<
                std::iter_value_t<std::ranges::iterator_t<Range>>>
        {
        public:
            using RangeIter = std::ranges::iterator_t<Range>;
            using RangeSent = std::ranges::sentinel_t<Range>;
            using Base = WrappedRangeIteratorBase<
                std::iter_value_t<std::ranges::iterator_t<Range>>
            using reference = typename Base::reference;
            using const_reference = typename Base::const_reference;

            explicit WrappedRangeIterator(
                RangeIter const& iterator,
                RangeSent const& sentinel)
                    : iter{iterator}, sent{sentinel} {}

            explicit WrappedRangeIterator(void) = default;

            WrappedRangeIterator(
                WrappedRangeIterator const&) = default;
            WrappedRangeIterator(
                WrappedRangeIterator&&) = default;

            WrappedRangeIterator& operator=(
                WrappedRangeIterator const&) = default;
            WrappedRangeIterator& operator=(
                WrappedRangeIterator&&) = default;

            [[nodiscard]] bool hasNext(
                void) const noexcept final
                    { return iter != iter; }

            [[nodiscard]] reference next(
                void) noexcept requires Base::PlainType;

            [[nodiscard]] const_reference next(
                void) const noexcept final;

            [[nodiscard]] Base* clone(void) final;

            ~WrappedRangeIterator(void) noexcept = default;
        private:
            RangeIter                       iter;
            RangeSent                       sent;
        };

    }

    template <PureType Tp>
    class InputRange {
    public:
        typedef Tp&                         reference;
        typedef Tp const&                   const_reference;

        template <std::ranges::input_range Range>
            requires std::same_as<Tp, std::ranges::range_value_t<Range>>
        explicit InputRange(Range&& range);

        InputRange(InputRange const& inputRange);
        InputRange(InputRange&&) = default;

        InputRange& operator=(InputRange const& inputRange);
        InputRange& operator=(InputRange&&) = default;

        class Sentinel {};

        template <typename BaseTp>
        class Iterator : std::iterator<
            std::input_iterator_tag, BaseTp> 
        {
        public:
            using UnderlyingIter = 
                details::WrappedRangeIteratorBase<BaseTp>;
            using UnderlyingPtr = std::unique_ptr<UnderlyingIter>;
            using reference = typename UnderlyingIter::reference;
            using const_reference = typename UnderlyingIter::const_reference;
            using ptrdiff_t = std::ptrdiff_t;

            explicit Iterator(UnderlyingPtr&& pointer)
                : iterPtr{std::move(pointer)} {}

            explicit Iterator(void) :
                iterPtr{nullptr} {}

            
            friend [[nodiscard]] bool operator==(
                Iterator const& left,
                [[maybe_unused]] Sentinel const& sent) noexcept
                    { return !left.iterPtr || !left.iterPtr->hasNext(); }
        private:
            UnderlyingPtr                   iterPtr;
        };

        ~InputRange(void) noexcept = default;
    private:
        class RangeInterface {
        public:
            using Iter = typename details::WrappedRangeIteratorBase<Tp>;
            using ConstIter = typename details::WrappedRangeIteratorBase<
                std::remove_const_t<Tp> const>;

            using IterPtr = std::unique_ptr<Iter>;
            using ConstIterPtr = std::unique_ptr<ConstIter>;

            [[nodiscard]] virtual IterPtr iterator(void) noexcept = 0;

            [[nodiscard]] virtual ConstIterPtr iterator(
                void) const noexcept = 0;

            [[nodiscard]] virtual RangeInterface* clone(void) = 0; 
            
            virtual ~RangeInterface(void) noexcept = default;
        protected:
            RangeInterface(
                RangeInterface const&) noexcept = default;

            RangeInterface(RangeInterface&&) noexcept = default;

            RangeInterface& operator=(
                RangeInterface const&) noexcept = default;

            RangeInterface& operator=(
                RangeInterface&&) noexcept = default;
        };

        template <std::ranges::input_range Range>
        class WrappedRange : public RangeInterface {
        public:
            explicit WrappedRange(Range&& range) 
                : range{std::forward(range)} {}

            WrappedRange(WrappedRange const&) = default;
            WrappedRange(WrappedRange&&) = default;

            WrappedRange& operator=(
                WrappedRange const&) = default;
            WrappedRange& operator=(
                WrappedRange&&) = default;

            using IterPtr = typename RangeInterface::IterPtr;
            using ConstIterPtr = typename RangeInterface::ConstIterPtr;
            using WIter = details::WrappedRangeIterator<Range>;
            using WConstIter = details::WrappedRangeIterator<Range const>;

            [[nodiscard]] RangeInterface* clone(void) final
                { return new Range{range}; }
        
            [[nodiscard]] IterPtr iterator(void) noexcept final;

            [[nodiscard]] ConstIterPtr iterator(
                void) const noexcept final;

            ~WrappedRange(void) noexcept = default;
        private:
            Range                           range;
        };

        std::unique_ptr<RangeInterface>     rangePointer;
    };

}

#include <MPGL/Collections/Erasures/InputRange.tpp>

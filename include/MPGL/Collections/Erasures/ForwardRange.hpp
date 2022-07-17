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
            [[nodiscard]] virtual bool hasNext(void) const noexcept = 0;

            [[nodiscard]] virtual reference next(
                void) noexcept requires PlainType = 0;

            [[nodiscard]] virtual const_reference const& next(
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

    }

    template <PureType Tp>
    class ForwardRange {
    public:
        typedef Tp&                         reference;
        typedef Tp const&                   const_reference;

    private:
        class RangeInterface {
        public:
            [[nodiscard]] virtual std::size_t size(
                void) const noexcept = 0;

            [[nodiscard]] virtual bool empty(
                void) const noexcept = 0;

            using Iter = typename WrappedRangeIteratorBase<Tp>;
            using ConstIter = typename WrappedRangeIteratorBase<
                std::remove_const_t<Tp> const>;

            using IterPtr = std::unique_ptr<Iter>;
            using ConstIterPtr = std::unique_ptr<ConstIter>;

            [[nodiscard]] virtual IterPtr iterator(void) noexcept = 0;

            [[nodiscard]] virtual ConstIterPtr iterator(
                void) const noexcept = 0;

            [[nodiscard]] virtual RangeInterface* clone(void) = 0; 
            
            virtual ~RangeInterface(void) noexcept = default;
        protected:

        };

    };

}

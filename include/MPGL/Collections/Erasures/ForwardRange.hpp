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

namespace mpgl::exp {

    template <PureType Tp>
    class ForwardRange {
    public:
        typedef Tp&                         reference;
        typedef Tp const&                   const_reference;

    private:
        class WrappedRangeIteratorBase {
        public:
            [[nodiscard]] virtual bool hasNext(void) const noexcept = 0;

            [[nodiscard]] virtual reference next(void) noexcept = 0;

            [[nodiscard]] virtual const_reference const& next(
                void) const noexcept = 0; 

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

    };

}

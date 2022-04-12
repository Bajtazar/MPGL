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

namespace mpgl {

    template <Adaptable Tp>
    Adapter<Tp>::Adapter(Tp const& range) noexcept
        : range{range / static_cast<Tp>(context.windowDimensions)
            * value_type{2} - value_type{1}} {}

    template <Adaptable Tp>
    Adapter<Tp>::Adapter(Tp&& range) noexcept
        : range{std::move(range) / static_cast<Tp>(context.windowDimensions)
            * value_type{2} - value_type{1}} {}

    template <Adaptable Tp>
    void Adapter<Tp>::operator= (Tp const& factor) noexcept {
        range = factor
            / static_cast<Tp>(context.windowDimensions)
                * value_type{2} - value_type{1};
    }

    template <Adaptable Tp>
    void Adapter<Tp>::operator= (Tp&& factor) noexcept {
        range = std::move(factor)
            / static_cast<Tp>(context.windowDimensions)
                * value_type{2} - value_type{1};
    }

    template <Adaptable Tp>
    Adapter<Tp>::operator Tp() const noexcept {
        return (range + value_type{1})
            * static_cast<Tp>(context.windowDimensions) / value_type{2};
    }

}

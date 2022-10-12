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
#include <MPGL/Utility/Adapter.hpp>

namespace mpgl {

    Adapter<2>::value_type Adapter<2>::cast(
        const_reference value) noexcept
    {
        return value / vectorCast<float32>(context.windowDimensions) * 2.f - 1.f;
    }

    Adapter<2>::Adapter(const_reference value) noexcept
        : value{cast(value)} {}

    Adapter<2>::Adapter(value_type&& value) noexcept
        : value{cast(value)} {}

    Adapter<2>& Adapter<2>::operator= (
        const_reference factor) noexcept
    {
        value = cast(factor);
        return *this;
    }

    Adapter<2>& Adapter<2>::operator= (value_type&& factor) noexcept {
        value = cast(factor);
        return *this;
    }

    [[nodiscard]] Adapter<2>::operator value_type() const noexcept {
        return (value + 1.f) * vectorCast<float32>(
            context.windowDimensions) / 2.f;
    }

}

/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

template <Arithmetic Tp>
template <Arithmetic Up>
[[nodiscard]] constexpr Up Quaternion<Tp>::length(void) const noexcept {
    return _M_data.length();
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::normalize(void) noexcept {
    _M_data.normalize();
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::conjugate(void) noexcept {
    for (uint8 i = 1;i < 4; ++i)
        _M_data[i] = -_M_data[i];
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::invert(void) const noexcept {
    auto const len = length();
    return conjugate() /= (len * len);
}

}

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

    [[nodiscard]] constexpr inline uint8 operator& (
        VectorizedGlyph::SimpleFlags const& left,
        uint8 right) noexcept
    {
        return static_cast<uint8>(left) & right;
    }

    [[nodiscard]] constexpr inline uint8 operator& (
        uint8 left,
        VectorizedGlyph::SimpleFlags const& right) noexcept
    {
        return left & static_cast<uint8>(right);
    }

    [[nodiscard]] constexpr inline uint16 operator& (
        VectorizedGlyph::CompositeFlags const& left,
        uint16 right) noexcept
    {
        return static_cast<uint16>(left) & right;
    }

    [[nodiscard]] constexpr inline uint16 operator& (
        uint16 left,
        VectorizedGlyph::CompositeFlags const& right) noexcept
    {
        return left & static_cast<uint16>(right);
    }

}

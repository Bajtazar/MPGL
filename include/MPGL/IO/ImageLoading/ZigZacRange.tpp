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

    template <uint8 Size>
        requires (Size > 1)
    template <std::ranges::random_access_range Range>
    ZigZacRange<Size>::RangeMatrix<Range>
        ZigZacRange<Size>::fromZigZac(Range const& range) noexcept
    {
        RangeMatrix<Range> matrix;
        for (uint8 i = 0; i != Size; ++i)
            for (uint8 j = 0; j != Size; ++j)
                matrix[i][j] = range[zigzac[i][j]];
        return matrix;
    }

    template <uint8 Size>
        requires (Size > 1)
    template <FlexibleRange Range>
    Range ZigZacRange<Size>::toZigZac(
        RangeMatrix<Range> const& matrix) noexcept
    {
        Range range;
        range.resize(Size * Size);
        for (uint8 i = 0; i != Size; ++i)
            for (uint8 j = 0; j != Size; ++j)
                range[zigzac[i][j]] = matrix[i][j];
        return range;
    }

    template <uint8 Size>
        requires (Size > 1)
    template <FixedSizeRange Range>
        requires (Range::size() >= Size * Size)
    Range& ZigZacRange<Size>::toZigZac(
        RangeMatrix<Range> const& matrix,
        Range& range) noexcept
    {
        for (uint8 i = 0; i != Size; ++i)
            for (uint8 j = 0; j != Size; ++j)
                range[zigzac[i][j]] = matrix[i][j];
        return range;
    }

    template <uint8 Size>
        requires (Size > 1)
    constexpr void ZigZacRange<Size>::nextPosition(
        std::size_t& first,
        std::size_t& second,
        bool& direction) noexcept
    {
        if (first == Size - 1) {
            ++second;
            direction = !direction;
        } else if (!second) {
            ++first;
            direction = !direction;
        } else {
            ++first;
            --second;
        }
    }

    template <uint8 Size>
        requires (Size > 1)
    consteval ZigZacRange<Size>::ZigZacBase
        ZigZacRange<Size>::generateZigZacArray(void) noexcept
    {
        ZigZacBase matrix;
        bool direction = true;
        for (std::size_t iter = 0, x = 0, y = 0;
            iter != Size * Size; ++iter)
        {
            matrix[y][x] = iter;
            if (direction)
                nextPosition(x, y, direction);
            else
                nextPosition(y, x, direction);
        }
        return matrix;
    }

}

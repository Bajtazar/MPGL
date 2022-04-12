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

#include <MPGL/Mathematics/Matrix.hpp>

#include <ranges>
#include <vector>
#include <array>

namespace mpgl {

    /**
     * Transfroms the given square matrix into vector using zig zac
     * and vice versa
     *
     * @tparam Size the size of the matrix row
     */
    template <uint8 Size>
        requires (Size > 1)
    class ZigZacRange {
    public:
        ZigZacRange(void) noexcept = delete;

        template <class Range>
        using RangeMatrix = Matrix<std::ranges::range_value_t<Range>,
            Size, Size>;

        /**
         * Convers the given range into the matrix using zig zac
         * method
         *
         * @tparam Range the range type
         * @param range the constant reference to the range object
         * @return the output matrix
         */
        template <std::ranges::random_access_range Range>
        [[nodiscard]] static RangeMatrix<Range> fromZigZac(
            Range const& range) noexcept;

        /**
         * Converts the given matrix into the range using zig zac
         * method
         *
         * @tparam Range the range type
         * @param matrix the constant reference to the matrix object
         * @return the output range
         */
        template <FlexibleRange Range>
        [[nodiscard]] static Range toZigZac(
            RangeMatrix<Range> const& matrix) noexcept;

        /**
         * Converts the given matrix into the range using zig zac
         * method
         *
         * @tparam Range the range type
         * @param matrix the constant reference to the matrix object
         * @param range the reference to the range object
         * @return the output range
         */
        template <FixedSizeRange Range>
            requires (Range::size() >= Size * Size)
        static Range& toZigZac(
            RangeMatrix<Range> const& matrix,
            Range& range) noexcept;
    private:
        typedef std::array<std::array<
            std::size_t, Size>, Size>                   ZigZacBase;

        /**
         * Sets the next position of the zig zac
         *
         * @param first the first dimension
         * @param second the second dimension
         * @param direction the direction of the zig zac
         */
        constexpr static void nextPosition(
            std::size_t& first,
            std::size_t& second,
            bool& direction) noexcept;

        /**
         * Generates the lookup table used in zig zac convertions
         *
         * @return the lookup table used in zig zac convertions
         */
        consteval static ZigZacBase generateZigZacArray(void) noexcept;

        constexpr static ZigZacBase                     zigzac
            = generateZigZacArray();
    };

}

#include <MPGL/Utility/ZigZacRange.tpp>

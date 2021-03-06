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

    template <std::signed_integral Tp>
    [[nodiscard]] inline constexpr Tp signum(
        Tp const& value) noexcept
    {
        return value < Tp{0} ? Tp{-1} : !value ? Tp{0} : Tp{1};
    }

    template <std::totally_ordered Tp>
    [[nodiscard]] inline constexpr bool between(
        Tp const& first,
        Tp const& second,
        Tp const& checked) noexcept
    {
        if (first < second)
            return (first <= checked) && (checked <= second);
        return (second <= checked) && (checked <= first);
    }

    template <std::totally_ordered Tp, std::size_t Size>
    [[nodiscard]] inline constexpr bool between(
        Vector<Tp, Size> const& first,
        Vector<Tp, Size> const& second,
        Vector<Tp, Size> const& checked) noexcept
    {
        for (std::size_t i = 0; i < Size; ++i)
            if (!between(first[i], second[i], checked[i]))
                return false;
        return true;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr Vector2<Tp> cartesianToPolar(
        Vector2<Tp> const& vector) noexcept
    {
        return { norm(vector), std::atan2(vector[1], vector[0]) };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr Vector2<Tp> polarToCartesian(
        Vector2<Tp> const& vector) noexcept
    {
        return { vector[0] * std::cos(vector[1]),
            vector[0] * std::sin(vector[1]) };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr Matrix2<Tp> rotationMatrix(
        float32 angle) noexcept
    {
        return {Vector2<Tp>{std::cos(angle), -std::sin(angle)},
            Vector2<Tp>{std::sin(angle), std::cos(angle)}};
    }

    template <std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<std::complex<float64>, Cols, Rows>
        hermitianTranspose(
            Matrix<std::complex<float64>, Rows, Cols> const& matrix
            ) noexcept
    {
        Matrix<std::complex<float64>, Cols, Rows> output;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < Cols; ++j)
                output[j][i] = std::conj(matrix[i][j]);
        return output;
    }


}

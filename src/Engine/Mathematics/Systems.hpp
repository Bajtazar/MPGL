#pragma once

#include "Ranges.hpp"
#include "Matrix.hpp"

#include <complex>

namespace mpgl {

    template <std::signed_integral Tp>
    inline constexpr Tp signum(Tp const& value) noexcept {
        return value < Tp{0} ? Tp{-1} : !value ? Tp{0} : Tp{1};
    }

    template <std::totally_ordered Tp>
    inline constexpr bool between(
        Tp const& first,
        Tp const& second,
        Tp const& checked) noexcept
    {
        if (first < second)
            return (first <= checked) && (checked <= second);
        return (second <= checked) && (checked <= first);
    }

    template <Mathematical Tp>
    inline constexpr Vector2<Tp> cartesianToPolar(
        Vector2<Tp> const& vector) noexcept
    {
        return { norm(vector), std::atan2(vector[1], vector[0]) };
    }

    template <Mathematical T>
    inline constexpr Vector2<T> polarToCartesian(
        Vector2<T> const& vector) noexcept
    {
        return { vector[0] * std::cos(vector[1]),
            vector[0] * std::sin(vector[1]) };
    }

    template <Mathematical Tp>
    inline constexpr Matrix2<Tp> rotationMatrix(float32 angle) noexcept {
        return {Vector2<Tp>{std::cos(angle), -std::sin(angle)},
            Vector2<Tp>{std::sin(angle), std::cos(angle)}};
    }

    template <std::size_t Rows, std::size_t Cols>
    inline constexpr Matrix<std::complex<float64>, Cols, Rows>
        hermitianTranspose(
            Matrix<std::complex<float64>, Rows, Cols> const& matrix) noexcept
    {
        Matrix<std::complex<float64>, Cols, Rows> output;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < Cols; ++j)
                output[j][i] = std::conj(matrix[i][j]);
        return output;
    }

}

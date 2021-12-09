#pragma once

#include "Ranges.hpp"
#include "Matrix.hpp"

#include <complex>

namespace ge {

    template <std::signed_integral Tp>
    inline constexpr Tp signum(Tp const& value) noexcept {
        return value < Tp{0} ? Tp{-1} : !value ? Tp{0} : Tp{1};
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
    inline constexpr Matrix2<Tp> rotationMatrix(float angle) noexcept {
        return {Vector2<Tp>{std::cos(angle), -std::sin(angle)},
            Vector2<Tp>{std::sin(angle), std::cos(angle)}};
    }

    template <std::size_t Rows, std::size_t Cols>
    inline constexpr Matrix<std::complex<double>, Cols, Rows>
        hermitianTranspose(
            Matrix<std::complex<double>, Rows, Cols> const& matrix) noexcept
    {
        Matrix<std::complex<double>, Cols, Rows> output;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < Cols; ++j)
                output[j][i] = std::conj(matrix[i][j]);
        return output;
    }

}

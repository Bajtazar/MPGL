#pragma once

#include "Ranges.hpp"
#include "Matrix.hpp"

namespace ge {

    template <Arithmetic T>
        requires std::is_signed_v<T>
    inline constexpr T signum(const T& value) noexcept {
        return value < 0 ? static_cast<T>(-1) : !value ? static_cast<T>(0) : static_cast<T>(1);
    }

    template <Mathematical T>
    inline constexpr TwoVector<T> cartesianToPolar(const TwoVector<T>& vector) noexcept {
        return { norm(vector), std::atan2(vector[1], vector[0]) };
    }

    template <Mathematical T>
    inline constexpr TwoVector<T> polarToCartesian(const TwoVector<T>& vector) noexcept {
        return { vector[0] * std::cos(vector[1]), vector[0] * std::sin(vector[1]) };
    }

    template <Mathematical T>
    inline constexpr Matrix<T, 2> rotationMatrix(float angle) noexcept {
        return {Vector2f{std::cos(angle), -std::sin(angle)}, Vector2f{std::sin(angle), std::cos(angle)}};
    }

}

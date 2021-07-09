#pragma once

#include "../Core/Figures/Views.hpp"
#include "Matrix.hpp"

#include <math.h>

namespace ge {

    template <Arithmetic T>
        requires std::is_signed_v<T>
    inline constexpr T signum(const T& value) noexcept {
        return value < 0 ? static_cast<T>(-1) : !value ? static_cast<T>(0) : static_cast<T>(1);
    }

    template <typename T>
    concept Mathematical = Arithmetic<T> && std::convertible_to<T, double>;

    struct normFn {

        struct powerInvocable {

            template <Mathematical T, Mathematical U>
            constexpr T operator() (const T& value, const U& power) const noexcept {
                return std::pow(value, power);
            }

        };

        template <std::weakly_incrementable Iter, std::sentinel_for<Iter> Sent, std::invocable<iterator_value_type_t<Iter>, double> Pred = powerInvocable>
            requires requires (iterator_value_type_t<Iter> a) { a += a; }
        constexpr iterator_value_type_t<Iter> operator() (Iter first, Sent last, double norm = 2., Pred pred = {}) const noexcept {
            iterator_value_type_t<Iter> sum {};
            for (;first != last; ++first)
                sum += std::move(std::invoke(pred, std::move(*first), norm));
            return std::invoke(pred, sum, 1. / norm);
        }

        template <std::ranges::input_range Range, std::invocable<iterator_value_type_t<std::ranges::iterator_t<Range>>, double> Pred = powerInvocable>
            requires requires (iterator_value_type_t<std::ranges::iterator_t<Range>> a) { a += a; }
        constexpr iterator_value_type_t<std::ranges::iterator_t<Range>> operator() (Range&& range, double norm = 2., Pred pred = {}) const noexcept {
            return (*this)(std::ranges::begin(range), std::ranges::end(range), norm, std::move(pred));
        }

    };

    inline constexpr normFn norm{};

    template <Mathematical T>
    inline constexpr Vector<T, T> cartesianToPolar(const Vector<T, T>& vector) noexcept {
        return { norm(vector), std::atan2(vector[1], vector[0]) };
    }

    template <Mathematical T>
    inline constexpr Vector<T, T> polarToCartesian(const Vector<T, T>& vector) noexcept {
        return { vector[0] * std::cos(vector[1]), vector[0] * std::sin(vector[1]) };
    }

    template <Mathematical T>
    inline constexpr Matrix<T, 2> rotationMatrix(float angle) noexcept {
        return {Vector2f{std::cos(angle), -std::sin(angle)}, Vector2f{std::sin(angle), std::cos(angle)}};
    }

}

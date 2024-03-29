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

#include <MPGL/Mathematics/Ranges.hpp>

namespace mpgl {

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <AllAbsolutelySame<typename Matrix<Tp, Rows, Cols>::
        value_type>... Rws>
            requires (sizeof...(Rws) == Rows)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(
        Rws&&... rows) noexcept
            : base{ static_cast<value_type>(
                std::forward<Rws>(rows))... }
    {}

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <AllAbsolutelySame<typename Matrix<Tp, Rows, Cols>::
        transposed_value_type>... Cls>
            requires (sizeof...(Cls) == Cols)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(
        [[maybe_unused]] TransposedTag tag,
        Cls&&... columns) noexcept
    {
        auto placer = [this](auto&& column, std::size_t const columnId) {
            std::size_t rowId = 0;
            for (auto&& element : column)
                base[rowId++][columnId] = element;
        };
        std::size_t columnId = 0;
        (placer(std::forward<Cls>(columns), columnId++), ...);
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(
        std::span<value_type const, Rows> const& span) noexcept
    {
        std::ranges::copy(span, begin());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        std::span<value_type const, Rows>() const noexcept
    {
        return std::span<value_type const, Rows>(base);
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        Matrix<Tp, Rows, Cols>::operator-(void) const noexcept
    {
        Matrix<Tp, Cols, Rows> result;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                result[i][j] = -(*this)[i][j];
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <Arithmetic Up>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Up, Rows, Cols>() const noexcept
    {
        Matrix<Up, Rows, Cols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <std::size_t URows, std::size_t UCols>
        requires (URows >= Rows && UCols >= Cols &&
            (URows != Rows || UCols != Cols))
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Tp, URows, UCols>() const noexcept
    {
        Matrix<Tp, URows, UCols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator+=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left + right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator-=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left - right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator*=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left * right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator/=(
             Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left / right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator%=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, %)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left % right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator^=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left ^ right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator&=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, &)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left & right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator|=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, |)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left | right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator+=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator-=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator*=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator/=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator%=(
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator^=(
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator&=(
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator|=(
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value |= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator+(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result += right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator-(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result -= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Vector<Tp, Rows> operator*(
        Matrix<Tp, Rows, Cols> const& matrix,
        Vector<Tp, Cols> const& vector)
    {
        Vector<Tp, Cols> result;
        for (std::size_t i = 0;i < Cols; ++i)
            result[i] = innerProduct(matrix[i], vector, Tp{0});
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result += right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result -= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result *= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result /= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result %= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result ^= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator&(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result &= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result |= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result += left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result -= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result *= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result /= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, %)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result %= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result ^= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
         operator&(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, &)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result &= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, |)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result |= left;
        return result;
    }

}

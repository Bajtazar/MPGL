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

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
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
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator=(
            Vector<Tp, Rows> const& vec) noexcept
    {
        std::ranges::copy(vec, begin());
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::Column::operator
        Vector<Tp, Rows>() const noexcept
    {
        Vector<Tp, Rows> vector;
        std::ranges::copy(*this, vector.begin());
        return vector;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <AllAbsolutelySame<typename
        Matrix<Tp, Rows, Cols>::value_type>... Args>
            requires (sizeof...(Args) == Rows)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(Args&&... args) noexcept
        : normalBase{tupleBuilder(std::forward<Args>(args)...)}
    {
        if constexpr (!std::is_constant_evaluated())
            reverse(begin(), end());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <typename... Args>
    constexpr Matrix<Tp, Rows, Cols>::NormalBase
        Matrix<Tp, Rows, Cols>::tupleBuilder(Args&&... args) const noexcept
    {
        if constexpr (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type&
        Matrix<Tp, Rows, Cols>::first(void)  noexcept
    {
        return static_cast<value_type&>(
            std::get<Rows - 1>(
                static_cast<NormalBase&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type const&
        Matrix<Tp, Rows, Cols>::first(void) const noexcept
    {
        return static_cast<value_type const&>(
            std::get<Rows - 1>(
                static_cast<NormalBase const&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type&
        Matrix<Tp, Rows, Cols>::firstCol(void)  noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type const&
        Matrix<Tp, Rows, Cols>::firstCol(void) const noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase const&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic Up>
    [[nodiscard]] constexpr Up Matrix<Tp, Rows, Cols>::Column::length(
        Up init) const noexcept
    {
        return accumulate(*this, std::move(init),
            [](auto const& value){ return value * value; });
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator+=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left + right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator-=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left - right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator*=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left * right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator/=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left / right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator%=(
            Column const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left % right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator^=(
            Column const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left ^ right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
         Matrix<Tp, Rows, Cols>::Column::operator&=(
            Column const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left & right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator|=(
            Column const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left | right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator+=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator-=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator*=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator/=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator%=(
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator^=(
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator&=(
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator|=(
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value |= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp
                { return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left / right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                 return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left / right; });
        return result;
    }


    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic Up>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Up, Rows, Cols>() const noexcept
    {
        Matrix<Up, Rows, Cols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
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
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator+=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator-=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator*=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator/=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator%=(
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator^=(
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator&=(
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator|=(
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value |= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t LRows,
        std::size_t LCols, std::size_t RCols>
            requires (LRows > 1 && LCols > 1 && RCols > 1)
    [[nodiscard]] Matrix<Tp, LRows, RCols> operator*(
        Matrix<Tp, LRows, LCols> const& left,
        Matrix<Tp, LCols, RCols> const& right)
    {
        Matrix<Tp, LRows, RCols> result;
        for (std::size_t i = 0; i < LRows; ++i)
            for (std::size_t j = 0; j < RCols; ++j)
                result[i][j] = innerProduct(left[i],
                    right.getColumn(j), Tp{0});
        return result;
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

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator=(Matrix&& matrix) noexcept
    {
        normalBase = std::move(matrix.normalBase);
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Vector<Tp, Rows>
        Matrix<Tp, Rows, Cols>::Column::operator-(
            void) const noexcept
    {
        return -static_cast<Vector<Tp, Rows>>(*this);
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr auto
        Matrix<Tp, Rows, Cols>::columnsRange(void) noexcept
    {
        return std::ranges::subrange(columnBegin(), columnEnd());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr auto
        Matrix<Tp, Rows, Cols>::columnsRange(void) const noexcept
    {
        return std::ranges::subrange(columnBegin(), columnEnd());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows == Cols && Rows > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols> identityMatrix(
        Tp diagonal) noexcept
    {
        Matrix<Tp, Rows, Cols> identity;
        for (std::size_t i = 0; i < Rows; ++i)
            identity[i][i] = diagonal;
        return identity;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Matrix<Tp, Cols, Rows>
        transpose(Matrix<Tp, Rows, Cols> const& matrix) noexcept
    {
        Matrix<Tp, Cols, Rows> transposed;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                transposed[j][i] = matrix[i][j];
        return transposed;
    }

   template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Vector<Tp, Rows>>
        LUPDecompositionFn::operator() (
            Matrix<Tp, Rows, Rows>& matrix) const noexcept
    {
        Vector<Tp, Rows> perms;
        std::iota(perms.begin(), perms.end(), Tp{0});
        for (std::size_t k = 0, u; k < Rows; ++k) {
            if (!findPivot(u, k, matrix))
                return {};
            if (u != k) {
                std::swap(perms[k], perms[u]);
                std::swap(matrix[k], matrix[u]);
            }
            permute(k, matrix);
        }
        return { perms };
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<bool>
        LUPDecompositionFn::operator() (
            Matrix<Tp, Rows, Rows>& matrix, CTag) const noexcept
    {
        std::size_t counter = 0;
        for (std::size_t k = 0, u = 0; k < Rows; ++k, u = k) {
            if (!findPivot(u, k, matrix))
                return {};
            if (u != k) {
                ++counter;
                std::swap(matrix[k], matrix[u]);
            }
            permute(k, matrix);
        }
        return { counter % 2 };
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    constexpr bool LUPDecompositionFn::findPivot(
        std::size_t& u,
        std::size_t& k,
        Matrix<Tp, Rows, Rows> const& matrix) const noexcept
    {
        Tp p{0};
        for (std::size_t i = k; i < Rows; ++i) {
            if (Tp v = std::abs(matrix[i][k]); v > p) {
                p = v;
                u = i;
            }
        }
        return p != 0;
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    constexpr void LUPDecompositionFn::permute(
        std::size_t& k,
        Matrix<Tp, Rows, Rows>& matrix) const noexcept
    {
        for (std::size_t i = k + 1; i < Rows; ++i) {
            matrix[i][k] /= matrix[k][k];
            for (std::size_t j = k + 1; j < Rows; ++j)
                 matrix[i][j] -= matrix[i][k] * matrix[k][j];
        }
    }

    template <Arithmetic Tp, std::size_t Rows,
        SizedRange<Tp, Rows> Permutations,
        SizedRange<Tp, Rows> Results>
        requires (Rows > 1)
    [[nodiscard]] constexpr Vector<Tp, Rows>
        lupSolve(
            Matrix<Tp, Rows, Rows> const& luMatrix,
            Permutations const& permutations,
            Results const& results) noexcept
    {
        Vector<Tp, Rows> x, y;
        for (std::size_t i = 0; i < Rows; ++i) {
            Tp sum = std::inner_product(luMatrix[i].begin(),
                luMatrix[i].begin() + i, y.begin(), Tp{0});
            y[i] = results[permutations[i]] - sum;
        }
        for (std::size_t i = Rows - 1; i < Rows; --i) {
            Tp sum = std::inner_product(luMatrix[i].begin() + i + 1,
                luMatrix[i].end(), x.begin() + i + 1, Tp{0});
            x[i] = (y[i] - sum) / luMatrix[i][i];
        }
        return x;
    }

    template <Arithmetic Tp, std::size_t Rows,
        Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Matrix<Up, Rows, Rows>>
        invert(Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto permutations = lupDecomposition(luMatrix)) {
            auto inverseMatrix = identityMatrix<Up, Rows, Rows>();
            for (auto& column : inverseMatrix.columnsRange())
                column = lupSolve(luMatrix, *permutations, column);
            return { inverseMatrix };
        }
        return {};
    }

    template <Arithmetic Tp, std::size_t Rows, Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr Up det(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto parity = lupDecomposition(luMatrix,
            LUPDecompositionFn::CounterTag))
        {
            Up det{luMatrix[0][0]};
            for (std::size_t i = 1; i < Rows; ++i)
                det *= luMatrix[i][i];
            return *parity ? -det : det;
        }
        return Up{0};
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr Tp trace(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Tp sum{0};
        for (std::size_t i = 0;i < Rows; ++i)
            sum += matrix[i][i];
        return sum;
    }

}

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

    template <Arithmetic Tp, std::size_t Size>
    template <typename... Args>
    constexpr UniformTuple<Tp, Size>
        Vector<Tp, Size>::tupleBuilder(Args&&... args) const noexcept
    {
        if (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

    template <Arithmetic Tp, std::size_t Size>
    template <Arithmetic Up>
        requires std::convertible_to<Tp, Up>
    [[nodiscard]] constexpr Vector<Tp, Size>::operator
        Vector<Up, Size>() const noexcept
    {
        return std::apply([](auto&&... args) -> Vector<Up, Size> {
            return Vector<Up, Size>{ std::move(std::tuple{
                static_cast<Up>(args)... }) };
        }, static_cast<Base const&>(*this));
    }

    template <Arithmetic Tp, std::size_t Size>
    template <std::size_t USize>
        requires (USize > Size)
    [[nodiscard]] constexpr Vector<Tp, Size>::operator
        Vector<Tp, USize>() const noexcept
    {
        Vector<Tp, USize> base;
        for (std::size_t i = 0 ;i < Size; ++i)
            base[i] = (*this)[i];
        return base;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size>
        Vector<Tp, Size>::operator- (void) const noexcept
    {
        return std::apply([]<typename... Args>(Args&&... args) {
                return Vector{std::forward<Args>(-args)...};
            }, std::apply([]<typename... Args>(Args&&... args) {
                return tupleReverser(std::forward<Args>(args)...);
            }, static_cast<Base const&>(*this)));
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Tp dot(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        return std::inner_product(left.begin(), left.end(),
            right.begin(), static_cast<Tp>(0));
    }

    template <Arithmetic Tp>
    [[nodiscard]] constexpr Tp cross(
        Vector<Tp, 2> const& left,
        Vector<Tp, 2> const& right) noexcept
    {
        return left[0] * right[1] - left[1] * right[0];
    }

    template <Arithmetic Up, Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Up, Size>
        vectorCast(Vector<Tp, Size> const& vector) noexcept
    {
        return static_cast<Vector<Up, Size>>(vector);
    }

    template <Arithmetic Tp, std::size_t Size>
    template <Arithmetic Up>
    [[nodiscard]] constexpr Up Vector<Tp, Size>::length(
        void) const noexcept
    {
        Tp sum = accumulate(*this, Tp{0}, [](Tp const& value)
            { return value * value; });
        return static_cast<Up>(std::sqrt(sum));
    }

    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> floor(
        Vector<Tp, Size> vector)
    {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::floor(value); });
        return vector;
    }

    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> ceil(
        Vector<Tp, Size> vector)
    {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::ceil(value); });
        return vector;
    }

    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> round(
        Vector<Tp, Size> vector)
    {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::round(value); });
        return vector;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator +=(
        Vector<Tp, Size> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left + right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator -=(
        Vector<Tp, Size> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left - right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator *=(
        Vector<Tp, Size> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left * right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator /=(
        Vector<Tp, Size> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left / right; });
        return *this;
    }


    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator %=(
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left % right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator ^=(
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left ^ right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator |=(
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left | right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator &=(
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right)->Tp{ return left & right; });
        return *this;
    }


    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator +=(
        Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator -=(
        Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator *=(
        Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator /=(
        Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator %=(
        Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator ^=(
        Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator |=(
        Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value |= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator &=(
        Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left / right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Vector<Tp, Size> const& left,
        Tp const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Vector<Tp, Size> const& left,
        Tp const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Vector<Tp, Size> const& left,
        Tp const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Vector<Tp, Size> const& left,
        Tp const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left / right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(left, result.begin(),
            [&right](Tp const& left) -> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Tp const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Tp const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Tp const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Tp const& left,
        Vector<Tp, Size> const& right)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left / right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Size> result;
        std::ranges::transform(right, result.begin(),
            [&left](Tp const& right) -> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::weak_ordering>
    [[nodiscard]] constexpr std::weak_ordering lexicographicalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        for (std::size_t i = 0; i != Size - 1; ++i)
            if (left[i] != right[i])
                return left[i] <=> right[i];
        return left[Size - 1] <=> right[Size - 1];
    }

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] constexpr std::partial_ordering totalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        auto first = left[0] <=> right[0];
        for (std::size_t i = 1; i != Size; ++i)
            if ((left[i] <=> right[i]) != first)
                return std::partial_ordering::unordered;
        return first;
    }

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] bool constexpr operator== (
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        return totalCompare(left, right)
            == std::partial_ordering::equivalent;
    }

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] std::partial_ordering constexpr operator<=> (
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        return totalCompare(left, right);
    }

    [[nodiscard]] constexpr Vector2f operator"" _x(
        float128 value) noexcept
    {
        return {value, 0.f};
    }

    [[nodiscard]] constexpr Vector2u operator"" _x(
        uint64 value) noexcept
    {
        return {value, 0u};
    }

    [[nodiscard]] constexpr Vector2f operator"" _y(
        float128 value) noexcept
    {
        return {0.f, value};
    }

    [[nodiscard]] constexpr Vector2u operator"" _y(
        uint64 value) noexcept
    {
        return {0u, value};
    }

    [[nodiscard]] constexpr Vector3f operator"" _z(
        float128 value) noexcept
    {
        return {0.f, 0.f, value};
    }

    [[nodiscard]] constexpr Vector3u operator"" _z(
        uint64 value) noexcept
    {
        return {0u, 0u, value};
    }

    [[nodiscard]] constexpr Vector4f operator"" _w(
        float128 value) noexcept
    {
        return {0.f, 0.f, 0.f, value};
    }

    [[nodiscard]] constexpr Vector4u operator"" _w(
        uint64 value) noexcept
    {
        return {0u, 0u, 0u, value};
    }

}

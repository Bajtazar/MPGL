#pragma once

#include <type_traits>
#include <inttypes.h>

namespace ge {

    template <typename T>
    concept Arithmetic = std::is_arithmetic<T>::value;

    template <Arithmetic T>
    struct Vector2 {
        constexpr Vector2(T x, T y) noexcept : x(x), y(y) {}
        constexpr Vector2(void) noexcept = default;

        T x;
        T y;

        constexpr Vector2& operator+=(const Vector2& left) noexcept;
        constexpr Vector2& operator-=(const Vector2& right) noexcept;
        constexpr Vector2& operator*=(const T& value) noexcept;
        constexpr Vector2& operator/=(const T& value) noexcept;

        template<std::size_t Index>
        constexpr auto&& get() &  { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() && { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() const &  { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() const && { return __getHelper<Index>(*this); }

    private:
        template<std::size_t Index, typename _This>
        constexpr auto&& __getHelper(_This&& __this) {
            static_assert(Index < 2, "Index out of Vector2 bounds");
            if constexpr (Index == 0) return std::forward<_This>(__this).x;
            if constexpr (Index == 1) return std::forward<_This>(__this).y;
        }
    };

    template class Vector2<float>;
    template class Vector2<uint16_t>;

    typedef Vector2<float> Vector2f;
    typedef Vector2<int32_t> Vector2i;

    template <Arithmetic T>
    constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& left) noexcept {
        x += left.x;
        y += left.y;
        return *this;
    }

    template <Arithmetic T>
    constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& left) noexcept {
        x -= left.x;
        y -= left.y;
        return *this;
    }

    template <Arithmetic T>
    constexpr Vector2<T>& Vector2<T>::operator*=(const T& left) noexcept {
        x *= left;
        y *= left;
        return *this;
    }

    template <Arithmetic T>
    constexpr Vector2<T>& Vector2<T>::operator/=(const T& left) noexcept {
        x /= left;
        y /= left;
        return *this;
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right) noexcept {
        return {left.x + right.x, left.y + right.y};
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right) noexcept {
        return {left.x - right.x, left.y - right.y};
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator*(const Vector2<T>& left, const T& right) noexcept {
        return {left.x * right, left.y * right};
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator/(const Vector2<T>& left, const T& right) noexcept {
        return {left.x / right, left.y / right};
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator*(const T& left, const Vector2<T>& right) noexcept {
        return {left * right.x, left * right.y};
    }

    template <Arithmetic T>
    constexpr Vector2<T> operator/(const T& left, const Vector2<T>& right) noexcept {
        return {left / right.x, left / right.y};
    }

}

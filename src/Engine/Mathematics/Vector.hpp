#pragma once

#include <tuple>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

#include "../Utility/Concepts.hpp"

namespace ge {

    template <Arithmetic T, AllSame<T>... Args>
    class Vector : public std::tuple<T, Args...> {
    public:
        using value_type = T;

        constexpr Vector(const T& first, const Args&... args) noexcept : std::tuple<T, Args...>{first, args...} {
            std::reverse(reinterpret_cast<T*>(this), reinterpret_cast<T*>(this) + sizeof...(Args) + 1);
        }
        constexpr Vector(void) noexcept = default;

        template <std::size_t Index>
            requires (Index <= sizeof...(Args))
        constexpr T& get(void) noexcept { return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&>(*this)); }

        template <std::size_t Index>
            requires (Index <= sizeof...(Args))
        constexpr const T& get(void) const noexcept { return std::get<sizeof...(Args) - Index>(static_cast<const std::tuple<T, Args...>&>(*this)); }

        template <std::size_t Index>
            requires (Index <= sizeof...(Args))
        constexpr T&& get(void) noexcept { return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&&>(*this)); }

        constexpr std::size_t size(void) const noexcept { return sizeof...(Args) + 1; }

        constexpr Vector& operator+=(const Vector& right) noexcept;
        constexpr Vector& operator-=(const Vector& right) noexcept;
        constexpr Vector& operator*=(const Vector& right) noexcept;
        constexpr Vector& operator/=(const Vector& right);

        constexpr Vector& operator+=(const T& right) noexcept;
        constexpr Vector& operator-=(const T& right) noexcept;
        constexpr Vector& operator*=(const T& right) noexcept;
        constexpr Vector& operator/=(const T& right);

        template <Arithmetic U, AllSame<U>... UArgs>
            requires (sizeof...(UArgs) == sizeof...(Args))
        constexpr operator Vector<U, UArgs...>() const noexcept;

        template <AllSame<T>... TArgs>
            requires (sizeof...(TArgs) > sizeof...(Args))
        constexpr operator Vector<T, TArgs...>() const noexcept;

        template <Arithmetic value_type>
        class Iterator : public std::iterator<std::contiguous_iterator_tag, value_type, std::ptrdiff_t, value_type*, value_type&> {
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using pointer = value_type*;

            constexpr explicit Iterator(value_type* iter) noexcept : iter(iter) {}
            constexpr explicit Iterator(void) noexcept = default;

            constexpr Iterator& operator++(void) noexcept { ++iter; return *this; }
            constexpr Iterator operator++(int) noexcept { auto tmp = *this; ++iter; return tmp; }

            constexpr Iterator& operator--(void) noexcept { --iter; return *this; }
            constexpr Iterator operator--(int) noexcept { auto tmp = *this; --iter; return tmp; }

            constexpr reference operator*(void) const noexcept { return *iter; }
            constexpr pointer operator->(void) noexcept { return iter; }

            constexpr Iterator& operator+=(difference_type offset) noexcept { iter += offset; return *this; }
            constexpr Iterator& operator-=(difference_type offset) noexcept { iter -= offset; return *this; }
            constexpr Iterator operator[](difference_type offset) const noexcept { auto temp = *this; temp += offset; return temp; }

            friend constexpr bool operator== (const Iterator& right, const Iterator& left) noexcept { return right.iter == left.iter; }
            friend constexpr bool operator!= (const Iterator& right, const Iterator& left) noexcept { return right.iter != left.iter; }

            friend constexpr Iterator operator+ (const Iterator& right, difference_type left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
            friend constexpr Iterator operator+ (difference_type right, const Iterator& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
            friend constexpr Iterator operator- (const Iterator& right, difference_type left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
            friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.iter - left.iter; }

            friend constexpr bool operator> (const Iterator& right, const Iterator& left) noexcept { return right.iter > left.iter; }
            friend constexpr bool operator< (const Iterator& right, const Iterator& left) noexcept { return right.iter < left.iter; }
            friend constexpr bool operator>= (const Iterator& right, const Iterator& left) noexcept { return right.iter >= left.iter; }
            friend constexpr bool operator<= (const Iterator& right, const Iterator& left) noexcept { return right.iter <= left.iter; }
        private:
            value_type* iter;
        };

        using iterator = Iterator<T>;
        using const_iterator = Iterator<const T>;

        iterator begin(void) noexcept { return iterator{ reinterpret_cast<T*>(this) }; }
        iterator end(void) noexcept { return iterator{ reinterpret_cast<T*>(this) + sizeof...(Args) + 1 }; }

        const_iterator begin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) }; }
        const_iterator end(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) + sizeof...(Args) + 1 }; }

        const_iterator cbegin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) }; }
        const_iterator cend(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) + sizeof...(Args) + 1 }; }

        auto rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        auto rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        auto crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        T& operator[] (std::size_t index) noexcept { return *(reinterpret_cast<T*>(this) + index); }
        const T& operator[] (std::size_t index) const noexcept { return *(reinterpret_cast<const T*>(this) + index); }

        explicit constexpr Vector(std::tuple<T, Args...>&& tuple) noexcept : std::tuple<T, Args...> { std::move(tuple) } {}
    };

    template <Arithmetic T, AllSame<T>... Args>
    template <Arithmetic U, AllSame<U>... UArgs>
        requires (sizeof...(UArgs) == sizeof...(Args))
    constexpr Vector<T, Args...>::operator Vector<U, UArgs...>() const noexcept {
        return std::apply([]<typename... IArgs>(const IArgs&... args) -> Vector<U, UArgs...> {
            return Vector<U, UArgs...>{ std::move(std::tuple{ static_cast<U>(args)... }) };
        }, static_cast<const std::tuple<T, Args...>&>(*this));
    }

    template <Arithmetic T, AllSame<T>... Args>
    template <AllSame<T>... TArgs>
        requires (sizeof...(TArgs) > sizeof...(Args))
    constexpr Vector<T, Args...>::operator Vector<T, TArgs...>() const noexcept {
        Vector<T, TArgs...> base;
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr T dotProduct(const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        return std::inner_product(left.begin(), left.end(), right.begin(), static_cast<T>(0));
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    constexpr decltype(auto) get(Vector<T, Args...>&& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&&>(vector));
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    constexpr decltype(auto) get(Vector<T, Args...>& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&>(vector));
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    constexpr decltype(auto) get(const Vector<T, Args...>& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<const std::tuple<T, Args...>&>(vector));
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator+= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left + right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator-= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left - right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator*= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left * right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator/= (const Vector& right) {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left / right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator+= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value += right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator-= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value -= right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator*= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value *= right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator/= (const T& right) {
        std::ranges::for_each(*this, [&right](T& value) -> void { value /= right; });
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left * right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator/ (const Vector<T, Args...>& left, const Vector<T, Args...>& right) {
        Vector<T, Args...> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left / right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left * right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator/ (const Vector<T, Args...>& left, const T& right) {
        Vector<T, Args...> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left / right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left * right; });
        return result;
    }

    template class Vector<float, float>;
    template class Vector<uint32_t, uint32_t>;

    typedef Vector<float, float> Vector2f;
    typedef Vector<uint32_t, uint32_t> Vector2i;

    constexpr Vector2f operator"" _x(long double value) noexcept {
        return Vector2f{static_cast<float>(value), 0.f};
    }

    constexpr Vector2i operator"" _x(unsigned long long int value) noexcept {
        return Vector2i{static_cast<uint32_t>(value), 0};
    }

    constexpr Vector2f operator"" _y(long double value) noexcept {
        return Vector2f{0.f, static_cast<float>(value)};
    }

    constexpr Vector2i operator"" _y(unsigned long long int value) noexcept {
        return Vector2i{0, static_cast<uint32_t>(value)};
    }

}

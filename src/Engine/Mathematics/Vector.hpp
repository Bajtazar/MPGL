#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include "../Traits/TupleTraits.hpp"
#include "../Traits/Concepts.hpp"

namespace ge {

    template <Arithmetic T, std::size_t Size>
    class Vector : public UniformTuple<T, Size> {
    public:
        using value_type = T;

        template <Arithmetic... Args>
            requires (sizeof...(Args) == Size)
        constexpr Vector(const Args&... args) noexcept : UniformTuple<T, Size>{args...} {
            std::reverse(reinterpret_cast<T*>(this), reinterpret_cast<T*>(this) + sizeof...(Args));
        }
        constexpr Vector(void) noexcept = default;

        template <std::size_t Index>
            requires (Index < Size)
        constexpr T& get(void) noexcept { return std::get<Size - 1 - Index>(static_cast<UniformTuple<T, Size>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr const T& get(void) const noexcept { return std::get<Size - 1 - Index>(static_cast<const UniformTuple<T, Size>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr T&& get(void) noexcept { return std::get<Size - 1 - Index>(static_cast<UniformTuple<T, Size>&&>(*this)); }

        consteval std::size_t size(void) const noexcept { return Size; }

        constexpr Vector& operator+=(const Vector& right) noexcept;
        constexpr Vector& operator-=(const Vector& right) noexcept;
        constexpr Vector& operator*=(const Vector& right) noexcept;
        constexpr Vector& operator/=(const Vector& right);

        constexpr Vector& operator+=(const T& right) noexcept;
        constexpr Vector& operator-=(const T& right) noexcept;
        constexpr Vector& operator*=(const T& right) noexcept;
        constexpr Vector& operator/=(const T& right);

        template <Arithmetic U>
        constexpr operator Vector<U, Size>() const noexcept;

        template <std::size_t USize>
            requires (USize > Size)
        constexpr operator Vector<T, USize>() const noexcept;

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
        iterator end(void) noexcept { return iterator{ reinterpret_cast<T*>(this) + Size }; }

        const_iterator begin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) }; }
        const_iterator end(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) + Size }; }

        const_iterator cbegin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) }; }
        const_iterator cend(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(this) + Size }; }

        auto rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        auto rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        auto crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        auto crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        T& operator[] (std::size_t index) noexcept { return *(reinterpret_cast<T*>(this) + index); }
        const T& operator[] (std::size_t index) const noexcept { return *(reinterpret_cast<const T*>(this) + index); }

        explicit constexpr Vector(UniformTuple<T, Size>&& tuple) noexcept : UniformTuple<T, Size>{ std::move(tuple) } {}
    };

    template <Arithmetic T, std::size_t Size>
    template <Arithmetic U>
    constexpr Vector<T, Size>::operator Vector<U, Size>() const noexcept {
        return std::apply([]<typename... IArgs>(const IArgs&... args) -> Vector<U, Size> {
            return Vector<U, Size>{ std::move(std::tuple{ static_cast<U>(args)... }) };
        }, static_cast<const UniformTuple<T, Size>&>(*this));
    }

    template <Arithmetic T, std::size_t Size>
    template <std::size_t USize>
        requires (USize > Size)
    constexpr Vector<T, Size>::operator Vector<T, USize>() const noexcept {
        Vector<T, USize> base;
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr T dotProduct(const Vector<T, Size>& left, const Vector<T, Size>& right) noexcept {
        return std::inner_product(left.begin(), left.end(), right.begin(), static_cast<T>(0));
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator+= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left + right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator-= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left - right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator*= (const Vector& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left * right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator/= (const Vector& right) {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left / right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator+= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value += right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator-= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value -= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator*= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value *= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size>& Vector<T, Size>::operator/= (const T& right) {
        std::ranges::for_each(*this, [&right](T& value) -> void { value /= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator+ (const Vector<T, Size>& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left + right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator- (const Vector<T, Size>& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left - right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator* (const Vector<T, Size>& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left * right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator/ (const Vector<T, Size>& left, const Vector<T, Size>& right) {
        Vector<T, Size> result;
        std::ranges::transform(left, right, result.begin(), [](const T& left, const T& right)->T{ return left / right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator+ (const Vector<T, Size>& left, const T& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left + right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator- (const Vector<T, Size>& left, const T& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left - right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator* (const Vector<T, Size>& left, const T& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left * right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator/ (const Vector<T, Size>& left, const T& right) {
        Vector<T, Size> result;
        std::ranges::transform(left, result.begin(), [&right](const T& left) -> T { return left / right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator+ (const T& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left + right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator- (const T& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left - right; });
        return result;
    }

    template <Arithmetic T, std::size_t Size>
    constexpr Vector<T, Size> operator* (const T& left, const Vector<T, Size>& right) noexcept {
        Vector<T, Size> result;
        std::ranges::transform(right, result.begin(), [&left](const T& right) -> T { return left * right; });
        return result;
    }

    template class Vector<float, 2>;
    template class Vector<uint32_t, 2>;

    typedef Vector<float, 2> Vector2f;
    typedef Vector<uint32_t, 2> Vector2i;

    template <Arithmetic T>
    using TwoVector = Vector<T, 2>;

    template <Arithmetic T>
    using ThreeVector = Vector<T, 3>;

    template <Arithmetic T>
    using FourVector = Vector<T, 4>;

    constexpr Vector2f operator"" _x(long double value) noexcept {
        return Vector2f{static_cast<float>(value), 0.f};
    }

    constexpr Vector2i operator"" _x(unsigned long long int value) noexcept {
        return Vector2i{static_cast<uint32_t>(value), 0u};
    }

    constexpr Vector2f operator"" _y(long double value) noexcept {
        return Vector2f{0.f, static_cast<float>(value)};
    }

    constexpr Vector2i operator"" _y(unsigned long long int value) noexcept {
        return Vector2i{0, static_cast<uint32_t>(value)};
    }

}

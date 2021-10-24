#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <math.h>

#include "../Traits/TupleTraits.hpp"
#include "../Traits/Concepts.hpp"
#include "../Utility/Ranges.hpp"

namespace ge {

    template <Arithmetic T, std::size_t Size>
    class Vector : public UniformTuple<T, Size> {
    public:
        using value_type = T;

        template <AbsolutelyArithmetic... Args>
            requires (sizeof...(Args) == Size && AllConvertible<value_type, std::remove_cvref_t<Args>...>)
        constexpr Vector(Args&&... args) noexcept;

        constexpr Vector(void) noexcept = default;

        template <std::size_t Index>
            requires (Index < Size)
        constexpr T& get(void) & noexcept { return std::get<Size - 1 - Index>(static_cast<UniformTuple<T, Size>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr const T& get(void) const& noexcept { return std::get<Size - 1 - Index>(static_cast<const UniformTuple<T, Size>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr T&& get(void) && noexcept { return std::get<Size - 1 - Index>(static_cast<UniformTuple<T, Size>&&>(*this)); }

        static consteval std::size_t size(void) noexcept { return Size; }

        template <Arithmetic U = T>
        constexpr U length(void) const noexcept;

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

        constexpr iterator begin(void) noexcept { return iterator{ &get<0>() }; }
        constexpr iterator end(void) noexcept { return iterator{ &get<0>() + Size }; }

        constexpr const_iterator begin(void) const noexcept { return const_iterator{ &get<0>() }; }
        constexpr const_iterator end(void) const noexcept { return const_iterator{ &get<0>() + Size }; }

        constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ &get<0>() }; }
        constexpr const_iterator cend(void) const noexcept { return const_iterator{ &get<0>() + Size }; }

        constexpr auto rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr auto rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr auto rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr auto rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr auto crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr auto crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr T& operator[] (std::size_t index) noexcept { return *(&get<0>() + index); }
        constexpr const T& operator[] (std::size_t index) const noexcept { return *(&get<0>() + index); }

        explicit constexpr Vector(UniformTuple<T, Size>&& tuple) noexcept : UniformTuple<T, Size>{ std::move(tuple) } {}
    private:
        template <typename... Args>
        constexpr UniformTuple<T, Size> tupleBuilder(Args&&... args) const noexcept;
    };

    template <Arithmetic T, std::size_t Size>
    template <AbsolutelyArithmetic... Args>
        requires (sizeof...(Args) == Size && AllConvertible<typename Vector<T, Size>::value_type,
            std::remove_cvref_t<Args>...>)
    constexpr Vector<T, Size>::Vector(Args&&... args) noexcept
        : UniformTuple<T, Size>{tupleBuilder(std::forward<Args>(args)...)}
    {
        if constexpr (!std::is_constant_evaluated())
            reverse(*this);
    }

    template <Arithmetic T, std::size_t Size>
    template <typename... Args>
    constexpr UniformTuple<T, Size> Vector<T, Size>::tupleBuilder(Args&&... args) const noexcept {
        if constexpr (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

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

    template <Arithmetic U, Arithmetic T, std::size_t Size>
    constexpr Vector<U, Size> vectorCast(Vector<T, Size> const& vector) noexcept {
        return static_cast<Vector<U, Size>>(vector);
    }

    template <Arithmetic T, std::size_t Size>
    template <Arithmetic U>
    constexpr U Vector<T, Size>::length(void) const noexcept {
        T sum = accumulate(*this, T{0}, [](T const& value) { return value * value; });
        return static_cast<U>(std::sqrt(sum));
    }

    template <Arithmetic T, std::size_t Size>
        requires std::is_floating_point_v<T>
    constexpr Vector<T, Size> floor(Vector<T, Size> vector) {
        std::ranges::for_each(vector, [](T& value) { value = std::floor(value); });
        return vector;
    }

    template <Arithmetic T, std::size_t Size>
        requires std::is_floating_point_v<T>
    constexpr Vector<T, Size> ceil(Vector<T, Size> vector) {
        std::ranges::for_each(vector, [](T& value) { value = std::ceil(value); });
        return vector;
    }

    template <Arithmetic T, std::size_t Size>
        requires std::is_floating_point_v<T>
    constexpr Vector<T, Size> round(Vector<T, Size> vector) {
        std::ranges::for_each(vector, [](T& value) { value = std::round(value); });
        return vector;
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

    typedef Vector<float, 2>    Vector2f;
    typedef Vector<uint32_t, 2> Vector2ui;
    typedef Vector<int32_t, 2>  Vector2i;

    template <Arithmetic T>
    using TwoVector = Vector<T, 2>;

    template <Arithmetic T>
    using ThreeVector = Vector<T, 3>;

    template <Arithmetic T>
    using FourVector = Vector<T, 4>;

    constexpr Vector2f operator"" _x(long double value) noexcept {
        return Vector2f{static_cast<float>(value), 0.f};
    }

    constexpr Vector2ui operator"" _x(unsigned long long int value) noexcept {
        return Vector2ui{static_cast<uint32_t>(value), 0u};
    }

    constexpr Vector2f operator"" _y(long double value) noexcept {
        return Vector2f{0.f, static_cast<float>(value)};
    }

    constexpr Vector2ui operator"" _y(unsigned long long int value) noexcept {
        return Vector2ui{0u, static_cast<uint32_t>(value)};
    }

}

namespace std {

    template <ge::Arithmetic T, std::size_t Size>
    struct tuple_size<ge::Vector<T, Size>> : integral_constant<size_t, Size> {};

    template <ge::Arithmetic T, std::size_t Size, std::size_t Index>
        requires (Index < Size)
    struct tuple_element<Index, ge::Vector<T, Size>> { using type = T; };

}

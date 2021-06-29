#pragma once

#include <tuple>
#include <concepts>
#include <algorithm>

namespace ge {

    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <typename T, typename... Args>
    concept AllSame = (... && std::same_as<T, Args>);

    template <Arithmetic T, AllSame<T>... Args>
    class Vector : public std::tuple<T, Args...> {
    public:
        using value_type = T;
        constexpr Vector(const T& first, const Args&... args) noexcept : std::tuple<T, Args...>{first, args...} {
            std::reverse(reinterpret_cast<T*>(this), reinterpret_cast<T*>(this) + sizeof...(Args) + 1);
        }
        constexpr Vector(void) noexcept = default;

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

        friend Vector operator+ <>(const Vector& left, const Vector& right) noexcept;
        friend Vector operator- <>(const Vector& left, const Vector& right) noexcept;
        friend Vector operator* <>(const Vector& left, const Vector& right) noexcept;
        friend Vector operator/ <>(const Vector& left, const Vector& right);

        friend Vector operator+ <>(const Vector& left, const T& right) noexcept;
        friend Vector operator- <>(const Vector& left, const T& right) noexcept;
        friend Vector operator* <>(const Vector& left, const T& right) noexcept;
        friend Vector operator/ <>(const Vector& left, const T& right);

        friend Vector operator+ <>(const T& left, const Vector& right) noexcept;
        friend Vector operator- <>(const T& left, const Vector& right) noexcept;
        friend Vector operator* <>(const T& left, const Vector& right) noexcept;
        friend Vector operator/ <>(const T& left, const Vector& right);

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
        T* end = reinterpret_cast<T*>(&base) + sizeof...(TArgs) - sizeof...(Args);
        std::copy(reinterpret_cast<const T*>(this), reinterpret_cast<const T*>(this) + sizeof...(Args) + 1, end);
        return base;
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(Vector<T, Args...>&& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&&>(vector));
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(Vector<T, Args...>& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<std::tuple<T, Args...>&>(vector));
    }

    template <std::size_t Index, Arithmetic T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(const Vector<T, Args...>& vector) noexcept {
        return std::get<sizeof...(Args) - Index>(static_cast<const std::tuple<T, Args...>&>(vector));
    }

    template <std::size_t Index, Arithmetic T, std::regular_invocable<const T&, const T&, T&> Predicate, AllSame<T>... Args>
    constexpr void performOperation(const Vector<T, Args...>& left, const Vector<T, Args...>& right, Vector<T, Args...>& target, Predicate predicate) noexcept {
        std::invoke(predicate, get<Index>(left), get<Index>(right), get<Index>(target));
        if constexpr (Index != sizeof...(Args))
            performOperation<Index + 1, T, Predicate, Args...>(left, right, target, std::move(predicate));
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator+= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left + right;});
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator-= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left - right;});
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator*= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left * right;});
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator/= (const Vector& right) {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left / right;});
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator+= (const T& right) noexcept {
        std::apply(
            [&right]<typename... IArgs>(IArgs&... args) -> void {
                ([&right](T& number)->void{ number += right; }(args), ...);
            }, static_cast<std::tuple<T, Args...>&>(*this)
        );
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator-= (const T& right) noexcept {
        std::apply(
            [&right]<typename... IArgs>(IArgs&... args) -> void {
                ([&right](T& number)->void{ number -= right; }(args), ...);
            }, static_cast<std::tuple<T, Args...>&>(*this)
        );
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator*= (const T& right) noexcept {
        std::apply(
            [&right]<typename... IArgs>(IArgs&... args) -> void {
                ([&right](T& number)->void{ number *= right; }(args), ...);
            }, static_cast<std::tuple<T, Args...>&>(*this)
        );
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator/= (const T& right) {
        std::apply(
            [&right]<typename... IArgs>(IArgs&... args) -> void {
                ([&right](T& number)->void{ number /= right; }(args), ...);
            }, static_cast<std::tuple<T, Args...>&>(*this)
        );
        return *this;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, right, result, [](const T& left, const T& right, T& target) -> void { target = left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, right, result, [](const T& left, const T& right, T& target) -> void { target = left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const Vector<T, Args...>& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, right, result, [](const T& left, const T& right, T& target) -> void { target = left * right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator/ (const Vector<T, Args...>& left, const Vector<T, Args...>& right) {
        Vector<T, Args...> result;
        performOperation<0>(left, right, result, [](const T& left, const T& right, T& target) -> void { target = left / right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, left, result, [&right](const T& left, const T& _, T& target) -> void { target = left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, left, result, [&right](const T& left, const T& _, T& target) -> void { target = left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const Vector<T, Args...>& left, const T& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(left, left, result, [&right](const T& left, const T& _, T& target) -> void { target = left * right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator/ (const Vector<T, Args...>& left, const T& right) {
        Vector<T, Args...> result;
        performOperation<0>(left, left, result, [&right](const T& left, const T& _, T& target) -> void { target = left / right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator+ (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(right, right, result, [&left](const T& _, const T& right, T& target) -> void { target = left + right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator- (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(right, right, result, [&left](const T& _, const T& right, T& target) -> void { target = left - right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator* (const T& left, const Vector<T, Args...>& right) noexcept {
        Vector<T, Args...> result;
        performOperation<0>(right, right, result, [&left](const T& _, const T& right, T& target) -> void { target = left * right; });
        return result;
    }

    template <Arithmetic T, AllSame<T>... Args>
    constexpr Vector<T, Args...> operator/ (const T& left, const Vector<T, Args...>& right) {
        Vector<T, Args...> result;
        performOperation<0>(right, right, result, [&left](const T& _, const T& right, T& target) -> void { target = left / right; });
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

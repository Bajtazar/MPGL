#pragma once

#include <tuple>
#include <concepts>
#include <algorithm>

namespace ge {

    template <typename T, typename... Args>
    concept AllSame =  (... && std::same_as<T, Args>);

    template <typename T, AllSame<T>... Args>
    class Vector : public std::tuple<T, Args...> {
    public:
        using value_type = T;
        constexpr Vector(const T& first, const Args&... args) noexcept : std::tuple<T, Args...>{first, args...} {}
        constexpr Vector(void) noexcept = default;

        constexpr Vector& operator+=(const Vector& right) noexcept;
        constexpr Vector& operator-=(const Vector& right) noexcept;
        constexpr Vector& operator*=(const Vector& right) noexcept;
        constexpr Vector& operator/=(const Vector& right);

        template <typename U, AllSame<U>... UArgs>
            requires (sizeof...(UArgs) == sizeof...(Args))
        constexpr operator Vector<U, UArgs...>() const noexcept;

        template <AllSame<T>... TArgs>
            requires (sizeof...(TArgs) > sizeof...(Args))
        constexpr operator Vector<T, TArgs...>() const noexcept;

    private:

        template <std::size_t Index, std::regular_invocable<const T&, const T&, T&> Predicate>
        static constexpr void performOperation(const Vector& left, const Vector& right, Vector& target, Predicate predicate) noexcept;

    };

    template <typename T, AllSame<T>... Args>
    template <typename U, AllSame<U>... UArgs>
        requires (sizeof...(UArgs) == sizeof...(Args))
    constexpr Vector<T, Args...>::operator Vector<U, UArgs...>() const noexcept {
        return std::apply(
            [](const Args&... args) -> Vector<U, UArgs...> {
                return { static_cast<U>(args)... };
            }, static_cast<const std::tuple<T, Args...>&>(*this)
        );
    }

    template <typename T, AllSame<T>... Args>
    template <AllSame<T>... TArgs>
        requires (sizeof...(TArgs) > sizeof...(Args))
    constexpr Vector<T, Args...>::operator Vector<T, TArgs...>() const noexcept {
        Vector<T, TArgs...> base;
        uint8_t* end = reinterpret_cast<uint8_t*>(&base) + sizeof(Vector<T, TArgs...>) - sizeof(Vector<T, Args...>);
        std::copy(reinterpret_cast<const uint8_t*>(this), reinterpret_cast<const uint8_t*>(this) + sizeof(Vector<T, Args...>), end);
        return base;
    }

    template <std::size_t Index, typename T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(Vector<T, Args...>&& vector) noexcept {
        return std::get<Index>(static_cast<std::tuple<T, Args...>&&>(vector));
    }

    template <std::size_t Index, typename T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(Vector<T, Args...>& vector) noexcept {
        return std::get<Index>(static_cast<std::tuple<T, Args...>&>(vector));
    }

    template <std::size_t Index, typename T, AllSame<T>... Args>
        requires (sizeof...(Args) >= Index)
    decltype(auto) get(const Vector<T, Args...>& vector) noexcept {
        return std::get<Index>(static_cast<const std::tuple<T, Args...>&>(vector));
    }

    template <typename T, AllSame<T>... Args>
    template <std::size_t Index, std::regular_invocable<const T&, const T&, T&> Predicate>
    constexpr void Vector<T, Args...>::performOperation(const Vector& left, const Vector& right, Vector& target, Predicate predicate) noexcept {
        std::invoke(predicate, get<Index>(left), get<Index>(right), get<Index>(target));
        if constexpr (Index != sizeof...(Args))
            performOperation<Index + 1, Predicate>(left, right, target, std::move(predicate));
    }

    template <typename T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator+= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left + right;});
        return *this;
    }

    template <typename T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator-= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left - right;});
        return *this;
    }

    template <typename T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator*= (const Vector& right) noexcept {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left * right;});
        return *this;
    }

    template <typename T, AllSame<T>... Args>
    constexpr Vector<T, Args...>& Vector<T, Args...>::operator/= (const Vector& right) {
        performOperation<0>(*this, right, *this, [](const T& left, const T& right, T& target) -> void { target = left / right;});
        return *this;
    }

}

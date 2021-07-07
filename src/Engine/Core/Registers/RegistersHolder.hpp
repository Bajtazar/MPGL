#pragma once

#include "RegisterInterface.hpp"

#include <memory>
#include <concepts>
#include <tuple>

namespace ge {

    template <class T, class... Args>
    concept AllDerives = (... && std::derived_from<Args, T>);

    template <AllDerives<RegisterInterface>... Registers>
    class RegistersHolder : public std::tuple<std::shared_ptr<Registers>...> {
    public:
        explicit RegistersHolder(void) noexcept : std::tuple<std::shared_ptr<Registers>...>{} {}

        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        decltype(auto) get(void) noexcept { return *std::get<Index>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(*this)); }

        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        decltype(auto) get(void) noexcept { return *std::get<Index>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(*this)); }

        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        decltype(auto) get(void) const noexcept { return *std::get<Index>(static_cast<const std::tuple<std::shared_ptr<Registers>...>&>(*this)); }

        template <class T>
            requires (... || std::is_same_v<T, Registers>)
        decltype(auto) get(void) noexcept { return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(*this)); }

        template <class T>
            requires (... || std::is_same_v<T, Registers>)
        decltype(auto) get(void) noexcept { return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(*this)); }

        template <class T>
            requires (... || std::is_same_v<T, Registers>)
        decltype(auto) get(void) const noexcept { return *std::get<T>(static_cast<const std::tuple<std::shared_ptr<Registers>...>&>(*this)); }

    };

    template <std::size_t Index, AllDerives<RegisterInterface>... Registers>
        requires (sizeof...(Registers) > Index)
    decltype(auto) get(RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<Index>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <std::size_t Index, AllDerives<RegisterInterface>... Registers>
        requires (sizeof...(Registers) > Index)
    decltype(auto) get(const RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<Index>(static_cast<const std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <std::size_t Index, AllDerives<RegisterInterface>... Registers>
        requires (sizeof...(Registers) > Index)
    decltype(auto) get(RegistersHolder<Registers...>&& holder) noexcept {
        return *std::get<Index>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires (... || std::is_same_v<T, Registers>)
    decltype(auto) get(RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires (... || std::is_same_v<T, Registers>)
    decltype(auto) get(const RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<const std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires (... || std::is_same_v<T, Registers>)
    decltype(auto) get(RegistersHolder<Registers...>&& holder) noexcept {
        return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(holder));
    }

}

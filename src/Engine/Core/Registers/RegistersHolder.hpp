#pragma once

#include "RegisterInterface.hpp"

#include <concepts>
#include <memory>
#include <tuple>

namespace ge {

    template <class T, class... Args>
    concept AllDerives = std::conjunction_v<std::is_base_of<T, Args>...>;

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
            requires std::disjunction_v<std::is_same<T, Registers>...>
        decltype(auto) get(void) noexcept { return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(*this)); }

        template <class T>
            requires std::disjunction_v<std::is_same<T, Registers>...>
        decltype(auto) get(void) noexcept { return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(*this)); }

        template <class T>
            requires std::disjunction_v<std::is_same<T, Registers>...>
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
        requires std::disjunction_v<std::is_same<T, Registers>...>
    decltype(auto) get(RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires std::disjunction_v<std::is_same<T, Registers>...>
    decltype(auto) get(const RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<const std::tuple<std::shared_ptr<Registers>...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires std::disjunction_v<std::is_same<T, Registers>...>
    decltype(auto) get(RegistersHolder<Registers...>&& holder) noexcept {
        return *std::get<T>(static_cast<std::tuple<std::shared_ptr<Registers>...>&&>(holder));
    }

}

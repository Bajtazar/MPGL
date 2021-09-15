#pragma once

#include "../../Traits/Concepts.hpp"
#include "RegisterInterface.hpp"

#include <memory>
#include <tuple>

namespace ge {

    template <class T>
    concept PushableRegister = requires (
        std::shared_ptr<typename T::value_type> const& t, T reg)
        { { reg.pushBack(t) } -> std::same_as<void>; };

    template <typename... Args>
    concept AllPushableRegisters = AllDerives<RegisterInterface, Args...>
        && (PushableRegister<Args> && ...);

    // all registers have to have pushBack method
    template <AllPushableRegisters... Registers>
    class RegistersHolder : public std::tuple<Registers...> {
    public:
        explicit RegistersHolder(void) noexcept : std::tuple<Registers...>{} {}

        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        inline constexpr decltype(auto) get(void) noexcept
            { return *std::get<Index>(static_cast<std::tuple<Registers...>&>(*this)); }

        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        inline constexpr decltype(auto) get(void) const noexcept
            { return *std::get<Index>(static_cast<std::tuple<Registers...> const&>(*this)); }

        template <Included<Registers...> T>
        inline constexpr decltype(auto) get(void) noexcept
            { return *std::get<T>(static_cast<std::tuple<Registers...>&>(*this)); }

        template <Included<Registers...> T>
        inline constexpr decltype(auto) get(void) const noexcept
            { return *std::get<T>(static_cast<std::tuple<Registers...> const&>(*this)); }

        template <class T>
        void addIfDerived(std::shared_ptr<T> const& pointer);
    private:
        template <class T, class Register>
        static void pushIfDerived(Register& reg, std::shared_ptr<T> const& pointer);
    };

    template <std::size_t Index, AllDerives<RegisterInterface>... Registers>
        requires (sizeof...(Registers) > Index)
    inline constexpr decltype(auto) get(RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<Index>(static_cast<std::tuple<Registers...>&>(holder));
    }

    template <std::size_t Index, AllDerives<RegisterInterface>... Registers>
        requires (sizeof...(Registers) > Index)
    inline constexpr decltype(auto) get(const RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<Index>(static_cast<const std::tuple<Registers...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires Included<T, Registers...>
    inline constexpr decltype(auto) get(RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<std::tuple<Registers...>&>(holder));
    }

    template <class T, AllDerives<RegisterInterface>... Registers>
        requires Included<T, Registers...>
    inline constexpr decltype(auto) get(const RegistersHolder<Registers...>& holder) noexcept {
        return *std::get<T>(static_cast<const std::tuple<Registers...>&>(holder));
    }

    template <AllPushableRegisters... Registers>
    template <class T>
    void RegistersHolder<Registers...>::addIfDerived(std::shared_ptr<T> const& pointer) {
        std::apply([&](auto&... args) { (pushIfDerived(args, pointer), ...);},
            static_cast<std::tuple<Registers...>>(*this));
    }

    template <AllPushableRegisters... Registers>
    template <class T, class Register>
    void RegistersHolder<Registers...>::pushIfDerived(
        [[maybe_unused]]Register& reg, [[maybe_unused]] std::shared_ptr<T> const& pointer)
    {
        if constexpr (std::derived_from<T, typename Register::value_type>)
            reg.pushBack(std::static_pointer_cast<typename Register::value_type>(pointer));
    }

}

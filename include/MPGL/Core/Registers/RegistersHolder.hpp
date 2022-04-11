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

#include <MPGL/Core/Registers/RegisterInterface.hpp>
#include <MPGL/Traits/Concepts.hpp>

#include <tuple>

namespace mpgl {

    /**
     * Holds multiple registers and allows to perform a few
     * operations on all of them
     */
    template <class... Registers>
        requires AllInstancesOf<RegisterInterface, Registers...>
    class RegistersHolder {
    public:
        /**
         * Constructs a new Registers Holder object
         */
        explicit RegistersHolder(void) noexcept = default;

        RegistersHolder(RegistersHolder&&) = default;

        RegistersHolder& operator=(RegistersHolder&&) = default;

        /**
         * Returns the register with the given index
         *
         * @tparam Index the register's index
         * @return the register with the given index
         */
        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        [[nodiscard]] inline auto&& get(void) noexcept
            { return std::get<Index>(registers); }

        /**
         * Returns the register with the given index
         *
         * @tparam Index the register's index
         * @return the register with the given index
         */
        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        [[nodiscard]] inline auto&& get(void) const noexcept
            { return std::get<Index>(registers); }

        /**
         * Returns the register of the given type
         *
         * @tparam Tp the register's type
         * @return the register of the given type
         */
        template <class Tp>
            requires Included<Tp, Registers...>
        [[nodiscard]] inline auto&& get(void) noexcept
            { return std::get<Tp>(registers); }

        /**
         * Returns the register of the given type
         *
         * @tparam Tp the register's type
         * @return the register of the given type
         */
        template <class Tp>
            requires Included<Tp, Registers...>
        [[nodiscard]] inline auto&& get(void) const noexcept
            { return std::get<Tp>(registers); }

        /**
         * Returns the register with the given index
         *
         * @tparam Index the register's index
         * @param holder the reference to the holder
         * @return the register with the given index
         */
        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        [[nodiscard]] friend inline auto&& get(
            RegistersHolder& holder) noexcept
                { return std::get<Index>(holder.registers); }

        /**
         * Returns the register with the given index
         *
         * @tparam Index the register's index
         * @param holder the constant reference to the holder
         * @return the register with the given index
         */
        template <std::size_t Index>
            requires (sizeof...(Registers) > Index)
        [[nodiscard]] friend inline auto&& get(
            RegistersHolder const& holder) noexcept
                { return std::get<Index>(holder.registers); }

        /**
         * Returns the register of the given type
         *
         * @tparam Tp the register's type
         * @param holder the reference to the holder
         * @return the register of the given type
         */
        template <class Tp>
            requires Included<Tp, Registers...>
        [[nodiscard]] friend inline auto&& get(
            RegistersHolder& holder) noexcept
                { return std::get<Tp>(holder.registers); }

        /**
         * Returns the register of the given type
         *
         * @tparam Tp the register's type
         * @param holder the constant reference to the holder
         * @return the register of the given type
         */
        template <class Tp>
            requires Included<Tp, Registers...>
        [[nodiscard]] friend inline auto&& get(
            RegistersHolder const& holder) noexcept
                { return std::get<Tp>(holder.registers); }

        /**
         * Pushes the given pointer to the registers which
         * holds type from which pointer's type derives
         *
         * @tparam Tp the type of the pointer
         * @param pointer the constant reference to the pointer
         * object
         */
        template <class Tp>
        void addIfDerived(std::shared_ptr<Tp> const& pointer);
    private:
        /**
         * Pushes pointer to register if its derives from the
         * register's type
         *
         * @tparam Tp the type of the pointer
         * @tparam Register the type of the register
         * @param register the reference to the register object
         * @param pointer the constant reference to the pointer
         * object
         */
        template <class Tp, InstanceOf<RegisterInterface> Register>
        static void pushIfDerived(
            Register& reg,
            std::shared_ptr<Tp> const& pointer);

        std::tuple<Registers...>                        registers;
    };

}

#include <MPGL/Core/Registers/RegistersHolder.tpp>

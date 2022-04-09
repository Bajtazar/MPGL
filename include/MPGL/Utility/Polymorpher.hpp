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

#include <MPGL/Traits/Concepts.hpp>

#include <variant>

namespace mpgl {

    /**
     * Allows to use dynamic polymorphism without using virtual
     * methods. Requires the range of types to which the
     * desired type belongs to. Allows to gain significant
     * performance boost by decreasing the calls to the memory
     * and switching v-table mechanism to a few if-calls.
     *
     * @tparam Types the range of types on which the polymorpher
     * operates. The number of this types should not be too
     * large
     */
    template <class... Types>
    class Polymorpher {
    public:
        /**
         * Constructs the Polymorpher object. Requires the object
         * to be one of the underlying types
         *
         * @tparam Tp the given object type
         */
        template <Included<Types...> Tp>
        constexpr explicit Polymorpher(Tp&& base) noexcept(
            NothrowMoveConstructible<Tp>)
                : variant{std::move(base)} {}

        Polymorpher(Polymorpher const&) = default;
        Polymorpher(Polymorpher&&) = default;

        Polymorpher& operator=(Polymorpher const&) = default;
        Polymorpher& operator=(Polymorpher&&) = default;

        /**
         * Invokes given invocable on the underlying object.
         * Invocable has to be able to invoke any underlying type
         *
         * @tparam Fn the invocable type
         * @param invocable the invocable object
         * @tparam Args the types of invocable arguments
         * @param args the rest of invocable arguments
         * @return the result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::conjunction_v<std::is_invocable<
                Fn, Types&, Args...>...>
        decltype(auto) invoke(Fn&& invocable, Args&&... args);

        /**
         * Invokes given invocable on the constant underlying object.
         * Invocable has to be able to invoke any underlying type
         *
         * @tparam Fn the invocable type
         * @param invocable the invocable object
         * @tparam Args the types of invocable arguments
         * @param args the rest of invocable arguments
         * @return the result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::conjunction_v<std::is_invocable<Fn,
                Types const&, Args...>...>
        decltype(auto) invoke(Fn&& invocable, Args&&... args) const;

        /**
         * Destroy the Polymorpher object
         */
        ~Polymorpher(void) = default;
    private:
        std::variant<Types...>                          variant;
    };

    template <class... Types>
    template <typename Fn, typename... Args>
        requires std::conjunction_v<std::is_invocable<
            Fn, Types&, Args...>...>
    decltype(auto) Polymorpher<Types...>::invoke(
        Fn&& invocable, Args&&... args)
    {
        return std::visit(variant,
            [&invocable, ...args=std::forward<Args>(args)]
            (auto&& base) {
                return std::invoke(invocable, base,
                    std::forward<Args>(args)...);
            }
        );
    }

    template <class... Types>
    template <typename Fn, typename... Args>
        requires std::conjunction_v<std::is_invocable<Fn,
            Types const&, Args...>...>
    decltype(auto) Polymorpher<Types...>::invoke(
        Fn&& invocable, Args&&... args) const
    {
        return std::visit(variant,
            [&invocable, ...args=std::forward<Args>(args)]
            (auto&& base) {
                return std::invoke(invocable, base,
                    std::forward<Args>(args)...);
            }
        );
    }

}

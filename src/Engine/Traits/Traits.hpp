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

#include <type_traits>
#include <functional>
#include <concepts>
#include <utility>

#include "Types.hpp"

namespace mpgl {

    /**
     * Helper metastruct. Provides information whether the given
     * type can be constructed at the compilation time
     *
     * @tparam Tp the checked type
     * @tparam Args the constructor's arguments types
     */
    template <typename Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    class IsConstexprConstructible {
        /**
         * Method choosen by the compiler when the given type
         * can be construced at the compilation time
         *
         * @tparam Invocable the lambda expression containing
         * object construction
         * @tparam int32 the sentence forcing lambda call
         * @return std::true_type
         */
        template <class Invocable, int32 = (Invocable{}(), 0)>
        static constexpr auto helper(Invocable) -> std::true_type;

        /**
         * Method choosen by the compiler when the given type
         * cannot be constructed at the compilation tine
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given type can be constructed at
         * compilation time
         */
        static constexpr bool value = std::same_as<
            decltype(helper([]{ Tp{std::declval<Args>()...}; })),
                std::true_type>;
    };

    /**
     * The convenient shortcut for the IsConstexprConstructible value
     *
     * @tparam Tp the checked type
     * @tparam Args the constructor's arguments types
     */
    template <typename Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    constexpr bool IsConstexprConstructibleV =
        IsConstexprConstructible<Tp, Args...>::value;

    /**
     * Helper metastruct. Provides information whether the given
     * function can be executed at compile time
     *
     * @tparam Signature the function's signature
     * @tparam Function the function's pointer
     * @tparam Args the function's arguments types
     */
    template <typename Signature, Signature Function, typename... Args>
        requires std::invocable<Signature, Args...>
    class IsConstexprEvaluable {
        /**
         * Method choosen by the compiler when the given function
         * can be executed during compilation
         *
         * @tparam Invocable the lambda expression containing
         * object construction
         * @tparam int32 the sentence forcing lambda call
         * @return std::true_type
         */
        template <class Invocable, int32 = (Invocable{}(), 0)>
        static constexpr auto helper(Invocable) -> std::true_type;

        /**
         * Method choosen by the compiler when the given function
         * cannot be executed at the compilation tine
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given function can be executed at
         * compilation time
         */
        static constexpr bool value = std::same_as<
            decltype(helper([]{ std::invoke(Function, std::declval<Args>()...); })), std::true_type>;
    };

    /**
     * The convenient shortcut for the IsConstexprEvaluable value
     *
     * @tparam Signature the function's signature
     * @tparam Function the function's pointer
     * @tparam Args the function's arguments types
     */
    template <typename Signature, Signature Function, typename... Args>
    constexpr bool IsConstexprEvaluableV
        = IsConstexprEvaluable<Signature, Function, Args...>::value;

    /**
     * Helper metastruct. Provides information whether the given
     * type is instiation of the given template
     *
     * @tparam Template the base template
     * @tparam Tp the checked type
     */
    template <template <typename...> class Template, class Tp>
    class IsInstance {
        /**
         * Method choosen by the compiler when the given type is
         * an instance of the given template
         *
         * @tparam Args the template instance arguments types
         * @return std::true_type
         */
        template <typename... Args>
        static constexpr auto helper(
            Template<Args...> const&) -> std::true_type;

        /**
         * Method choosen by the compiler when the given type
         * is not an instance of the given template
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given type is an instance of the
         * given template
         */
        static constexpr bool value = std::same_as<
            decltype(helper(std::declval<Tp>())), std::true_type>;
    };

    /**
     * The convenient shortcut for the IsInstance value
     *
     * @tparam Template the base template
     * @tparam Tp the checked type
     */
    template <template <typename...> class Template, class Tp>
    constexpr bool IsInstanceV = IsInstance<Template, Tp>::value;

}

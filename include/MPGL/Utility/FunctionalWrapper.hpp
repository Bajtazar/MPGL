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

#include <concepts>
#include <memory>

namespace mpgl {

    /**
     * Constructs and casts the given type to the base type
     * upon calling a functor object
     *
     * @tparam Derived the given type
     * @tparam Base the base type
     */
    template <class Derived, class Base = Derived>
        requires std::derived_from<Derived, Base>
    struct FunctionalWrapper {
        typedef std::unique_ptr<Base>           BasePtr;

        /**
         * Checks whether the given type is nothrow constructible
         *
         * @tparam Args the type's constructor argument types
         */
        template <typename... Args>
        constexpr static bool NothrowConstructible =
            std::is_nothrow_constructible_v<Derived, Args...>;

        /**
         * Constructs a new Functional Wrapper object
         */
        FunctionalWrapper(void) noexcept = default;

        /**
         * Constructs and casts the given type into the base type
         * unique pointer
         *
         * @tparam Args the derived type's constructor argument types
         * @param args the constructor arguments
         * @return the casted object
         */
        template <typename... Args>
            requires std::constructible_from<Derived, Args...>
        BasePtr operator() (
            Args&&... args) const noexcept(
                NothrowConstructible<Args...>)
        {
            return std::make_unique<Derived>(
                std::forward<Args>(args)...);
        }
    };

}
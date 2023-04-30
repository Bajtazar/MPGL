/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

namespace mpgl {

    /**
     * Base class for security tags
     */
    struct Security {};

    /**
     * The unsecured execution tag
     */
    struct Unsecured : Security {};

    /**
     * The secured execution tag
     */
    struct Secured : Security {};

    namespace security {

        inline constexpr Unsecured                      unsecured{};
        inline constexpr Secured                        secured;

        /**
         * Checks whether the given type is the security policy tag
         *
         * @tparam Tp
         */
        template <class Tp>
        concept SecurityPolicy = std::derived_from<Tp, Security>;

        /**
         * Checks whether the given type is the secured
         * security policy tag
         *
         * @tparam Tp the checked type
         */
        template <SecurityPolicy Tp>
        constexpr bool isSecurePolicy = std::derived_from<Tp,
            Secured>;

        /**
         * Checks whether the given type is the unsecured
         * security policy tag
         *
         * @tparam Tp the checked type
         */
        template <SecurityPolicy Tp>
        constexpr bool isUnsecuredPolicy = std::derived_from<Tp,
            Unsecured>;

    }

}

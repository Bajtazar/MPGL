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

namespace mpgl {

    namespace details {

        /**
         * Base for the negative derive if operation
         *
         * @tparam Tp the positive derive if result type
         */
        template <class Tp>
        class DeriveIfBase {
        public:
            /**
             * Constructs a new derive if base object
             */
            constexpr explicit DeriveIfBase(
                void) noexcept = default;

            /**
             * Virtual destructor. Destroys the derive if base object
             */
            constexpr virtual ~DeriveIfBase(
                void) noexcept = default;
        protected:
            constexpr DeriveIfBase(
                DeriveIfBase const&) noexcept = default;
            constexpr DeriveIfBase(
                DeriveIfBase&&) noexcept = default;

            constexpr DeriveIfBase& operator=(
                DeriveIfBase const&) noexcept = default;

            constexpr DeriveIfBase& operator=(
                DeriveIfBase&&) noexcept = default;
        };

        /**
         * Special filler class
         *
         * @tparam N the number
         */
        template <size_t N>
        class DeriveIfHandler {};

    }

    /**
     * Derives from the given type if the value is true. Otherwise
     * derives from the derive if base
     *
     * @tparam Value the value flag
     * @tparam Tp the positive derive if result type
     */
    template <bool Value, class Tp>
    using DeriveIfT = std::conditional_t<
        Value, Tp, details::DeriveIfBase<Tp>>;

    /**
     * Derives from the given type if the value is true. Otherwise
     * derives from the derive if base with the size_t number
     *
     * @tparam Value the value flag
     * @tparam Tp the positive derive if result type
     */
    template <bool Value, class Tp, size_t N>
    using DeriveIfTN = std::conditional_t<
        Value, Tp, details::DeriveIfBase<details::DeriveIfHandler<N>>>;

}

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
         * Base for the negative derive if operation, designed
         * for types that can be used at compilation time
         *
         * @tparam Tp the positive derive if result type
         */
        template <class Tp>
        class DeriveIfConstexprBase {
        public:
            /**
             * Constructs a new derive if constexpr base object
             */
            constexpr explicit DeriveIfConstexprBase(
                void) noexcept = default;

            /**
             * Virtual destructor. Destroys the derive if constexpr
             * base object
             */
            constexpr virtual ~DeriveIfConstexprBase(
                void) noexcept = default;
        protected:
            constexpr DeriveIfConstexprBase(
                DeriveIfConstexprBase const&) noexcept = default;
            constexpr DeriveIfConstexprBase(
                DeriveIfConstexprBase&&) noexcept = default;

            constexpr DeriveIfConstexprBase& operator=(
                DeriveIfConstexprBase const&) noexcept = default;

            constexpr DeriveIfConstexprBase& operator=(
                DeriveIfConstexprBase&&) noexcept = default;
        };

        /**
         * Base for the negative derive if operation, designed
         * for types that cannot be used at compilation time
         *
         * @tparam Tp the positive derive if result type
         */
        template <class Tp>
        class DeriveIfNormalBase {
        public:
            /**
             * Constructs a new derive if normal base object
             */
            constexpr explicit DeriveIfNormalBase(
                void) noexcept = default;

            /**
             * Virtual destructor. destroys the derive if normal base
             * object
             */
            virtual ~DeriveIfNormalBase(
                void) noexcept = default;
        protected:
            constexpr DeriveIfNormalBase(
                DeriveIfNormalBase const&) noexcept = default;
            constexpr DeriveIfNormalBase(
                DeriveIfNormalBase&&) noexcept = default;

            constexpr DeriveIfNormalBase& operator=(
                DeriveIfNormalBase const&) noexcept = default;

            constexpr DeriveIfNormalBase& operator=(
                DeriveIfNormalBase&&) noexcept = default;
        };

    }

    /**
     * Derives from the given type if the value is true. Otherwise
     * derives from the derive if constexpr base if the constexpr
     * base flag is true or from derive if normal base if the
     * constexpr base flag is false
     *
     * @tparam Value the value flag
     * @tparam ConstexprBase the constexpr base flag
     * @tparam Tp the positive derive if result type
     */
    template <bool Value, bool ConstexprBase, class Tp>
    using DeriveIfWBST = std::conditional_t<
        Value, Tp, std::conditional_t<ConstexprBase,
            details::DeriveIfConstexprBase<Tp>,
            details::DeriveIfNormalBase<Tp>>>;

    /**
     * Derives from the given type if the value is true. Otherwise
     * derives from the derive if constexpr base if the given type
     * can be used at compilation time or from normal base
     * otherwise
     *
     * @tparam Value the value flag
     * @tparam Tp the positive derive if result type
     */
    template <bool Value, class Tp>
    using DeriveIfT = DeriveIfWBST<Value, ConstevalType<Tp>, Tp>;

}

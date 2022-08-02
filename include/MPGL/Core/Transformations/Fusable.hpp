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

namespace mpgl {

    namespace details {

        /**
         * Base for the Fusable class. Implements interface for fusing
         * with one object
         *
         * @tparam Tp the fused object's type
         */
        template <class Tp>
        class FusableBase {
        public:
            /**
             * Constructs a new fusable base object
             */
            explicit FusableBase(void) noexcept = default;

            /**
             * Pure virtual method. Has to be overloaded. Fuses
             * this object with the other one
             *
             * @param other the constant reference to the other
             * object
             */
            virtual void fuse(Tp const& other) noexcept = 0;

            /**
             * Virtual destructor. Destroys the fusable base object
             */
            virtual ~FusableBase(void) noexcept = default;
        protected:
            FusableBase(
                FusableBase const&) noexcept = default;
            FusableBase(FusableBase&&) noexcept = default;

            FusableBase& operator=(
                FusableBase const&) noexcept = default;
            FusableBase& operator=(
                FusableBase&&) noexcept = default;
        };

    }

    /**
     * An interface for the fusable objects. Allows to combine
     * objects which types are in the set of this template arguments
     * with the current one which reduces a number of instructions that
     * is later executed
     *
     * @tparam Args the types of objects that can be fused with the
     * current one
     */
    template <class... Args>
    class Fusable : public details::FusableBase<Args>... {
    public:
        using FusionBase = Fusable;

        /**
         * Constructs a new fusable object
         */
        explicit Fusable(void) noexcept = default;

        /**
         * Virtual destructor. Destroys the fusable object
         */
        virtual ~Fusable(void) noexcept = default;
    protected:
        Fusable(Fusable const&) noexcept = default;
        Fusable(Fusable&&) noexcept = default;

        Fusable& operator=(Fusable const&) noexcept = default;
        Fusable& operator=(Fusable&&) noexcept = default;
    };

}

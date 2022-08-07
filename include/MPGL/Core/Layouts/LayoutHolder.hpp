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

#include <MPGL/Events/Types/ScreenTransformationEvent.hpp>
#include <MPGL/Core/Transformations/Transformable.hpp>
#include <MPGL/Core/Layouts/LayoutTag.hpp>

#include <memory>

namespace mpgl {

    class LayoutHolder : public ScreenTransformationEvent {
    public:

    private:
        /**
         * An interface for the layout class. Creates layout
         * and performs its transformation during screen
         * transformation event
         */
        class LayoutInterface {
        public:
            /**
             * Constructs a new layout interface object
             */
            explicit LayoutInterface(void) noexcept = default;

            /**
             * Pure virtual method. Has to be overloaded. Performs
             * layout's transformation on the given transformable
             * object
             *
             * @param transformable a reference to the transformable
             * object
             * @param dimensions a constant reference to the
             * old window's dimensions
             */
            virtual void operator() (
                Transformable2D& transformable,
                Vector2u const& dimensions) const noexcept = 0;

            /**
             * Pure virtual method. Has to be overloaded. Moves this
             * object to the designated memory
             *
             * @param memory the reference to the first byte of
             * the memory
             */
            virtual void move(std::byte& memory) noexcept = 0;

            /**
             * Virtual destructor. Destroys the Layout Interface object
             */
            virtual ~LayoutInterface(void) noexcept = default;
        protected:
            LayoutInterface(LayoutInterface const&) noexcept = default;
            LayoutInterface(LayoutInterface&&) noexcept = default;

            LayoutInterface& operator=(
                LayoutInterface const&) noexcept = default;
            LayoutInterface& operator=(
                LayoutInterface&&) noexcept = default;
        };

        /**
         * Performs operation described by the Tp functor
         * on the given object during screen transformation
         * event
         *
         * @tparam Tp the type of the layout's transformer
         * @tparam Args the rest of layout's transformer constructor
         * argument types
         */
        template <InstanceOf<Transformation> Tp, class... Args>
        class Layout {
        public:
            using Tag = LayoutTag<Tp, Args...>;

            /**
             * Constructs a new layout object
             *
             * @param layoutTag a rvalue reference to the
             * layout tag object
             */
            explicit Layout(Tag&& layoutTag) noexcept
                : layoutTag{std::move(layoutTag)} {}

            Layout(Layout const&) = default;
            Layout(Layout&&) noexcept = default;

            Layout& operator=(Layout const&) = default;
            Layout& operator=(Layout&&) noexcept = default;

            /**
             * Performs layout's transformation on the given
             * transformable object
             *
             * @param transformable a reference to the transformable
             * object
             * @param dimensions a constant reference to the
             * old window's dimensions
             */
            void operator() (
                Transformable2D& transformable,
                Vector2u const& dimensions) const noexcept final;

            /**
             * Moves this object to the designated memory
             *
             * @param memory the reference to the first byte of
             * the memory
             */
            void move(std::byte& memory) noexcept final;

            /**
             * Destroys the layout object
             */
            ~Layout(void) noexcept = default;
        private:
            [[no_unique_address]] Tag                   layoutTag;
        };

        std::shared_ptr<Transformable2D>                transformable;
    };

}

#include <MPGL/Core/Layouts/LayoutHolder.tpp>

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
#include <MPGL/Events/Types/MouseReleaseEvent.hpp>
#include <MPGL/Events/Types/WindowCloseEvent.hpp>
#include <MPGL/Events/Types/MouseMotionEvent.hpp>
#include <MPGL/Events/Types/KeyReleaseEvent.hpp>
#include <MPGL/Events/Types/MousePressEvent.hpp>
#include <MPGL/Events/Types/TextWriteEvent.hpp>
#include <MPGL/Events/Types/KeyPressEvent.hpp>
#include <MPGL/Events/Types/ScrollEvent.hpp>
#include <MPGL/Events/Types/TickEvent.hpp>
#include <MPGL/Core/DimensionTraits.hpp>
#include <MPGL/Traits/DeriveIf.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Core/Drawable.hpp>

#include <memory>

namespace mpgl {

    namespace details {

        /**
         * Base class for the polymorpher
         *
         * @tparam Tp the type of the wrapped object
         */
        template <class Tp>
        class PolymorpherBase {
        public:
            using pointer = Tp*;
            using reference = Tp&;

            /**
             * Constructs a new polymorpher base object
             *
             * @param base a reference to the base object (optional)
             */
            explicit PolymorpherBase(pointer base = nullptr) noexcept
                : base{base} {}

            /**
             * Sets the new pointer used by the wrapper
             *
             * @param newBase a pointer to the wrapped object
             */
            void set(pointer newBase) noexcept
                { base = newBase; }

            /**
             * Sets the new pointer used by the wrapper
             *
             * @param newBase a reference to the wrapped object
             */
            void set(reference newBase) noexcept
                { base = &newBase; }

            /**
             * Returns the pointer currently used by the wrapper
             *
             * @return the pointer currently used by the wrapper
             */
            [[nodiscard]] pointer get(void) const noexcept
                { return base; }

            /**
             * Returns the reference currently used by the wrapper
             *
             * @return the reference currently used by the wrapper
             */
            [[nodiscard]] reference getReference(void) const noexcept
                { return *base; }

            /**
             * Virtual destructor. Destroys the polymorpher base object
             */
            virtual ~PolymorpherBase(void) noexcept = default;
        protected:
            PolymorpherBase(PolymorpherBase const&) noexcept = default;
            PolymorpherBase(PolymorpherBase&&) noexcept= default;

            PolymorpherBase& operator=(
                PolymorpherBase const&) noexcept = default;
            PolymorpherBase& operator=(
                PolymorpherBase&&) noexcept = default;
        private:
            pointer                                     base;
        };

        /**
         * Base class that extends the polymorpher implementation
         * by providing the backward compatybility with the
         * drawable interface
         *
         * @tparam Tp the type of the wrapped object
         */
        template <InstanceOf<Drawable> Tp>
        struct DrawablePolymorpher :
            private virtual PolymorpherBase<Tp>,
            public Drawable<DimensionOfT<Drawable, Tp>>
        {
            /**
             * Constructs a new drawable polymorpher object
             */
            explicit DrawablePolymorpher(void) noexcept = default;

            /**
             * Allows to draw an object on the screen
             */
            void draw(void) const noexcept
                { this->get()->draw(); }

            /**
             * Virtual destructor. Destroys the drawable polymorpher
             * object
             */
            virtual ~DrawablePolymorpher(
                void) noexcept override = default;
        };

        /**
         * Base class that extends the polymorpher implementation
         * by providing the backward compatybility with the
         * transformable interface
         *
         * @tparam Tp the type of the wrapped object
         */
        template <InstanceOf<Transformable> Tp>
        struct TransformablePolymorpher :
            private virtual PolymorpherBase<Tp>,
            public Transformable<DimensionOfT<Transformable, Tp>>
        {
            using Transformator = Transformation<
                DimensionOfT<Transformable, Tp>>;

            /**
             * Constructs a new transformable polymorpher object
             */
            explicit TransformablePolymorpher(void) noexcept = default;

            /**
             * Performs transformation on this object
             *
             * @param transformator the constant reference to the object
             * perfoming a transformation
             */
            void transform(Transformator const& transformator) noexcept
                { this->get()->transform(transformator); }

            /**
             * Virtual destructor. Destroys the transformable
             * polymorpher object
             */
            virtual ~TransformablePolymorpher(
                void) noexcept override = default;
        };

        /**
         * Base class that extends the polymorpher implementation
         * by providing the backward compatybility with the
         * key press event interface
         *
         * @tparam Tp the type of the wrapped object
         */
        template <std::derived_from<KeyPressEvent> Tp>
        struct KeyPressEventPolymorpher :
            private virtual PolymorpherBase<Tp>,
            public KeyPressEvent
        {
            /**
             * Constructs a new key press event polymorpher object
             */
            explicit KeyPressEventPolymorpher(void) noexcept = default;

            /**
             * Notifies when the key is pressed
             *
             * @param key the pressed key
             */
            void onKeyPress(Key const& key) noexcept
                { this->get()->onKeyPress(key); }

            /**
             * Virtual destructor. Destroys the key press event
             * polymorpher object
             */
            virtual ~KeyPressEventPolymorpher(
                void) noexcept override = default;
        };

        /**
         * Base class that extends the polymorpher implementation
         * by providing the backward compatybility with the
         * key release event interface
         *
         * @tparam Tp the type of the wrapped object
         */
        template <std::derived_from<KeyReleaseEvent> Tp>
        struct KeyReleaseEventPolymorpher :
            private virtual PolymorpherBase<Tp>,
            public KeyReleaseEvent
        {
            /**
             * Constructs a new key release event polymorpher object
             */
            explicit KeyReleaseEventPolymorpher(void) noexcept = default;

            /**
             * Notifies when the key is released
             *
             * @param key the released key
             */
            void onKeyRelease(Key const& key) noexcept
                { this->get()->onKeyRelease(key); }

            /**
             * Virtual destructor. Destroys the key release event
             * polymorpher object
             */
            virtual ~KeyReleaseEventPolymorpher(
                void) noexcept override = default;
        };

        /**
         * Base class that extends the polymorpher implementation
         * by providing the backward compatybility with the
         * key release event interface
         *
         * @tparam Tp the type of the wrapped object
         */
        template <std::derived_from<MouseMotionEvent> Tp>
        struct MouseMotionEventPolymorpher :
            private virtual PolymorpherBase<Tp>,
            public MouseMotionEvent
        {
            /**
             * Constructs a new mouse motion event polymorpher object
             */
            explicit MouseMotionEventPolymorpher(void) noexcept = default;

            /**
             * Notifies when mouse is moved
             *
             * @param position the new mouse position
             */
            void onMouseMotion(Vector2f const& position) noexcept
                { this->get()->onMouseMotion(position); }

            /**
             * Virtual destructor. Destroys the mouse motion event
             * polymorpher object
             */
            virtual ~MouseMotionEventPolymorpher(
                void) noexcept override = default;
        };

    }

    /**
     * Convinient wrapper that allows to easy change the
     * object that is being handled by the smart pointers and also
     * can be used by the MPGL-compatible systems that utilizes
     * concepts to determine which events are being derived by the
     * object type and allows to call them
     *
     * @tparam Tp the type of the wrapped object
     */
    template <class Tp>
    struct Polymorpher :
        private virtual details::PolymorpherBase<Tp>,
        public DeriveIfT<InstanceOf<Tp, Drawable>, details::DrawablePolymorpher<Tp>>,
        public DeriveIfT<InstanceOf<Tp, Transformable>, details::TransformablePolymorpher<Tp>>,
        public DeriveIfT<std::derived_from<Tp, KeyPressEvent>, details::KeyPressEventPolymorpher<Tp>>,
        public DeriveIfT<std::derived_from<Tp, KeyReleaseEvent>, details::KeyReleaseEventPolymorpher<Tp>>,
        public DeriveIfT<std::derived_from<Tp, MouseMotionEvent>, details::MouseMotionEventPolymorpher<Tp>>
    {
        using pointer = typename details::PolymorpherBase<Tp>::pointer;

        /**
         * Constructs a new polymorpher object
         *
         * @param base a pointer to the wrapped object
         */
        explicit Polymorpher(pointer base) noexcept
            : details::PolymorpherBase<Tp>{base} {}

        Polymorpher(Polymorpher const&) noexcept = default;
        Polymorpher(Polymorpher&&) noexcept= default;

        Polymorpher& operator=(
            Polymorpher const&) noexcept = default;
        Polymorpher& operator=(
            Polymorpher&&) noexcept = default;

        using details::PolymorpherBase<Tp>::set;
        using details::PolymorpherBase<Tp>::get;
        using details::PolymorpherBase<Tp>::getReference;

        /**
         * Virtual destructor. Destroys the polymorpher object
         */
        virtual ~Polymorpher(
            void) noexcept override = default;
    };

}

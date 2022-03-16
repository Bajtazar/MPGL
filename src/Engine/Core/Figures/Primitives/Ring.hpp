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

#include "../Elliptic.hpp"

namespace mpgl {

    /**
     * Represents the ring on the screen
     */
    class Ring : public Elliptic {
    public:
        /**
         * Represents the ellipse describing inner outline
         * of the ring
         */
        class InnerEllipse : public Transformable2D {
        public:
            /**
             * Construct a new ellipse with given center
             * and semi-axis. The angle specifies
             * a counterclockwise angle bewteen the x-axis
             * and x-semi-axis component
             *
             * @param center the center of the ellipse
             * @param semiAxis the semi-axis of the ellipse
             * @param angle the angle between x-axis and semi-axis
             */
            InnerEllipse(
                Vector2f const& center = {},
                Vector2f const& semiAxis = {},
                float32 angle = 0.f);

            /**
             * Construct a new circle with given center and
             * radius
             *
             * @param center the center of the circle
             * @param radius the radius of the circle
             */
            InnerEllipse(
                Vector2f const& center,
                float radius);

            InnerEllipse(InnerEllipse const& ellipse) = default;
            InnerEllipse(InnerEllipse&& ellipse) noexcept = default;

            InnerEllipse& operator=(
                InnerEllipse const& ellipse) = default;
            InnerEllipse& operator=(
                InnerEllipse&& ellipse) noexcept = default;

            /**
             * Transforms the figure during the screen
             * transformation event
             *
             * @param oldDimensions the old screen dimensions
             */
            void onScreenTransformation(
                Vector2u const& oldDimensions) noexcept final;

            /**
             * Translates the figure by the given shift vector
             *
             * @param shift the shift vector
             */
            void translate(Vector2f const& shift) noexcept final;

            /**
             * Scales the figure around given center by
             * a given factor
             *
             * @param center the scale center
             * @param factor the scale factor
             */
            void scale(Vector2f const& center,
                float32 factor) noexcept final;

            /**
             * Rotates the figure around given point by
             * the given angle counter clockwise
             *
             * @param center the rotation point
             * @param angle the rotation angle [in rads]
             */
            void rotate(Vector2f const& center,
                float32 angle) noexcept final;

            /**
             * Rotates the figure around given point using
             * given matrix
             *
             * @param center the rotation point
             * @param rotation the rotation matrix
             */
            void rotate(Vector2f const& center,
                Matrix2f const& rotation) noexcept final;

            /**
             * Returns the position of the center of the ellipse
             *
             * @return the position of the center of the ellipse
             */
            [[nodiscard]] Vector2f getCenter(void) const noexcept;

            /**
             * Returns the semi-axis of the ellipse. If they are
             * equal then the ellipse is a circle
             *
             * @return the semi-axis of the ellipse
             */
            [[nodiscard]] Vector2f getSemiAxis(void) const noexcept;

            /**
             * Destroy the Inner Ellipse object
             */
            ~InnerEllipse(void) noexcept = default;

            friend class Ring;
        private:
            typedef Adapter<Vector2f>                       Vertex;
            typedef std::array<Vertex, 4>                   Vertices;

            Vertices                                        vertices;
            Matrix2f                                        outline;

            /**
             * Constructs the vector with ellipse vertices
             *
             * @param center the center of the ellipse
             * @param semiAxis the semi-axis of the ellipse
             * @param angle the angle between x-axis and semi-axis
             * @return the ellipse's vertices
             */
            static Vertices ellipseVertices(Vector2f const& center,
                Vector2f const& semiAxis, float32 angle);

            /**
             * Constructs the vector with circle vertices
             *
             * @param center the center of the circle
             * @param radius the radius of the circle
             * @return the circle's vertices
             */
            static Vertices circleVertices(Vector2f const& center,
                float32 radius);

            /**
             * Actualizes the matrices responsible for the outline
             */
            void actualizeMatrices(void) noexcept;
        };

        /**
         * Construct a new ring with given center,
         * color, semi-axis and the inner ellipse. The
         * angle specifies a counterclockwise angle
         * between the x-axis and x-semi-axis component
         *
         * @param center the center of the ring
         * @param semiAxis the semi-axis of the outer ellipse
         * @param innerEllipse the inner ellipse
         * @param color the color of the ring
         * @param angle the angle between x-axis and semi-axis
         */
        Ring(
            Vector2f const& center = {},
            Vector2f const& semiAxis = {},
            InnerEllipse const& innerEllipse = {},
            Color const& color = {},
            float32 angle = 0.f);

        /**
         * Construct a new ring with given center,
         * color, outer and inner semi-axis. The
         * angle specifies a counterclockwise angle
         * between the x-axis and x-semi-axis component
         *
         * @param center the center of the ring
         * @param outerSemiAxis the semi-axis of the outer ellipse
         * @param innerSemiAxis the semi-axis of the inner ellipse
         * @param color the color of the ring
         * @param angle the angle between x-axis and semi-axis
         */
        Ring(
            Vector2f const& center,
            Vector2f const& outerSemiAxis,
            Vector2f const& innerSemiAxis,
            Color const& color = {},
            float32 angle = 0.f);

        /**
         * Construct a new circular ring with given center,
         * radius, color and inner ellipse
         *
         * @param center the center of the ring
         * @param radius the radius of the ring
         * @param innerEllipse the inner ellipse
         * @param color the color of the ring
         */
        Ring(
            Vector2f const& center,
            float32 radius,
            InnerEllipse const& innerEllipse = {},
            Color const& color = {});

        /**
         * Construct a new circular ring with given center,
         * inner and outer radius and ring color
         *
         * @param center the center of the ring
         * @param outerRadius the radius of the outer circle
         * @param innerRadius the radius of the inner circle
         * @param color the color of the ring
         */
        Ring(
            Vector2f const& center,
            float32 outerRadius,
            float32 innerRadius,
            Color const& color = {});

        Ring(Ring const& ring) = default;
        Ring(Ring&& ring) noexcept = default;

        Ring& operator=(Ring const& ring) = default;
        Ring& operator=(Ring&& ring) noexcept = default;

        /**
         * Draws the ring on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Transforms the figure during the screen
         * transformation event
         *
         * @param oldDimensions the old screen dimensions
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        void translate(Vector2f const& shift) noexcept final;

        /**
         * Scales the figure around given center by
         * a given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        void scale(Vector2f const& center,
            float32 factor) noexcept final;

        /**
         * Rotates the figure around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        void rotate(Vector2f const& center,
            float32 angle) noexcept final;

        /**
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept final;

        /**
         * Returns the center of the ring
         *
         * @return the center of the ring
         */
        [[nodiscard]] Vector2f getCenter(void) const noexcept;

        /**
         * Returns the outer semi-axis of the ring. If they are
         * equal then the outer shape is a circle
         *
         * @return the outer semi-axis of the ring
         */
        [[nodiscard]] Vector2f getOuterSemiAxis(void) const noexcept;

        /**
         * Returns the inner semi-axis of the ring. If they are
         * equal then the inner shape is a circle
         *
         * @return the inner semi-axis of the ring
         */
        [[nodiscard]] Vector2f getInnerSemiAxis(void) const noexcept
            { return innerEllipse.getSemiAxis(); }

        /**
         * Returns the reference to the inner ellipse
         *
         * @return the reference to the inner ellipse
         */
        [[nodiscard]] InnerEllipse& getInnerEllipse(void) noexcept
            { return innerEllipse; }

        /**
         * Returns the constant reference to the inner ellipse
         *
         * @return the constant reference to the inner ellipse
         */
        [[nodiscard]] InnerEllipse const&
            getInnerEllipse(void) const noexcept
                { return innerEllipse; }

        /**
         * Checks whether a given point is located inside the ring
         *
         * @param position the point position [pixel position]
         * @return if the given point is inside the ring
         */
        [[nodiscard]] bool contains(
            Vector2f const& position) const noexcept /*final*/;

        /**
         * Destroy the Ring object
         */
        ~Ring(void) noexcept = default;
    private:
        InnerEllipse                                    innerEllipse;
        Matrix2f                                        outline;

        static const Executable                         shaderExec;

        /**
         * Actualizes the matrices responsible for the outline
         * and inline
         */
        void actualizeMatrices(void) noexcept final;

        /**
         * Sets the shader program uniforms
         */
        void setUniforms(void) const noexcept;

        /**
         * Returns whether a given point is inside local system
         * created by translation vector and transformation matrix
         *
         * @param position the point position
         * @param shift the translation vector
         * @param transform the transformation matrix
         * @return if point is inside the local system
         */
        [[nodiscard]] bool insideSystem(
            Vector2f const& position,
            Vector2f const& shift,
            Matrix2f const& transform) const noexcept;
    };

}

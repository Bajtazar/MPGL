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

#include "EllipticSprite.hpp"

namespace mpgl {

    /**
     * Represents a ring sprite
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class RingSprite : public EllipticSprite<IsColorable> {
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

            friend class RingSprite;
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
         * Construct a new ring sprite with given center,
         * semi-axis, texture and the inner ellipse. The
         * angle specifies a counterclockwise angle
         * between the x-axis and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param semiAxis the semi-axis of the outer ellipse
         * @param innerEllipse the inner ellipse
         * @param angle the angle between x-axis and semi-axis
         */
        explicit RingSprite(
            Texture const& texture = {},
            Vector2f const& center = {},
            Vector2f const& semiAxis = {},
            InnerEllipse const& innerEllipse = {},
            float32 angle = 0.f);

        /**
         * Construct a new ring sprite with given center,
         * color, semi-axis, texture and the inner ellipse.
         * The angle specifies a counterclockwise angle
         * between the x-axis and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param semiAxis the semi-axis of the outer ellipse
         * @param innerEllipse the inner ellipse
         * @param color the color of the vertices
         * @param angle the angle between x-axis and semi-axis
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            Vector2f const& semiAxis,
            InnerEllipse const& innerEllipse,
            Color const& color,
            float32 angle = 0.f) requires IsColorable;

        /**
         * Construct a new ring sprite with given center, texture,
         * outer and inner semi-axis. The angle specifies
         * a counterclockwise angle between the x-axis
         * and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param outerSemiAxis the semi-axis of the outer ellipse
         * @param innerSemiAxis the semi-axis of the inner ellipse
         * @param angle the angle between x-axis and semi-axis
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            Vector2f const& outerSemiAxis,
            Vector2f const& innerSemiAxis,
            float32 angle = 0.f);

        /**
         * Construct a new ring sprite with given center, texture,
         * color outer and inner semi-axis. The angle specifies
         * a counterclockwise angle between the x-axis
         * and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param outerSemiAxis the semi-axis of the outer ellipse
         * @param innerSemiAxis the semi-axis of the inner ellipse
         * @param color the color of the vertices
         * @param angle the angle between x-axis and semi-axis
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            Vector2f const& outerSemiAxis,
            Vector2f const& innerSemiAxis,
            Color const& color,
            float32 angle = 0.f) requires IsColorable;

        /**
         * Construct a new circular ring sprite with given texture,
         * center, radius and inner ellipse
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param radius the radius of the ring
         * @param innerEllipse the inner ellipse
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            float32 radius,
            InnerEllipse const& innerEllipse = {});

        /**
         * Construct a new circular ring sprite with given texture,
         * center, radius, color and inner ellipse
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param radius the radius of the ring
         * @param innerEllipse the inner ellipse
         * @param color the color of the vertices
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            float32 radius,
            InnerEllipse const& innerEllipse,
            Color const& color) requires IsColorable;

        /**
         * Construct a new circular ring sprite with given texture,
         * center, inner and outer radius
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param outerRadius the radius of the outer circle
         * @param innerRadius the radius of the inner circle
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            float32 outerRadius,
            float32 innerRadius);

        /**
         * Construct a new circular ring sprite with given texture,
         * center, color, inner and outer radius
         *
         * @param texture the sprite's texture
         * @param center the center of the ring
         * @param outerRadius the radius of the outer circle
         * @param innerRadius the radius of the inner circle
         * @param color the color of the vertices
         */
        explicit RingSprite(
            Texture const& texture,
            Vector2f const& center,
            float32 outerRadius,
            float32 innerRadius,
            Color const& color) requires IsColorable;

        RingSprite(RingSprite const& sprite) = default;
        RingSprite(RingSprite&& sprite) noexcept = default;

        RingSprite& operator=(RingSprite const& sprite) = default;
        RingSprite& operator=(RingSprite&& sprite) noexcept = default;

        /**
         * Draws the ring sprite on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Transforms the figure during the screen
         * transformation event
         *
         * @param oldDimensions the old screen dimensions
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept override final;

        /**
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        void translate(Vector2f const& shift) noexcept override final;

        /**
         * Scales the figure around given center by
         * a given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        void scale(
            Vector2f const& center,
            float32 factor) noexcept override final;

        /**
         * Rotates the figure around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        void rotate(
            Vector2f const& center,
            float32 angle) noexcept override final;

        /**
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        void rotate(
            Vector2f const& center,
            Matrix2f const& rotation) noexcept override final;

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
         * Destroy the Ring Sprite object
         */
        ~RingSprite(void) noexcept = default;
    private:
        InnerEllipse                                    innerEllipse;
        Matrix2f                                        outline;

        /**
         * Actualizes the matrices responsible for the outline
         */
        void actualizeMatrices(void) noexcept final;

        /**
         * Sets the shader program uniforms
         */
        void setUniforms(void) const noexcept;

        /**
         * Returns the shader name used by sprite implementation
         *
         * @return the shader name used by sprite implementation
         */
        static std::string shaderName(void) noexcept;
    };

    template class RingSprite<true>;
    template class RingSprite<false>;

    typedef RingSprite<true>                            ColorableRingSprite;
    typedef RingSprite<false>                           DefaultRingSprite;

}

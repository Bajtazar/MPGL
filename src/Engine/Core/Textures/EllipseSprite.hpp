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
     * Represents an ellipse sprite
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class EllipseSprite : public EllipticSprite<IsColorable> {
    public:
        /**
         * Construct a new ellipse sprite with given center,
         * color, semi-axis and texture. The angle specifies
         * a counterclockwise angle bewteen the x-axis
         * and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ellipse
         * @param semiAxis the semi-axis of the ellipse
         * @param angle the angle between x-axis and semi-axis
         */
        explicit EllipseSprite(
            Texture const& texture = {},
            Vector2f const& center = {},
            Vector2f const& semiAxis = {},
            float32 angle = 0.f);

        /**
         * Construct a new ellipse sprite with given center,
         * color, semi-axis, texture and vertices color.
         * The angle specifies a counterclockwise angle bewteen
         * the x-axis and x-semi-axis component
         *
         * @param texture the sprite's texture
         * @param center the center of the ellipse
         * @param semiAxis the semi-axis of the ellipse
         * @param angle the angle between x-axis and semi-axis
         * @param color the color of the ellipse's vertices
         */
        explicit EllipseSprite(
            Texture const& texture,
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle,
            Color const& color) requires IsColorable;

        /**
         * Construct a new circle sprite with given center,
         * radius and texture
         *
         * @param texture the sprite's texture
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @param color the color of the circle
         */
        explicit EllipseSprite(
            Texture const& texture,
            Vector2f const& center,
            float radius);

        /**
         * Construct a new circle sprite with given center,
         * radius, texture and vertices color
         *
         * @param texture the sprite's texture
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @param color the color of the circle's vertices
         */
        explicit EllipseSprite(
            Texture const& texture,
            Vector2f const& center,
            float radius,
            Color const& color) requires IsColorable;

        EllipseSprite(EllipseSprite const& sprite) = default;
        EllipseSprite(EllipseSprite&& sprite) noexcept = default;

        EllipseSprite& operator=(
            EllipseSprite const& sprite) = default;
        EllipseSprite& operator=(
            EllipseSprite&& sprite) noexcept = default;

        /**
         * Draws the ellipse sprite on the screen
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
         * Destroy the Ellipse Sprite object
         */
        ~EllipseSprite(void) noexcept = default;
    private:
        Matrix2f                                        outline;

        /**
         * Actualizes the matrices responsible for the outline
         */
        void actualizeMatrices(void) noexcept final;

        /**
         * Returns the shader name used by sprite implementation
         *
         * @return the shader name used by sprite implementation
         */
        static std::string shaderName(void) noexcept;
    };

    template class EllipseSprite<true>;
    template class EllipseSprite<false>;

    typedef EllipseSprite<true>                         ColorableEllipseSprite;
    typedef EllipseSprite<false>                        DefaultEllipseSprite;

}

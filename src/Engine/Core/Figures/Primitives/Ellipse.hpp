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
     * Represents an ellipse shape
     */
    class Ellipse : public Elliptic {
    public:
        /**
         * Construct a new ellipse with given center,
         * color and semi-axis. The angle specifies
         * a counterclockwise angle bewteen the x-axis
         * and x-semi-axis component
         *
         * @param center the center of the ellipse
         * @param semiAxis the semi-axis of the ellipse
         * @param color the color of the ellipse
         * @param angle the angle between x-axis and semi-axis
         */
        Ellipse(
            Vector2f const& center = {},
            Vector2f const& semiAxis = {},
            Color const& color = {},
            float32 angle = 0.f);

        /**
         * Construct a new circle with given center,
         * radius and color
         *
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @param color the color of the circle
         */
        Ellipse(
            Vector2f const& center,
            float radius,
            Color const& color = {});

        Ellipse(Ellipse const& ellipse) = default;
        Ellipse(Ellipse&& ellipse) noexcept = default;

        Ellipse& operator= (Ellipse const& ellipse) = default;
        Ellipse& operator= (Ellipse&& ellipse) noexcept = default;

        /**
         * Draws the circle on the screen
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
         * Returns the position of the center of the ellipse
         *
         * @return the position of the center of the ellipse
         */
        Vector2f getCenter(void) const noexcept;

        /**
         * Returns the semi-axis of the ellipse. If they are
         * equal then the ellipse is a circle
         *
         * @return the semi-axis of the ellipse
         */
        Vector2f getSemiAxis(void) const noexcept;

        /**
         * Destroy the Ellipse object
         */
        ~Ellipse(void) noexcept = default;
    private:
        Matrix2f                                    outlineTransform;

        /// Loads the shader's constant variables
        static const Executable                     shaderExec;

        /**
         * Actualizes the matrices responsible for the outline
         */
        void actualizeMatrices(void) noexcept final;
    };

}

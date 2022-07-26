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

#include <MPGL/Utility/Deferred/DelegatePointer.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Core/Figures/Elliptic.hpp>

namespace mpgl {

    /**
     * Represents an ellipse shape
     */
    class Ellipse : public Elliptic {
    public:
        /**
         * Constructs a new ellipse with given center,
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
         * Constructs a new circle with given center,
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
         * Transforms the ellipse during the screen
         * transformation event
         *
         * @param layout the layout of the ellipse
         * @param oldDimensions the old screen dimensions
         */
        void onScreenTransformation(
            Layout& layout,
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(
            Transformation2D const& transformator) noexcept final;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the shader program
         * object
         */
        void setShader(
            ShaderProgram const& program) noexcept override final;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the shader program
         * object
         */
        void setShader(
            ShaderProgram&& program) noexcept override final;

        /**
         * Sets the given shader program
         *
         * @param name the name of the shader program
         */
        void setShader(std::string const& name) override final;

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
         * Checks whether the given point is located inside the ellipse
         *
         * @param position the point position [pixel position]
         * @return if the given point is inside the ellipse
         */
        [[nodiscard]] bool contains(
            Vector2f const& position) const noexcept final;

        /**
         * Destroys the Ellipse object
         */
        ~Ellipse(void) noexcept = default;
    private:
        /**
         * Collection of all shader location's pointers
         */
        struct Locations {
            ShaderLocation                              color;
            ShaderLocation                              shift;
            ShaderLocation                              transform;
        };

        DelegatePointer<Locations>                      locations;
        Matrix2f                                        outlineTransform;

        /// Loads the shader's constant variables
        static const Executable                         shaderExec;

        /**
         * Sets the shader locations
         */
        void setLocations(void);

        /**
         * Actualizes the matrices responsible for the outline
         */
        void actualizeMatrices(void) noexcept final;
    };

}

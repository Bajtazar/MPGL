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

#include <MPGL/Core/Figures/Primitives/Helpers/EllipseHelpers.hpp>
#include <MPGL/Utility/Deferred/DelegatePointer.hpp>

namespace mpgl {

    /**
     * Represents an ellipse shape
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    class Ellipse : public Elliptic<Dim, Spec> {
    public:
        using VertexTraits = typename Elliptic<Dim, Spec>::VertexTraits;
        using Vector = typename VertexTraits::Vector;
        using Adapter = typename VertexTraits::Adapter;
        using ShaderManager = EllipseShader<Dim, Spec>;

        /**
         * Constructs a new 2D ellipse with given center,
         * color and semi-axis. The angle specifies
         * a counterclockwise angle bewteen the x-axis
         * and x-semi-axis component
         *
         * @param center the center of the 2D ellipse
         * @param semiAxis the semi-axis of the 2D ellipse
         * @param color the color of the 2D ellipse
         * @param angle the angle between x-axis and semi-axis
         */
        Ellipse(
            Vector2f const& center = {},
            Vector2f const& semiAxis = {},
            Color const& color = colors::white,
            float32 angle = 0.f) requires TwoDimensional<Dim>;

        /**
         * Constructs a new 2D circle with given center,
         * radius and color
         *
         * @param center the center of the 2D circle
         * @param radius the radius of the 2D circle
         * @param color the color of the 2D circle
         */
        Ellipse(
            Vector2f const& center,
            float radius,
            Color const& color = colors::white
            ) requires TwoDimensional<Dim>;

        /**
         * Constructs a new 2D ellipse object with given center
         * and with semiaxises lying on the designated vectors
         *
         * @throws NotPerpendicularException when the minor and
         * major axis are not perpendicular
         * @param center the center of the 3D ellipse
         * @param minorAxis the minor axise's vector
         * @param majorAxis the major axise's vector
         * @param color the color of the 3D ellipse
         */
        Ellipse(
            Vector3f const& center,
            Vector3f const& minorAxis,
            Vector3f const& majorAxis,
            Color const& color = colors::white
            ) requires ThreeDimensional<Dim>;

        Ellipse(Ellipse const& ellipse) = default;
        Ellipse(Ellipse&& ellipse) noexcept = default;

        Ellipse& operator= (Ellipse const& ellipse) = default;
        Ellipse& operator= (Ellipse&& ellipse) noexcept = default;

        /**
         * Draws the circle on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the shader program
         * object
         */
        virtual void setShader(
            ShaderProgram const& program) noexcept override;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the shader program
         * object
         */
        virtual void setShader(
            ShaderProgram&& program) noexcept override;

        /**
         * Sets the given shader program
         *
         * @param name the name of the shader program
         */
        virtual void setShader(std::string const& name) override;

        /**
         * Returns the position of the center of the ellipse
         *
         * @return the position of the center of the ellipse
         */
        [[nodiscard]] Vector getCenter(void) const noexcept;

        /**
         * Returns the semi-axis of the ellipse. If they are
         * equal then the ellipse is a circle
         *
         * @return the semi-axis of the ellipse
         */
        [[nodiscard]] Vector2f getSemiAxis(void) const noexcept;

        /**
         * Checks whether the given pixel is located inside the ellipse
         *
         * @param position the point position [pixel position]
         * @return if the given point is inside the ellipse
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept;

        /**
         * Virtual destructor. Destroys the Ellipse object
         */
        virtual ~Ellipse(void) noexcept = default;

        friend class EllipseClickChecker<Dim, Spec>;
        friend class EllipseOutlineCalculator<Dim, Spec>;
    private:
        using OutlineCalculator = EllipseOutlineCalculator<Dim, Spec>;
        using Clicker = EllipseClickChecker<Dim, Spec>;
        using MatrixT = typename OutlineCalculator::MatrixT;

        /**
         * Collection of all shader location's pointers
         */
        struct Locations {
            ShaderLocation                              color;
            ShaderLocation                              shift;
            ShaderLocation                              transform;
        };

        DelegatePointer<Locations>                      locations;
        MatrixT                                         outlineTransform;

        static OutlineCalculator const                  outlineCalc;
        static Clicker const                            clicker;
        static ShaderManager const                      shaderManager;

        /**
         * Sets the shader locations
         */
        void setLocations(void);

        /**
         * Actualizes the matrices responsible for the outline
         */
        void actualizeMatrices(void) noexcept final;

        /**
         * Actualizes shader's locations before draw
         */
        void actualizeLocations(void) const noexcept override final;
    };

    template class Ellipse<dim::Dim2>;
    template class Ellipse<dim::Dim3>;
    template class Ellipse<dim::Dim2, uint8>;
    template class Ellipse<dim::Dim3, uint8>;

    typedef Ellipse<dim::Dim2>                      Ellipse2D;
    typedef Ellipse<dim::Dim3>                      Ellipse3D;

}

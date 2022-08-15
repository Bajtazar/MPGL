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

#include <MPGL/Core/Figures/Primitives/Helpers/RingHelpers.hpp>
#include <MPGL/Utility/Deferred/DelegatePointer.hpp>

namespace mpgl {

    /**
     * Represents the ring on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    class Ring : public Elliptic<Dim, Spec> {
    public:
        using VertexTraits = typename Elliptic<Dim, Spec>::VertexTraits;
        using Vector = typename VertexTraits::Vector;
        using Adapter = typename VertexTraits::Adapter;

        /**
         * Represents the ellipse describing inner outline
         * of the ring
         */
        class InnerEllipse : public Transformable<Dim> {
        public:
            /**
             * Construct a new 2D ellipse with given center
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
                float32 angle = 0.f) requires TwoDimensional<Dim>;

            /**
             * Construct a new 2D circle with given center and
             * radius
             *
             * @param center the center of the circle
             * @param radius the radius of the circle
             */
            InnerEllipse(
                Vector2f const& center,
                float radius) requires TwoDimensional<Dim>;

            /**
             * Construct a new 3D ellipse with given center
             * and semi-axises
             *
             * @throws NotPerpendicularException when the minor and
             * major axis are not perpendicular
             * @param center the center of the 3D ellipse
             * @param minorAxis the minor axise's vector
             * @param majorAxis the major axise's vector
             */
            InnerEllipse(
                Vector3f const& center,
                Vector3f const& minorAxis,
                Vector3f const& majorAxis
                ) requires ThreeDimensional<Dim>;

            InnerEllipse(InnerEllipse const& ellipse) = default;
            InnerEllipse(InnerEllipse&& ellipse) noexcept = default;

            InnerEllipse& operator=(
                InnerEllipse const& ellipse) = default;
            InnerEllipse& operator=(
                InnerEllipse&& ellipse) noexcept = default;

            /**
             * Performs transformation on the figure
             *
             * @param transformator the constant reference to the
             * transforming object
             */
            void transform(
                Transformation<Dim> const& transformator) noexcept final;

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
             * Destroy the Inner Ellipse object
             */
            ~InnerEllipse(void) noexcept = default;

            friend class Ring;
            friend class RingClickChecker<Dim, Spec>;
            friend class RingOutlineCalculator<Dim, Spec>;
        private:
            using OutlineCalculator = RingOutlineCalculator<Dim, Spec>;
            using MatrixT = typename OutlineCalculator::MatrixT;
            using Vertex = Adapter;
            using Vertices = std::array<Vertex, 4>;

            Vertices                                        vertices;
            MatrixT                                         outline;

            /**
             * Constructs the vector with ellipse vertices
             *
             * @param center the center of the ellipse
             * @param semiAxis the semi-axis of the ellipse
             * @param angle the angle between x-axis and semi-axis
             * @return the ellipse's vertices
             */
            static Vertices ellipseVertices(
                Vector2f const& center,
                Vector2f const& semiAxis,
                float32 angle) requires TwoDimensional<Dim>;

            /**
             * Constructs the vector with circle vertices
             *
             * @param center the center of the circle
             * @param radius the radius of the circle
             * @return the circle's vertices
             */
            static Vertices circleVertices(
                Vector2f const& center,
                float32 radius) requires TwoDimensional<Dim>;

            /**
             * Actualizes the matrices responsible for the outline
             */
            void actualizeMatrices(void) noexcept;
        };

        /**
         * Construct a new 2D ring with given center,
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
            float32 angle = 0.f) requires TwoDimensional<Dim>;

        /**
         * Construct a new 2D ring with given center,
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
            float32 angle = 0.f) requires TwoDimensional<Dim>;

        /**
         * Construct a new circular 2D ring with given center,
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
            Color const& color = {}) requires TwoDimensional<Dim>;

        /**
         * Construct a new circular 2D ring with given center,
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
            Color const& color = {}) requires TwoDimensional<Dim>;

        /**
         * Construct a new circular 3D ring with given center,
         * semi-axies, color and inner ellipse
         *
         * @throws NotPerpendicularException when the minor and
         * major axis are not perpendicular
         * @throws DifferentPlanesException when the inner and outer
         * ring planes are different
         * @param center the center of the ring
         * @param minorAxis the minor axise's vector
         * @param majorAxis the major axise's vector
         * @param innerEllipse the inner ellipse
         * @param color the color of the ring
         */
        Ring(
            Vector3f const& center,
            Vector3f const& minorAxis,
            Vector3f const& majorAxis,
            InnerEllipse const& innerEllipse,
            Color const& color = {}) requires ThreeDimensional<Dim>;

        Ring(Ring const& ring) = default;
        Ring(Ring&& ring) noexcept = default;

        Ring& operator=(Ring const& ring) = default;
        Ring& operator=(Ring&& ring) noexcept = default;

        /**
         * Draws the ring on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(
            Transformation<Dim> const& transformator) noexcept final;

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
         * Returns the center of the ring
         *
         * @return the center of the ring
         */
        [[nodiscard]] Vector getCenter(void) const noexcept;

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
         * Checks whether the given pixel is located inside the ring
         *
         * @param position the pixel's position
         * @return if the given point is inside the ring
         */
        [[nodiscard]] bool contains(
            Vector2u const& position) const noexcept final;

        /**
         * Destroy the Ring object
         */
        ~Ring(void) noexcept = default;

        friend class RingClickChecker<Dim, Spec>;
        friend class RingOutlineCalculator<Dim, Spec>;
    private:
        using OutlineCalculator = RingOutlineCalculator<Dim, Spec>;
        using Clicker = RingClickChecker<Dim, Spec>;
        using ShaderManager = RingShader<Dim, Spec>;
        using MatrixT = typename OutlineCalculator::MatrixT;

        /**
         * Collection of all shader location's pointers
         */
        struct Locations {
            ShaderLocation                              color;
            ShaderLocation                              outerShift;
            ShaderLocation                              innerShift;
            ShaderLocation                              outerTransform;
            ShaderLocation                              innerTransform;
        };

        DelegatePointer<Locations>                      locations;
        InnerEllipse                                    innerEllipse;
        MatrixT                                         outline;

        static OutlineCalculator const                  outlineCalc;
        static Clicker const                            clicker;
        static ShaderManager const                      shaderManager;

        /**
         * Checks whether the inner and outer ring lies on the same
         * plane
         *
         * @throws DifferentPlanesException if planes are different
         */
        void checkInnerAndOuterPlanes(
            void) const requires ThreeDimensional<Dim>;

        /**
         * Sets the shader locations
         */
        void setLocations(void);

        /**
         * Actualizes the matrices responsible for the outline
         * and inline
         */
        void actualizeMatrices(void) noexcept final;

        /**
         * Actualizes shader's locations before draw
         */
        void actualizeLocations(void) const noexcept override final;
    };

    template class Ring<dim::Dim2>;
    template class Ring<dim::Dim3>;
    template class Ring<dim::Dim2, int32>;
    template class Ring<dim::Dim3, int32>;

    typedef Ring<dim::Dim2>                         Ring2D;
    typedef Ring<dim::Dim3>                         Ring3D;
    typedef Ring<dim::Dim2, int32>                  ColorableRing2D;
    typedef Ring<dim::Dim3, int32>                  ColorableRing3D;

}

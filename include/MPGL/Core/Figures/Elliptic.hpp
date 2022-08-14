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

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>
#include <MPGL/Core/Figures/EllipticVertices.hpp>
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    /**
     * Base class for all elliptic shapes
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <
        Dimension Dim,
        EllipticTraitSpecifier<Dim> Specifier = void>
    class Elliptic :
        public virtual Clickable,
        public Figure<Dim>
    {
    public:
        using VertexTraits = EllipticVertices<Dim, Specifier>;
        /// The vertex used by all elliptic shapes
        using Vertex = typename VertexTraits::Vertex;
        using Vertices = std::vector<Vertex>;

        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Checks whether the given pixel is located
         * inside of the figure [boundry is concidered
         * as a part of the figure, the 3D figures are
         * projected onto screen and then checked]
         *
         * @param position the pixel's position
         * @return if point is inside figure
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept = 0;

        /**
         * Sets the elliptic color
         *
         * @param color the color of the elliptic
         */
        void setColor(Color const& color = {}) noexcept
            { this->color = color; }

        /**
         * Returns the constant reference to the elliptic color
         *
         * @return the constant reference to the elliptic color
         */
        [[nodiscard]] Color const& getColor(void) const noexcept
            { return color; }

        /**
         * Virtual destructor. Destroy the Elliptic object
         */
        virtual ~Elliptic(void) noexcept = default;
    protected:
        using Executable = typename Shadeable::Executable;

        /**
         * Constructs a new Elliptic object with given
         * vector of vertices, color, program name and
         * the executable
         *
         * @param vertices the vector of vertices
         * @param programName the shader program name
         * @param exec the shader's executible
         * @param color the ellipse's color
         */
        explicit Elliptic(
            Vertices vertices,
            std::string const& programName,
            Executable exec,
            Color const& color);

        /**
         * Constructs a new Elliptic object from the given
         * constant reference
         *
         * @param shape the given elliptic constant reference
         */
        Elliptic(Elliptic const& shape);

        Elliptic(Elliptic&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given elliptic constant reference
         * @return the reference to this object
         */
        Elliptic& operator=(Elliptic const& shape);

        Elliptic& operator=(Elliptic&& shape) noexcept = default;

        typedef std::array<uint32, 6>                   Indexes;

        ElementArrayBuffer                              elementBuffer;
        Color                                           color;
        Vertices                                        vertices;

        /**
         * Actualizes vertex buffer before draw
         */
        virtual void actualizeBufferBeforeDraw(void) const noexcept;

        /**
         * Actualizes shader's locations before draw
         */
        virtual void actualizeLocations(void) const noexcept;

        /**
         * Pure virtual method. Has to be overloaded. Actualizes
         * the matrices responsible for the elliptic shape
         */
        virtual void actualizeMatrices(void) noexcept = 0;

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
    private:
        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

        static constexpr const Indexes                  indexes {
            0, 1, 2, 0, 3, 2};
    };

    template class Elliptic<dim::Dim2, void>;
    template class Elliptic<dim::Dim2, int32>;
    template class Elliptic<dim::Dim3, void>;
    template class Elliptic<dim::Dim3, int32>;

    typedef Elliptic<dim::Dim2, void>                   Elliptic2D;
    typedef Elliptic<dim::Dim2, int32>                  ColorableElliptic2D;
    typedef Elliptic<dim::Dim3, void>                   Elliptic3D;
    typedef Elliptic<dim::Dim3, int32>                  ColorableElliptic3D;

}

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
#include <MPGL/Iterators/AccessRegisteringIterator.hpp>
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
    private:
        constexpr static bool Iterable = !std::same_as<
            typename VertexTraits::IterableFields, NullTSHolderT>;

        using acr_iterator = AccessRegisteringIterator<
            typename Vertices::iterator>;
        using racr_iterator = AccessRegisteringIterator<
            typename Vertices::reverse_iterator>;
    public:
        using iterator = VertexViewIterFromHolderT<
            acr_iterator, typename VertexTraits::IterableFields>;

        using const_iterator = VertexViewIterFromHolderT<
            typename Vertices::const_iterator,
            typename VertexTraits::IterableFields>;

        using reverse_iterator = VertexViewIterFromHolderT<
            racr_iterator, typename VertexTraits::IterableFields>;

        using const_reverse_iterator = VertexViewIterFromHolderT<
            typename Vertices::const_reverse_iterator,
            typename VertexTraits::IterableFields>;

        using vertex_view = typename iterator::value_type;
        using const_vertex_view = typename const_iterator::value_type;

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
         * Returns a vertex view to the vertex with
         * given index
         *
         * @param index the vertex's index
         * @return the view to a desired vertex
         */
        [[nodiscard]] vertex_view operator[] (
            std::size_t index) noexcept requires Iterable;

        /**
         * Returns a constant vertex view to the vertex with
         * given index
         *
         * @param index the vertex's index
         * @return the constant view to a desired vertex
         */
        [[nodiscard]] const_vertex_view operator[] (
            std::size_t index) const noexcept requires Iterable;

        /**
         * Returns a vertex view to the first vertex
         *
         * @return the view to the first vertex
         */
        [[nodiscard]] vertex_view front(
            void) noexcept requires Iterable;

        /**
         * Returns a constant vertex view to the first vertex
         *
         * @return the constant view to the first vertex
         */
        [[nodiscard]] const_vertex_view front(
            void) const noexcept requires Iterable;

        /**
         * Returns a vertex view to the last vertex
         *
         * @return the view to the last vertex
         */
        [[nodiscard]] vertex_view back(
            void) noexcept requires Iterable;

        /**
         * Returns a constant vertex view to the last vertex
         *
         * @return the constant view to the last vertex
         */
        [[nodiscard]] const_vertex_view back(
            void) const noexcept requires Iterable;

        /**
         * Returns a vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] iterator begin(void) noexcept requires Iterable;

        /**
         * Returns a vertex view iterator to the end of
         * the vertices vector
         *
         * @return the vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] iterator end(void) noexcept requires Iterable;

        /**
         * Returns a constant vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] const_iterator begin(
            void) const noexcept requires Iterable;

        /**
         * Returns a constant vertex view iterator to the end of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] const_iterator end(
            void) const noexcept requires Iterable;

        /**
         * Returns a constant vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] const_iterator cbegin(
            void) const noexcept requires Iterable;

        /**
         * Returns a constant vertex view iterator to the end of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] const_iterator cend(
            void) const noexcept requires Iterable;

        /**
         * Returns a reverse vertex view iterator to the end of
         * the vertices vector
         *
         * @return the reverse vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] reverse_iterator rbegin(
            void) noexcept requires Iterable;

        /**
         * Returns a reverse vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the reverse vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] reverse_iterator rend(
            void) noexcept requires Iterable;

        /**
         * Returns a constant reverse vertex view iterator to
         * the end of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the end of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept requires Iterable;

        /**
         * Returns a constant reverse vertex view iterator to
         * the begin of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the begin of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept requires Iterable;

        /**
         * Returns a constant reverse vertex view iterator to
         * the end of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the end of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept requires Iterable;

        /**
         * Returns a constant reverse vertex view iterator to
         * the begin of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the begin of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept requires Iterable;

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
         */
        explicit Elliptic(
            Vertices vertices,
            std::string const& programName,
            Executable exec);

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

        typedef std::array<uint32, 6>                   Indices;

        ElementArrayBuffer                              elementBuffer;
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
         * @param color the vertices' color
         * @return the ellipse's vertices
         */
        static Vertices ellipseVertices(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle,
            Color const& color) requires TwoDimensional<Dim>;

        /**
         * Constructs the vector with circle vertices
         *
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @param color the vertices' color
         * @return the circle's vertices
         */
        static Vertices circleVertices(
            Vector2f const& center,
            float32 radius,
            Color const& color) requires TwoDimensional<Dim>;
    private:
        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

        static constexpr const Indices                  indices {
            0, 1, 2, 0, 3, 2};
    };

    template class Elliptic<dim::Dim2, void>;
    template class Elliptic<dim::Dim3, void>;
    template class Elliptic<dim::Dim2, uint8>;
    template class Elliptic<dim::Dim3, uint8>;

    typedef Elliptic<dim::Dim2, void>                   Elliptic2D;
    typedef Elliptic<dim::Dim3, void>                   Elliptic3D;

}

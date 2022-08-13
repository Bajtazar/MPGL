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

#include <MPGL/Core/Figures/AngularVertices.hpp>
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    /**
     * Base class for all angular shapes
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <
        Dimension Dim,
        AngularTraitSpecifier<Dim> Specifier = void>
    class Angular : public Figure<Dim> {
    public:
        using VertexTraits = AngluarVertices<Dim, Specifier>;
        /// The vertex used by all angular shapes
        using Vertex = typename VertexTraits::Vertex;
        using Vertices = std::vector<Vertex>;

        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(
            Transformation<Dim> const& transformator) noexcept final;

        /**
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept;

        /**
         * Returns the constant reference to vertex with
         * the given index
         *
         * @param index the vertex's index
         * @return the constant reference to vertex with
         * the given index
         */
        [[nodiscard]] Vertex const&
            operator[] (std::size_t index) const noexcept;

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        [[nodiscard]] Vertex& front(void) noexcept;

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        [[nodiscard]] Vertex const& front(void) const noexcept;

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        [[nodiscard]] Vertex& back(void) noexcept;

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        [[nodiscard]] Vertex const& back(void) const noexcept;

        using iterator                    = Vertices::iterator;
        using const_iterator              = Vertices::const_iterator;
        using reverse_iterator
            = Vertices::reverse_iterator;
        using const_reverse_iterator
            = Vertices::const_reverse_iterator;

        /**
         * Returns the number of vertices in the object
         *
         * @return the number of vertices
         */
        [[nodiscard]] std::size_t size(void) const noexcept
            { return vertices.size(); }

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept;

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept;

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept;

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rend(void) const noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept;

        /**
         * Virtual destructor. Destroy the Angular object
         */
        virtual ~Angular(void) noexcept = default;
    protected:
        /**
         * Construct a new Angular object. Creates the
         * vertices vector of the given size with the given color
         *
         * @param size the size of the vertices vector
         * @param color the color of the vertices
         */
        explicit Angular(size_t size, Color const& color);

        /**
         * Construct a new Angular object with given
         * vector of vertices
         *
         * @param vertices the vector of vertices
         */
        explicit Angular(Vertices vertices);

        /**
         * Construct a new Angular object from the given
         * constant reference
         *
         * @param shape the given angular constant reference
         */
        Angular(Angular const& shape);

        Angular(Angular&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given angular constant reference
         * @return the reference to this object
         */
        Angular& operator=(Angular const& shape);

        Angular& operator=(Angular&& shape) noexcept = default;

        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

        /**
         * Actualizes vertex buffer before draw
         */
        virtual void actualizeBufferBeforeDraw(void) const noexcept;

        Vertices                            vertices;
    };

    template class Angular<dim::Dim2>;
    template class Angular<dim::Dim3>;

    typedef Angular<dim::Dim2>              Angular2D;
    typedef Angular<dim::Dim3>              Angular3D;

}

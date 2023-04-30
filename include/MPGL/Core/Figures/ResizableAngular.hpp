/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Core/Figures/Angular.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

#include <optional>

namespace mpgl {

    /**
     * Base class for all resizable angular shapes
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class ResizableAngular :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public Angular<Dim, Spec>
    {
    public:
        using VertexTraits = typename Angular<Dim, Spec>::VertexTraits;
        using Vertex = typename VertexTraits::Vertex;
        using Vector = typename VertexTraits::Vector;
        using Optional = std::optional<Vector>;
        using size_type = std::size_t;
        using iterator = typename Angular<Dim, Spec>::iterator;
        using const_iterator = typename Angular<Dim, Spec>::const_iterator;

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
         * Changes size of the vertices array
         *
         * @param size the new size of the vertices array
         */
        void resize(size_type size);

        /**
         * Reserves new space for the vertices in the vertices
         * array
         *
         * @param size the reserved size
         */
        void reserve(size_type size);

        /**
         * Removes unused vertice's storage capacity
         */
        void shrinkToFit(void);

        /**
         * Returns vertice's storage capacity
         *
         * @return vertice's storage capacity
         */
        [[nodiscard]] std::size_t capacity(void) const noexcept;

        /**
         * Returns whether the vertice's storage is empty
         *
         * @return if the vertice's storage is empty
         */
        [[nodiscard]] bool empty(void) const noexcept;

        /**
         * Pushes a new vertex into the vertices array
         *
         * @param vertex the new vertex
         */
        void push(Vertex vertex);

        /**
         * Emplaces a new vertex into the vertices array
         *
         * @param position the position of the vertex
         * @param color the color of the vertex
         */
        void emplace(
            Vector const& position,
            Color const& color);

        /**
         * Emplaces a new vertex into the vertices array
         *
         * @tparam Args the types of arguments
         * @param args the vertex constructor's arguments
         */
        template <typename... Args>
            requires std::constructible_from<Vertex, Args...>
        void emplace(Args&&... args);

        /**
         * Removes last vertex from the vertices array
         */
        void pop(void) noexcept;

        /**
         * Removes the vertex lying under the given position
         *
         * @param position a constant reference to the iterator
         * indicating the removed vertex
         */
        void erase(iterator const& position);

        /**
         * Removes the vertex lying under the given position
         *
         * @param position a constant reference to the constant iterator
         * indicating the removed vertex
         */
        void erase(const_iterator const& position);

        /**
         * Removes the vertices lying under the given range indicated
         * by the iterators
         *
         * @param first a constant reference to the iterator
         * indicating the begining of the removed range
         * @param last a constant reference to the iterator
         * indicating the end of the removed range
         */
        void erase(
            iterator const& first,
            iterator const& last);

        /**
         * Removes the vertices lying under the given range indicated
         * by the constant iterators
         *
         * @param first a constant reference to the constant iterator
         * indicating the begining of the removed range
         * @param last a constant reference to the constant iterator
         * indicating the end of the removed range
         */
        void erase(
            const_iterator const& first,
            const_iterator const& last);

        /**
         * Returns the center of the angular
         *
         * @return the vector containing center
         */
        [[nodiscard]] Optional getCenter(void) const noexcept;

        /**
         * Virtual Destructor. Destroys the Resizable Angular object
         */
        virtual ~ResizableAngular(void) noexcept = default;
    protected:
        using Vertices = typename Angular<Dim, Spec>::Vertices;

        /**
         * Construct a new Resizable Angular object with the given
         * size vertices array
         *
         * @param size the size of the vertices array
         * @param color the color of the vertices
         */
        explicit ResizableAngular(size_t size, Color const& color);

        /**
         * Construct a new Resizable Angular object with the given
         * vertices array
         *
         * @param vertices the vertices array
         */
        explicit ResizableAngular(Vertices vertices);

        ResizableAngular(
            ResizableAngular const& shape) noexcept = default;
        ResizableAngular(
            ResizableAngular&& shape) noexcept = default;

        /**
         * Construct a new Resizable Angular object from the given
         * vertices positions and their common color
         *
         * @tparam ColorTp the type of the color vector
         * @tparam Args the parameter pack of vectors
         * @param color the color of the vertices
         * @param positions the positions of the vertices
         */
        template <class ColorTp, AllConvertible<Vector>... Args>
            requires std::constructible_from<Color, ColorTp>
        explicit ResizableAngular(
            ColorTp&& color,
            Args&&... positions);

        /**
         * Construct a new Resizable Angular object from the given
         * vertices positions
         *
         * @tparam Args the parameter pack of vectors
         * @param positions the positions of the vertices
         */
        template <AllConvertible<Vector>... Args>
        explicit ResizableAngular(Args&&... positions);

        ResizableAngular& operator= (
            ResizableAngular&& shape) noexcept = default;

        /**
         * Assigns the given resizable angular to this object
         *
         * @param shape the assigned resizable angular
         * @return the reference to this object
         */
        ResizableAngular& operator= (
            ResizableAngular const& shape);

        /**
         * Actualizes vertex buffer before draw. Overloads
         * the Angular implementation
         */
        void actualizeBufferBeforeDraw(
            void) const noexcept override final;

        bool mutable                                isExtended = false;
    };

    extern template class ResizableAngular<dim::Dim2>;
    extern template class ResizableAngular<dim::Dim3>;
    extern template class ResizableAngular<dim::Dim2, uint8>;
    extern template class ResizableAngular<dim::Dim3, uint8>;

    typedef ResizableAngular<dim::Dim2>             ResizableAngular2D;
    typedef ResizableAngular<dim::Dim3>             ResizableAngular3D;

}

#include <MPGL/Core/Figures/ResizableAngular.tpp>

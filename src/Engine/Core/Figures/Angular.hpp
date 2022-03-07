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

#include "Figure.hpp"
#include "../Color.hpp"
#include "../../Utility/Adapter.hpp"
#include "../Context/Buffers/Vertex.hpp"

namespace mpgl {

    /**
     * Base class for all angular shapes
     */
    class Angular : public Figure {
    public:
        /// The vertex used by all angular shapes
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        typedef std::vector<Vertex>         Vertices;

        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

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
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        Vertex& operator[] (std::size_t index) noexcept
            { isModified = true; return vertices[index]; }

        /**
         * Returns the constant reference to vertex with
         * the given index
         *
         * @param index the vertex's index
         * @return the constant reference to vertex with
         * the given index
         */
        Vertex const& operator[] (std::size_t index) const noexcept
            { return vertices[index]; }

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        Vertex& front(void) noexcept
            { isModified = true; return vertices.front(); }

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        Vertex const& front(void) const noexcept
            { return vertices.front(); }

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        Vertex& back(void) noexcept
            { isModified = true; return vertices.back(); }

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        Vertex const& back(void) const noexcept
            { return vertices.back(); }

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
        std::size_t size(void) const noexcept
            { return vertices.size(); }

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        iterator begin(void) noexcept
            { isModified = true; return vertices.begin(); }

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        iterator end(void) noexcept
            { isModified = true; return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        const_iterator begin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        const_iterator end(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        const_iterator cbegin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        const_iterator cend(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        reverse_iterator rbegin(void) noexcept
            { isModified = true; return vertices.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        reverse_iterator rend(void) noexcept
            { isModified = true; return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        const_reverse_iterator rbegin(void) const noexcept
            { return vertices.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        const_reverse_iterator rend(void) const noexcept
            { return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        const_reverse_iterator crbegin(void) const noexcept
            { return vertices.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        const_reverse_iterator crend(void) const noexcept
            { return vertices.crend(); }

        /**
         * Virtual destructor. Destroy the Angular object
         */
        virtual ~Angular(void) noexcept = default;
    protected:
        /**
         * Construct a new Angular object. Creates the
         * vertices vector of a given size with a given color
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
         * Construct a new Angular object from a given
         * constant reference
         *
         * @param shape the given angular constant reference
         */
        explicit Angular(Angular const& shape);

        explicit Angular(Angular&& shape) noexcept = default;

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
        void actualizeBufferBeforeDraw(void) const noexcept;

        Vertices                            vertices;
    };

}

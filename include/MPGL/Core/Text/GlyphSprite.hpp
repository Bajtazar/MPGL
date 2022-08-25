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
#include <MPGL/Core/Textures/Texturable.hpp>
#include <MPGL/Core/Vertex/Vertex.hpp>
#include <MPGL/Core/Shape.hpp>

namespace mpgl {

    /**
     * Represents the lightweight sprite dependent from
     * external commands in order to gain performance boost.
     * Cannot be drawn alone
     *
     * @tparam Dim the dimension of the space where the glyph is
     * being drawn
     */
    template <Dimension Dim>
    class GlyphSprite :
        public Texturable<Dim>,
        public Shape<Dim>
    {
    private:
        static constexpr std::size_t Degree
            = Dim::orthogonal_space_degree;
    public:
        using Vector = mpgl::Vector<float32, Degree>;
        using Adapter = mpgl::Adapter<Degree>;
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
        using Vertices = std::vector<Vertex>;

        /**
         * Constructs a new Glyph Sprite object with given
         * texture
         *
         * @param texture the texture drawn by sprite
         * @param color sprite's color
         */
        GlyphSprite(
            Texture const& texture = {},
            Color const& color = {});

        /**
         * Constructs a new parallelogram-like Glyph Sprite
         * object from the given three vertices, sprite's
         * texture and its color
         *
         * @param texture the texture drawn by sprite
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex position
         * @param color the sprite's color
         */
        GlyphSprite(
            Texture const& texture,
            Vector const& firstVertex,
            Vector const& secondVertex,
            Vector const& thirdVertex,
            Color const& color);

        /**
         * Construct a new rectangular Glyph Sprite object.
         * The edges are parallel to x and y axis. Colors
         * sprite with given color
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param dimensions the sprite's dimensions
         * @param color the sprite's color
         */
        GlyphSprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& dimensions,
            Color const& color) requires TwoDimensional<Dim>;

        /**
         * Construct a new Glyph Sprite object from the given
         * constant reference to the glyph sprite object
         *
         * @param sprite the constant reference to the glyph
         * sprite object
         */
        GlyphSprite(GlyphSprite const& sprite);

        GlyphSprite(GlyphSprite&& sprite) noexcept = default;

        /**
         * Assigns the given glyph sprite to this object
         *
         * @param sprite the constant reference to sprite object
         * @return the reference to this object
         */
        GlyphSprite& operator= (GlyphSprite const& sprite);

        GlyphSprite& operator= (
            GlyphSprite&& sprite) noexcept = default;

        /**
         * Draws glyph sprite on the screen
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
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept
            { this->isModified = true; return vertices[index]; }

        /**
         * Returns the constant reference to vertex with
         * the given index
         *
         * @param index the vertex's index
         * @return the constant reference to vertex with
         * the given index
         */
        [[nodiscard]] Vertex const& operator[] (
            std::size_t index) const noexcept
                { return vertices[index]; }

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        [[nodiscard]] Vertex& front(void) noexcept
            { this->isModified = true; return vertices.front(); }

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        [[nodiscard]] Vertex const& front(void) const noexcept
            { return vertices.front(); }

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        [[nodiscard]] Vertex& back(void) noexcept
            { this->isModified = true; return vertices.back(); }

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        [[nodiscard]] Vertex const& back(void) const noexcept
            { return vertices.back(); }

        using iterator = AccessRegisteringIterator<
            typename Vertices::iterator>;
        using const_iterator = Vertices::const_iterator;
        using reverse_iterator = AccessRegisteringIterator<
            typename Vertices::reverse_iterator>;
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
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept;

        /**
         * Sets the sprite's color
         *
         * @param color the sprite's new color
         */
        void setColor(Color const& color = {}) noexcept;

        /**
         * Destroys the Glyph Sprite object
         */
        ~GlyphSprite(void) noexcept = default;
    private:
        using Positions = std::array<Vector, 4>;
        using Indices = std::array<uint32, 6>;

        Vertices                                        vertices;
        ElementArrayBuffer                              elementBuffer;

        /**
         * Actualizes buffers before draw
         */
        void actualizeBufferBeforeDraw(void) const noexcept;

        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

        /**
         * Creates the four vertices vector with the correct
         * texture positions and with the given color
         *
         * @param color the color of the vertices
         * @param positions the vertices positions
         * @return the vertices vector
         */
        static Vertices makeVertices(
            Color const& color,
            Positions const& positions = {});

        static constexpr const Indices                  indices {
            0, 1, 2, 0, 3, 2};
    };

    template class GlyphSprite<dim::Dim2>;
    template class GlyphSprite<dim::Dim3>;

    using GlyphSprite2D = GlyphSprite<dim::Dim2>;
    using GlyphSprite3D = GlyphSprite<dim::Dim3>;

}

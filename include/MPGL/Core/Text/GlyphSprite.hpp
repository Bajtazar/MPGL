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

#include <MPGL/Core/Textures/Texturable.hpp>

namespace mpgl {

    /**
     * Represents the lightweight sprite dependent from
     * external commands in order to gain performance boost.
     * Cannot be drawn alone
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class GlyphSprite : public Texturable {
    private:
        /// The default texturable vertex
        using DefaultVertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>
        >;

        /// The colorable texturable vertex
        using ColorableVertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    public:
        using Vertex = std::conditional_t<IsColorable, ColorableVertex,
            DefaultVertex>;

        typedef std::vector<Vertex>                     Vertices;
        /**
         * Construct a new Glyph Sprite object with given
         * texture
         *
         * @param texture the texture drawn by sprite
         */
        GlyphSprite(Texture const& texture = {});

        /**
         * Construct a new Glyph Sprite object with given
         * texture and color
         *
         * @param texture the texture drawn by sprite
         * @param color sprite's color
         */
        GlyphSprite(Texture const& texture,
            Color const& color) requires (IsColorable);

        /**
         * Construct a new parallelogram-like Glyph Sprite
         * object from the given three vertices and sprite's
         * texture
         *
         * @param texture the texture drawn by sprite
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex position
         */
        GlyphSprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex);

        /**
         * Construct a new parallelogram-like Glyph Sprite
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
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex,
            Color const& color) requires (IsColorable);

        /**
         * @brief Construct a new rectangular Glyph Sprite
         * object. The edges are parallel to x and y axis
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param dimensions the sprite's dimensions
         */
        GlyphSprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& dimensions);

        /**
         * @brief Construct a new rectangular Glyph Sprite
         * object. The edges are parallel to x and y axis.
         * Colors sprite with given color
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
            Color const& color) requires (IsColorable);

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
         * Draws sprite on the screen
         */
        void draw(void) const noexcept;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation2D const& transformator) noexcept;

        /**
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept
            { isModified = true; return vertices[index]; }

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
            { isModified = true; return vertices.front(); }

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
            { isModified = true; return vertices.back(); }

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        [[nodiscard]] Vertex const& back(void) const noexcept
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
        [[nodiscard]] std::size_t size(void) const noexcept
            { return vertices.size(); }

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept
            { isModified = true; return vertices.begin(); }

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept
            { isModified = true; return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { isModified = true; return vertices.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { isModified = true; return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return vertices.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return vertices.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return vertices.crend(); }

        /**
         * Sets the sprite's color
         *
         * @param color the sprite's new color
         */
        void setColor(Color const& color = {}) noexcept
            requires (IsColorable);

        /**
         * Destroy the Glyph Sprite object
         */
        ~GlyphSprite(void) noexcept = default;
    private:
        Vertices                                        vertices;

        /**
         * Creates the four vertices vector with the correct
         * texture positions
         *
         * @param positions the vertices positions
         * @return the vertices vector
         */
        static Vertices makeVertices(
            Positions const& positions = {});

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
            Positions const& positions = {}
            ) requires IsColorable;

        /**
         * Actualizes buffers before draw
         */
        void actualizeBufferBeforeDraw(void) const noexcept final;

        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;
    };

    template class GlyphSprite<true>;
    template class GlyphSprite<false>;

}

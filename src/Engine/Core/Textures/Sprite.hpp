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

#include "ShadeableSprite.hpp"

namespace mpgl {


    /**
     * Represents a texture on the screen
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class Sprite : public ShadeableSprite<IsColorable> {
    public:
        typedef ShadeableSprite<IsColorable>::Vertex    Vertex;
        typedef ShadeableSprite<IsColorable>::Vertices  Vertices;

        /**
         * Construct a new Sprite object with a given
         * texture
         *
         * @param texture the sprite's texture
         */
        Sprite(Texture const& texture = {});

        /**
         * Construct a new Sprite object with a given
         * texture and color
         *
         * @param texture the sprite's texture
         * @param color the sprite vertices color
         */
        Sprite(
            Texture const& texture,
            Color const& color) requires IsColorable;

        /**
         * Construct a new parralelogram-like Sprite object
         * from a three given vertices with given texture
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex color
         */
        Sprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex);

        /**
         * Construct a new parralelogram-like Sprite object
         * from a three given vertices with given texture
         * and color
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex color
         * @param color the vertices color
         */
        Sprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex,
            Color const& color) requires IsColorable;

        /**
         * Construct a new Sprite object parallel to
         * x and y axis with a given texture
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param dimensions the sprite dimensions
         */
        Sprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& dimensions);

        /**
         * Construct a new Sprite object parallel to
         * x and y axis with a given texture and color
         *
         * @param texture the sprite's texture
         * @param firstVertex the first vertex position
         * @param dimensions the sprite dimensions
         * @param color the vertices color
         */
        Sprite(
            Texture const& texture,
            Vector2f const& firstVertex,
            Vector2f const& dimensions,
            Color const& color) requires IsColorable;

        Sprite(Sprite const& sprite) = default;
        Sprite(Sprite&& sprite) noexcept = default;

        Sprite& operator= (Sprite const& sprite) = default;
        Sprite& operator= (Sprite&& sprite) noexcept = default;

        /**
         * Draws the sprite on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept
            { this->isModified = true; return this->vertices[index]; }

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
                { return this->vertices[index]; }

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        [[nodiscard]] Vertex& front(void) noexcept
            { this->isModified = true; return this->vertices.front(); }

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        [[nodiscard]] Vertex const& front(void) const noexcept
            { return this->vertices.front(); }

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        [[nodiscard]] Vertex& back(void) noexcept
            { this->isModified = true; return this->vertices.back(); }

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        [[nodiscard]] Vertex const& back(void) const noexcept
            { return this->vertices.back(); }

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
            { return this->vertices.size(); }

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept
            { this->isModified = true; return this->vertices.begin(); }

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept
            { this->isModified = true; return this->vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return this->vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return this->vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return this->vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return this->vertices.end(); }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { this->isModified = true; return this->vertices.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { this->isModified = true; return this->vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return this->vertices.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return this->vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return this->vertices.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return this->vertices.crend(); }

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram const& program) noexcept override final;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram&& program) noexcept override final;

        /**
         * Sets the given shader program from an internal
         * library
         *
         * @param name the name of the shader program
         */
        void setShader(
            std::string const& name) override final;

        /**
         *  Destroy the Sprite object
         */
        ~Sprite(void) noexcept = default;
    private:
        typedef Shadeable::Executable               Executable;

        /**
         * Returns the shader name used by sprite implementation
         *
         * @return the shader name used by sprite implementation
         */
        static std::string shaderName(void) noexcept;

        static const Executable                     shaderExec;
    };

    template class Sprite<true>;
    template class Sprite<false>;

    typedef Sprite<false>                           DefaultSprite;
    typedef Sprite<true>                            ColorableSprite;

}

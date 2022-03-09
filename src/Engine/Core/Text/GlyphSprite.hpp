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

#include "../Textures/Texturable.hpp"

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
    class GlyphSprite : public Texturable<IsColorable> {
    public:
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
         * object from a given three vertices and sprite's
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
         * object from a given three vertices, sprite's
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

        GlyphSprite(GlyphSprite const& sprite) = default;
        GlyphSprite(GlyphSprite&& sprite) noexcept = default;

        GlyphSprite& operator= (
            GlyphSprite const& sprite) = default;
        GlyphSprite& operator= (
            GlyphSprite&& sprite) noexcept = default;

        /**
         * Draws sprite on the screen
         */
        void draw(void) const noexcept;

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
    };

    template class GlyphSprite<true>;
    template class GlyphSprite<false>;

}

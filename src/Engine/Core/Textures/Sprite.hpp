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
         *  Destroy the Sprite object
         */
        ~Sprite(void) noexcept = default;
    private:
        /**
         * Returns the shader name used by sprite implementation
         *
         * @return the shader name used by sprite implementation
         */
        static std::string shaderName(void) noexcept;
    };

    template class Sprite<true>;
    template class Sprite<false>;

    typedef Sprite<false>                           DefaultSprite;
    typedef Sprite<true>                            ColorableSprite;

}

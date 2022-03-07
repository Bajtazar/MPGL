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

#include "../Angular.hpp"

namespace mpgl {

    /**
     * Represents a triangle figure
     */
    struct Triangle : public Angular {
        /**
         * Constructs a new Triangle with given
         * vertices
         *
         * @param firstVertex the first vertex coord
         * @param secondVertex the second vertex coord
         * @param thirdVertex the third vertex coord
         * @param color the color of the triangle
         */
        Triangle(Vector2f const& firstVertex,
                Vector2f const& secondVertex,
                Vector2f const& thirdVertex,
                Color const& color = {});

        /**
         * Constructs a new Triangle with given
         * color
         *
         * @param color the color of the triangle
         */
        Triangle(Color const& color = {});

        Triangle(Triangle const& triangle) = default;
        Triangle(Triangle&& triangle) noexcept = default;

        Triangle& operator= (Triangle const& triangle) = default;
        Triangle& operator= (Triangle&& triangle) noexcept = default;

        /**
         * Draws the triangle on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Destroy the Triangle object
         */
        ~Triangle(void) noexcept = default;
    };

}

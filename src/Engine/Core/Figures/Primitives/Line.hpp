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
     * Represents the line figure
     */
    struct Line : public Angular {
        /**
         * Construct a new line with the begin
         * in the first vertex and the end in the
         * second one
         *
         * @param firstVertex the begin of the line
         * @param secondVertex the end of the line
         * @param color the color of the line
         */
        Line(Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Color const& color = {});

        /**
         * Construct a new Line object with the
         * given color
         *
         * @param color the color of the line
         */
        Line(Color const& color = {});

        Line(Line const& line) = default;
        Line(Line&& line) noexcept = default;

        Line& operator=(Line const& line) = default;
        Line& operator=(Line&& line) noexcept = default;

        /**
         * Returns the center of the line
         *
         * @return the center of the line
         */
        Vector2f getLineCenter(void) const noexcept;

        /**
         * Draws the line on the screen
         */
        void draw(void) const noexcept final;

        /**
         *  Destroy the Line object
         */
        ~Line(void) noexcept = default;
    };

}

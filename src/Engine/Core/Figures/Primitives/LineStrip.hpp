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

#include "../ResizableAngular.hpp"

#include <optional>

namespace mpgl {

    /**
     * Represents the line strip on the screen
     */
    struct LineStrip : public ResizableAngular {
        /**
         * Construct a new Line Strip object and initializes the
         * vertices with the given positions and color
         *
         * @tparam ColorTp the color vector type
         * @tparam Vectors the parameter pack of 2D vectors
         * @param color the color of the vertices
         * @param positions the vertices positions
         */
        template <class ColorTp, AllConvertible<Vector2f>... Vectors>
            requires std::constructible_from<Color, ColorTp>
        LineStrip(ColorTp&& color, Vectors&&... vertices);

        /**
         * Construct a new Line Strip object and initializes the
         * vertices with the given positions
         *
         * @tparam Vectors the parameter pack of 2D vectors
         * @param positions the vertices positions
         */
        template <AllConvertible<Vector2f>... Vectors>
        LineStrip(Vectors&&... vertices);

        /**
         * Construct a new Line Strip object with given number
         * of vertices and the given base color
         *
         * @param vertices the number of the vertices
         * @param color the color of the vertices
         */
        LineStrip(std::size_t vertices = 0,
            Color const& color = {});

        LineStrip(LineStrip const& lineStrip) = default;
        LineStrip(LineStrip&& lineStrip) noexcept = default;

        LineStrip& operator= (LineStrip const& lineStrip) = default;
        LineStrip& operator= (LineStrip&& lineStrip) noexcept = default;

        /**
         * Draws the polygon on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Destroy the Line Strip object
         */
        ~LineStrip(void) noexcept = default;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Vectors>
        requires std::constructible_from<Color, ColorTp>
    LineStrip::LineStrip(ColorTp&& color, Vectors&&... vertices)
        : ResizableAngular{std::forward<Color>(color),
            std::forward<Vectors>(vertices)...} {}

    template <AllConvertible<Vector2f>... Vectors>
    LineStrip::LineStrip(Vectors&&... vertices)
        : ResizableAngular{std::forward<Vectors>(vertices)...} {}

}

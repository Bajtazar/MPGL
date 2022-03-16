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

namespace mpgl {

    /**
     * Represents a polygon figure
     */
    class Polygon : public ResizableAngular {
    public:
        /**
         * Construct a new circle-like Polygon object with
         * the given radius, center, number of segments and color
         *
         * @param center the center of the polygon
         * @param radius the radius of the polygon
         * @param segments the number of the triangles which
         * the polygon consists of
         * @param color the color of the polygon
         */
        Polygon(Vector2f const& center = {}, float32 radius = 0.f,
            std::size_t segments = 0, Color const& color = {});

        /**
         * Construct a new Polygon object and initializes the
         * vertices with the given positions and color
         *
         * @tparam ColorTp the color vector type
         * @tparam Vectors the parameter pack of 2D vectors
         * @param color the color of the vertices
         * @param positions the vertices positions
         */
        template <class ColorTp, AllConvertible<Vector2f>... Vectors>
            requires std::constructible_from<Color, ColorTp>
        Polygon(ColorTp&& color, Vectors&&... vertices);

        /**
         * Construct a new Polygon object and initializes the
         * vertices with the given positions
         *
         * @tparam Vectors the parameter pack of 2D vectors
         * @param positions the vertices positions
         */
        template <AllConvertible<Vector2f>... Vectors>
        Polygon(Vectors&&... positions);

        Polygon(Polygon const& polygon) = default;
        Polygon(Polygon&& polygon) noexcept = default;

        Polygon& operator= (Polygon const& polygon) = default;
        Polygon& operator= (Polygon&& polygon) noexcept = default;

        /**
         * Draws the polygon on the screen
         */
        void draw(void) const noexcept;

        /**
         * Checks whether a given point is located inside the polygon
         *
         * @param position the point position [pixel position]
         * @return if the given point is inside the polygon
         */
        [[nodiscard]] bool contains(
            Vector2f const& position) const noexcept /*final*/;

        /**
         * Destroy the Polygon object
         */
        ~Polygon(void) noexcept = default;
    private:
        /**
         * Calculates whether a given point is inside subtriangle
         *
         * @param position the point position
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex position
         * @return if the given point is inside subtriangle
         */
        [[nodiscard]] bool insideSubtriangle(
            Vector2d const& position,
            Vector2d const& firstVertex,
            Vector2d const& secondVertex,
            Vector2d const& thirdVertex) const noexcept;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Vectors>
        requires std::constructible_from<Color, ColorTp>
    Polygon::Polygon(ColorTp&& color, Vectors&&... vertices)
        : ResizableAngular{std::forward<ColorTp>(color),
            std::forward<Vectors>(vertices)...} {}

    template <AllConvertible<Vector2f>... Vectors>
    Polygon::Polygon(Vectors&&... vertices)
        : ResizableAngular{std::forward<Vectors>(vertices)...} {}

}

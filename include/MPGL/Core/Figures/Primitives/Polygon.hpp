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

#include <MPGL/Core/Figures/Primitives/Helpers/PolygonHelpers.hpp>
#include <MPGL/Core/Figures/ResizableAngular.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

namespace mpgl {

    /**
     * Represents a polygon figure
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    class Polygon :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public ResizableAngular<Dim, Spec>
    {
    public:
        using VertexTraits = Angular<Dim, Spec>::VertexTraits;
        using Vector = VertexTraits::Vector;

        /**
         * Constructs a new Polygon object and initializes the
         * vertices with the given positions and color
         *
         * @tparam ColorTp the color vector type
         * @tparam Vectors the parameter pack of 2D vectors
         * @param color the color of the vertices
         * @param positions the vertices positions
         */
        template <class ColorTp, AllConvertible<Vector>... Vectors>
            requires std::constructible_from<Color, ColorTp>
        Polygon(ColorTp&& color, Vectors&&... vertices);

        /**
         * Constructs a new Polygon object and initializes the
         * vertices with the given positions
         *
         * @tparam Vectors the parameter pack of 2D vectors
         * @param positions the vertices positions
         */
        template <AllConvertible<Vector>... Vectors>
        Polygon(Vectors&&... positions);

        /**
         * Constructs a new Polygon object with given number
         * of vertices and the given base color
         *
         * @param vertices the number of the vertices
         * @param color the color of the vertices
         */
        Polygon(
            std::size_t vertices = 0,
            Color const& color = {});

        /**
         * Constructs a new circle-like Polygon object with
         * the given radius, center, number of segments and color
         *
         * @param center the center of the polygon
         * @param radius the radius of the polygon
         * @param segments the number of the triangles which
         * the polygon consists of
         * @param color the color of the polygon
         */
        Polygon(
            Vector2f const& center,
            float32 radius = 0.f,
            std::size_t segments = 0,
            Color const& color = {}) requires TwoDimensional<Dim>;

        Polygon(Polygon const& polygon) = default;
        Polygon(Polygon&& polygon) noexcept = default;

        Polygon& operator= (Polygon const& polygon) = default;
        Polygon& operator= (Polygon&& polygon) noexcept = default;

        /**
         * Draws the polygon on the screen
         */
        void draw(void) const noexcept;

        /**
         * Checks whether the given pixel is located inside the polygon
         *
         * @param position the pixel's position
         * @return if the given point is inside the polygon
         */
        [[nodiscard]] bool contains(
            Vector2u const& position) const noexcept final;

        /**
         * Destroys the Polygon object
         */
        ~Polygon(void) noexcept = default;

        friend class PolygonDrawer<Dim, Spec>;
        friend class PolygonClickChecker<Dim, Spec>;
    private:
        using Drawer = PolygonDrawer<Dim, Spec>;
        using Clicker = PolygonClickChecker<Dim, Spec>;

        static Drawer const                         drawer;
        static Clicker const                        clicker;
    };

    template class Polygon<dim::Dim2>;
    template class Polygon<dim::Dim3>;

    typedef Polygon<dim::Dim2>                      Polygon2D;
    typedef Polygon<dim::Dim3>                      Polygon3D;

}

#include <MPGL/Core/Figures/Primitives/Polygon.tpp>

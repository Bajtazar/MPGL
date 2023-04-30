/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Core/Figures/Primitives/LineStrip.hpp>
#include <MPGL/Core/Figures/Primitives/LineLoop.hpp>
#include <MPGL/Core/Figures/Primitives/Triangle.hpp>
#include <MPGL/Core/Figures/Primitives/Tetragon.hpp>
#include <MPGL/Core/Figures/Primitives/Ellipse.hpp>
#include <MPGL/Core/Figures/Primitives/Polygon.hpp>
#include <MPGL/Core/Figures/Primitives/Points.hpp>
#include <MPGL/Core/Figures/Primitives/Ring.hpp>
#include <MPGL/Core/Figures/Primitives/Line.hpp>

namespace mpgl {

    /**
     * Functor responsible for setting the default texture
     * coordinates for the given figures
     *
     * @tparam Tp the figure type
     */
    template <SpecializationOf<Figure> Tp>
    class TexturedFigurePlacer;

    /**
     * Functor responsible for the default texture
     * coordinates for the triangle
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Triangle<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the triangle
         * ({[0, 0], [1, 0], [0.5, 1]} by default)
         *
         * @param triangle a reference to the triangle object
         */
        void operator() (Triangle<Dim, Spec>& triangle) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Triangle<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Triangle<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the tetragon
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Tetragon<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the tetragon
         * ({[0, 0], [0, 1], [1, 1], [1, 0]} by default)
         *
         * @param tetragon a reference to the tetragon object
         */
        void operator() (Tetragon<Dim, Spec>& tetragon) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Tetragon<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Tetragon<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Line<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the line
         * ({[0, 0], [1, 0]} by default)
         *
         * @param line a reference to the line object
         */
        void operator() (Line<Dim, Spec>& line) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Line<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Line<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line strip
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<LineStrip<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the line strip
         * ([i/(n - 1), 0] for each vertex in range [0, n] if
         * strip size is bigger than 1)
         *
         * @param lineStrip a reference to the line strip object
         */
        void operator() (LineStrip<Dim, Spec>& lineStrip) const noexcept;
    };

    extern template struct TexturedFigurePlacer<LineStrip<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<LineStrip<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line loop
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<LineLoop<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the line loop
         * ([i/(n - 1), 0] for each vertex in range [0, n] if
         * loop size is bigger than 1)
         *
         * @param lineLoop a reference to the line loop object
         */
        void operator() (LineLoop<Dim, Spec>& lineLoop) const noexcept;
    };

    extern template struct TexturedFigurePlacer<LineLoop<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<LineLoop<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the points
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Points<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the points
         * ([i/(n - 1), 0] for each vertex in range [0, n] if
         * points is bigger than 1)
         *
         * @param points a reference to the points object
         */
        void operator() (Points<Dim, Spec>& points) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Points<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Points<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the polygon
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the angular vertices specifier
     */
    template <Dimension Dim, TexturableAngularTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Polygon<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the polygon
         * ([i/(n - 1), 0] for each vertex in range [0, n] if
         * polygon is bigger than 1)
         *
         * @param polygon a reference to the polygon object
         */
        void operator() (Polygon<Dim, Spec>& polygon) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Polygon<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Polygon<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the ellipse
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the elliptic vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Ellipse<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the ellipse
         * ({[0, 0], [0, 1], [1, 1], [1, 0]})
         *
         * @param ellipse a reference to the ellipse object
         */
        void operator() (Ellipse<Dim, Spec>& ellipse) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Ellipse<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Ellipse<dim::Dim3, uint8>>;

    /**
     * Functor responsible for the default texture
     * coordinates for the ring
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the elliptic vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    struct TexturedFigurePlacer<Ring<Dim, Spec>> {
        /**
         * Sets the default texture coordinates for the ring
         * ({[0, 0], [0, 1], [1, 1], [1, 0]})
         *
         * @param ring a reference to the ring object
         */
        void operator() (Ring<Dim, Spec>& ring) const noexcept;
    };

    extern template struct TexturedFigurePlacer<Ring<dim::Dim2, uint8>>;
    extern template struct TexturedFigurePlacer<Ring<dim::Dim3, uint8>>;

}

#include <MPGL/Core/Textures/TexturedFigurePlacer.tpp>

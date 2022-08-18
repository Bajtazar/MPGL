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
     * @tparam Figure the figure type
     */
    template <template <class, typename> class Figure>
    class TexturedFigurePlacer;

    /**
     * Functor responsible for the default texture
     * coordinates for the triangle
     */
    template <>
    struct TexturedFigurePlacer<Triangle> {
        /**
         * Functor responsible for the default texture
         * coordinates for the triangle. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the triangle
             * ({[0, 0], [1, 0], [0.5, 1]} by default)
             *
             * @param triangle a reference to the triangle object
             */
            void operator() (
                Triangle<Dim, Spec>& triangle) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the triangle. Calls
         * the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param triangle a reference to the triangle object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Triangle<Dim, Spec>& triangle) const noexcept
            { Setter<Dim, Spec>{}(triangle); }
    };

    template struct TexturedFigurePlacer<Triangle>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Triangle>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the tetragon
     */
    template <>
    struct TexturedFigurePlacer<Tetragon> {
        /**
         * Functor responsible for the default texture
         * coordinates for the tetragon. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the tetragon
             * ({[0, 0], [0, 1], [1, 1], [1, 0]} by default)
             *
             * @param tetragon a reference to the tetragon object
             */
            void operator() (
                Tetragon<Dim, Spec>& tetragon) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the tetragon. Calls
         * the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param tetragon a reference to the tetragon object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Tetragon<Dim, Spec>& tetragon) const noexcept
            { Setter<Dim, Spec>{}(tetragon); }
    };

    template struct TexturedFigurePlacer<Tetragon>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Tetragon>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line
     */
    template <>
    struct TexturedFigurePlacer<Line> {
        /**
         * Functor responsible for the default texture
         * coordinates for the line. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the line
             * ({[0, 0], [1, 0]} by default)
             *
             * @param line a reference to the line object
             */
            void operator() (
                Line<Dim, Spec>& line) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the line. Calls
         * the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param line a reference to the line object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Line<Dim, Spec>& line) const noexcept
            { Setter<Dim, Spec>{}(line); }
    };

    template struct TexturedFigurePlacer<Line>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Line>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line strip
     */
    template <>
    struct TexturedFigurePlacer<LineStrip> {
        /**
         * Functor responsible for the default texture
         * coordinates for the line strip. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the line strip
             * ([i/(n - 1), 0] for each vertex in range [0, n] if
             * strip size is bigger than 1)
             *
             * @param line a reference to the line strip object
             */
            void operator() (
                LineStrip<Dim, Spec>& line) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the line strip.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param lineStrip a reference to the line strip object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (LineStrip<Dim, Spec>& lineStrip) const noexcept
            { Setter<Dim, Spec>{}(lineStrip); }
    };

    template struct TexturedFigurePlacer<LineStrip>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<LineStrip>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the line loop
     */
    template <>
    struct TexturedFigurePlacer<LineLoop> {
        /**
         * Functor responsible for the default texture
         * coordinates for the line loop. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the line loop
             * ([i/(n - 1), 0] for each vertex in range [0, n] if
             * loop size is bigger than 1)
             *
             * @param lineLoop a reference to the line loop object
             */
            void operator() (
                LineLoop<Dim, Spec>& lineLoop) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the line loop.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param lineLoop a reference to the line loop object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (LineLoop<Dim, Spec>& lineLoop) const noexcept
            { Setter<Dim, Spec>{}(lineLoop); }
    };

    template struct TexturedFigurePlacer<LineLoop>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<LineLoop>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the points
     */
    template <>
    struct TexturedFigurePlacer<Points> {
        /**
         * Functor responsible for the default texture
         * coordinates for the points. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the points
             * ([i/(n - 1), 0] for each vertex in range [0, n] if
             * points is bigger than 1)
             *
             * @param points a reference to the points object
             */
            void operator() (
                Points<Dim, Spec>& points) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the points.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param points a reference to the points object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Points<Dim, Spec>& points) const noexcept
            { Setter<Dim, Spec>{}(points); }
    };

    template struct TexturedFigurePlacer<Points>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Points>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the polygon
     */
    template <>
    struct TexturedFigurePlacer<Polygon> {
        /**
         * Functor responsible for the default texture
         * coordinates for the polygon. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the polygon
             * ([i/(n - 1), 0] for each vertex in range [0, n] if
             * polygon is bigger than 1)
             *
             * @param polygon a reference to the polygon object
             */
            void operator() (
                Polygon<Dim, Spec>& polygon) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the polygon.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param polygon a reference to the polygon object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Polygon<Dim, Spec>& polygon) const noexcept
            { Setter<Dim, Spec>{}(polygon); }
    };

    template struct TexturedFigurePlacer<Polygon>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Polygon>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the ellipse
     */
    template <>
    struct TexturedFigurePlacer<Ellipse> {
        /**
         * Functor responsible for the default texture
         * coordinates for the ellipse. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the ellipse
             * ({[0, 0], [0, 1], [1, 1], [1, 0]})
             *
             * @param ellipse a reference to the ellipse object
             */
            void operator() (
                Ellipse<Dim, Spec>& ellipse) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the ellipse.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param ellipse a reference to the ellipse object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Ellipse<Dim, Spec>& ellipse) const noexcept
            { Setter<Dim, Spec>{}(ellipse); }
    };

    template struct TexturedFigurePlacer<Ellipse>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Ellipse>::Setter<dim::Dim3, uint8>;

    /**
     * Functor responsible for the default texture
     * coordinates for the ring
     */
    template <>
    struct TexturedFigurePlacer<Ring> {
        /**
         * Functor responsible for the default texture
         * coordinates for the ring. Allows to shift
         * code into the source file
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        struct Setter {
            /**
             * Sets the default texture coordinates for the ring
             * ({[0, 0], [0, 1], [1, 1], [1, 0]})
             *
             * @param ellipse a reference to the ring object
             */
            void operator() (
                Ring<Dim, Spec>& ring) const noexcept;
        };

        /**
         * Sets the default texture coordinates for the ring.
         * Calls the inner setter
         *
         * @tparam Dim the dimension of the space
         * @tparam Specifier the angular vertices specifier
         * @param ring a reference to the ring object
         */
        template <
            Dimension Dim,
            TexturableAngularTraitSpecifier<Dim> Spec>
        void operator() (Ring<Dim, Spec>& ring) const noexcept
            { Setter<Dim, Spec>{}(ring); }
    };

    template struct TexturedFigurePlacer<Ring>::Setter<dim::Dim2, uint8>;
    template struct TexturedFigurePlacer<Ring>::Setter<dim::Dim3, uint8>;

}

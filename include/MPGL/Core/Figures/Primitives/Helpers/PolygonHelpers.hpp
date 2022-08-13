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

#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a polygon on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class Polygon;

    /**
     * Functor responsible for drawing polygon on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct PolygonDrawer;

    /**
     * Functor responsible for drawing 2D polygon on the screen
     */
    template <>
    struct PolygonDrawer<dim::Dim2, void> {
        /**
         * Draws 2D polygon on the screen
         *
         * @param polygon a constant reference to the polygon object
         */
        void operator() (
            Polygon<dim::Dim2, void> const& polygon) const noexcept;
    };

    /**
     * Functor responsible for drawing 3D polygon on the screen
     */
    template <>
    struct PolygonDrawer<dim::Dim3, void> {
        /**
         * Draws 3D polygon on the screen
         *
         * @param polygon a constant reference to the polygon object
         */
        void operator() (
            Polygon<dim::Dim3, void> const& polygon) const noexcept;
    };

    /**
     * Functor responsible for checking whether given point is
     * inside a polygon
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct PolygonClickChecker;

    /**
     * Functor responsible for checking whether given point is
     * inside a default polygon
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    struct PolygonClickChecker<Dim, void> {
        /**
         * Functor responsible for normalizing the vertices range
         * inside the polygon click checker functor
         */
        struct Normalizer;

        /**
         * Checks whether the given point is inside a default polygon
         *
         * @param polygon a constant reference to the polygon object
         * @param position a constant reference to the position object
         * @return if the given point is inside a default polygon
         */
        [[nodiscard]] bool operator() (
            Polygon<Dim, void> const& polygon,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 2D polygon click checker functor
     */
    template <>
    struct PolygonClickChecker<dim::Dim2, void>::Normalizer {
        /**
         * Normalizes the polygon's vertex positions and returns
         * a view to it
         *
         * @param polygon a constant reference to the polygon object
         * @return the view to normalized polygon's vertex
         * positions
         */
        [[nodiscard]] auto operator() (
            Polygon<dim::Dim2, void> const& polygon) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 3D polygon click checker functor
     */
    template <>
    struct PolygonClickChecker<dim::Dim3, void>::Normalizer {
        /**
         * Normalizes the polygon's projected vertex positions
         * and returns a view to it
         *
         * @param polygon a constant reference to the polygon object
         * @return the view to normalized polygon's projected
         * vertex positions
         */
        [[nodiscard]] auto operator() (
            Polygon<dim::Dim3, void> const& polygon) const noexcept;
    };

    template class PolygonClickChecker<dim::Dim2, void>;
    template class PolygonClickChecker<dim::Dim3, void>;

}

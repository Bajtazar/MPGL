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
     * Represents a points on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class Points;

    /**
     * Functor responsible for drawing points on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct PointsDrawer;

    /**
     * Functor responsible for drawing 2D points on the screen
     */
    template <>
    struct PointsDrawer<dim::Dim2, void> {
        /**
         * Draws 2D points on the screen
         *
         * @param points a constant reference to the points object
         */
        void operator() (
            Points<dim::Dim2, void> const& points) const noexcept;
    };

    /**
     * Functor responsible for drawing 3D points on the screen
     */
    template <>
    struct PointsDrawer<dim::Dim3, void> {
        /**
         * Draws 3D points on the screen
         *
         * @param points a constant reference to the points object
         */
        void operator() (
            Points<dim::Dim3, void> const& points) const noexcept;
    };

    /**
     * Functor responsible for checking whether given point is
     * inside a points group
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct PointsClickChecker;

    /**
     * Functor responsible for checking whether given point is
     * inside a default points group
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    struct PointsClickChecker<Dim, void> {
        /**
         * Functor responsible for normalizing the vertices range
         * inside the line strip click checker functor
         */
        struct Normalizer;

        /**
         * Checks whether the given point is inside a default points
         * group
         *
         * @param points a constant reference to the points object
         * @param position a constant reference to the position object
         * @return if the given point is inside a default points group
         */
        [[nodiscard]] bool operator() (
            Points<Dim, void> const& points,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 2D points click checker functor
     */
    template <>
    struct PointsClickChecker<dim::Dim2, void>::Normalizer {
        /**
         * Normalizes the points's vertex positions and returns
         * a view to it
         *
         * @param points a constant reference to the points object
         * @return the view to normalized points's vertex
         * positions
         */
        [[nodiscard]] auto operator() (
            Points<dim::Dim2, void> const& points) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 3D points click checker functor
     */
    template <>
    struct PointsClickChecker<dim::Dim3, void>::Normalizer {
        /**
         * Normalizes the points's projected vertex positions
         * and returns a view to it
         *
         * @param points a constant reference to the points object
         * @return the view to normalized points's projected
         * vertex positions
         */
        [[nodiscard]] auto operator() (
            Points<dim::Dim3, void> const& points) const noexcept;
    };

    template class PointsClickChecker<dim::Dim2, void>;
    template class PointsClickChecker<dim::Dim3, void>;

}

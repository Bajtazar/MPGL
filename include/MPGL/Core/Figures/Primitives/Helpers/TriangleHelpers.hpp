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
     * Represents a triangle figure
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class Triangle;

    /**
     * Functor responsible for drawing triangle on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct TriangleDrawer;

    /**
     * Functor responsible for drawing 2D triangle on the screen
     */
    template <>
    struct TriangleDrawer<dim::Dim2, void> {
        /**
         * Draws 2D triangle on the screen
         *
         * @param triangle a constant reference to the triangle object
         */
        void operator() (
            Triangle<dim::Dim2, void> const& triangle) const noexcept;
    };

    /**
     * Functor responsible for drawing 3D triangle on the screen
     */
    template <>
    struct TriangleDrawer<dim::Dim3, void> {
        /**
         * Draws 3D triangle on the screen
         *
         * @param triangle a constant reference to the triangle object
         */
        void operator() (
            Triangle<dim::Dim3, void> const& triangle) const noexcept;
    };

    /**
     * Functor responsible for checking whether given point is
     * inside a triangle
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct TriangleClickChecker;

    /**
     * Functor responsible for checking whether given point is
     * inside a 2D triangle
     */
    template <>
    struct TriangleClickChecker<dim::Dim2, void> {
        /**
         * Checks whether the given point is inside a 2D triangle
         *
         * @param triangle a constant reference to the triangle object
         * @param position a constant reference to the position object
         * @return if the given point is inside a 2D triangle
         */
        [[nodiscard]] bool operator() (
            Triangle<dim::Dim2, void> const& triangle,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for checking whether given point is
     * inside a 3D triangle's projection
     */
    template <>
    struct TriangleClickChecker<dim::Dim3, void> {
        /**
         * Checks whether the given point is inside a 3D triangle's
         * projection
         *
         * @param triangle a constant reference to the triangle object
         * @param position a constant reference to the position object
         * @return if the given point is inside a 3D triangle's
         * projection
         */
        [[nodiscard]] bool operator() (
            Triangle<dim::Dim3, void> const& triangle,
            Vector2u const& position) const noexcept;
    };

}

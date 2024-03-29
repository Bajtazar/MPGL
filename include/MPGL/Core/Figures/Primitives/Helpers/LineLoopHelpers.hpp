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

#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a line loop
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class LineLoop;

    /**
     * Functor responsible for drawing line loop on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineLoopDrawer;

    /**
     * Functor responsible for drawing 2D line loop on the screen
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim2> Spec>
    struct LineLoopDrawer<dim::Dim2, Spec> {
        /**
         * Draws 2D line loop on the screen
         *
         * @param lineLoop a constant reference to the
         * line loop object
         */
        void operator() (
            LineLoop<dim::Dim2, Spec> const& lineLoop) const noexcept;
    };

    /**
     * Functor responsible for drawing 3D line loop on the screen
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec>
    struct LineLoopDrawer<dim::Dim3, Spec> {
        /**
         * Draws 3D line loop on the screen
         *
         * @param lineLoop a constant reference to the line loop
         * object
         */
        void operator() (
            LineLoop<dim::Dim3, Spec> const& lineLoop) const noexcept;
    };

    extern template class LineLoopDrawer<dim::Dim2, void>;
    extern template class LineLoopDrawer<dim::Dim2, uint8>;
    extern template class LineLoopDrawer<dim::Dim3, void>;
    extern template class LineLoopDrawer<dim::Dim3, uint8>;

    /**
     * Functor responsible for normalizing the vertices range
     * inside the line loop click checker functor
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineLoopClickCheckerNormalizer;

    /**
     * Functor responsible for checking whether given point is
     * inside a line loop
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineLoopClickChecker {
        /**
         * Functor responsible for normalizing the vertices range
         * inside the line loop click checker functor
         */
        using Normalizer = LineLoopClickCheckerNormalizer<Dim, Spec>;

        /**
         * Checks whether the given point is inside a default line
         * loop
         *
         * @param lineLoop a constant reference to the line loop
         * object
         * @param position a constant reference to the position object
         * @return if the given point is inside a default line loop
         */
        [[nodiscard]] bool operator() (
            LineLoop<Dim, Spec> const& lineLoop,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 2D line loop click checker functor
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim2> Spec>
    struct LineLoopClickCheckerNormalizer<dim::Dim2, Spec> {
        /**
         * Normalizes the line loop's vertex positions and returns
         * a view to it
         *
         * @param lineLoop a constant reference to the line loop
         * object
         * @return the view to normalized line loop's vertex
         * positions
         */
        [[nodiscard]] auto operator() (
            LineLoop<dim::Dim2, Spec> const& lineLoop) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 3D line loop click checker functor
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec>
    struct LineLoopClickCheckerNormalizer<dim::Dim3, Spec> {
        /**
         * Normalizes the line loop's projected vertex positions
         * and returns a view to it
         *
         * @param lineLoop a constant reference to the line loop
         * object
         * @return the view to normalized line loop's projected
         * vertex positions
         */
        [[nodiscard]] auto operator() (
            LineLoop<dim::Dim3, Spec> const& lineLoop) const noexcept;
    };

    extern template class LineLoopClickCheckerNormalizer<dim::Dim2, void>;
    extern template class LineLoopClickCheckerNormalizer<dim::Dim3, void>;
    extern template class LineLoopClickCheckerNormalizer<dim::Dim2, uint8>;
    extern template class LineLoopClickCheckerNormalizer<dim::Dim3, uint8>;

    extern template class LineLoopClickChecker<dim::Dim2, void>;
    extern template class LineLoopClickChecker<dim::Dim3, void>;
    extern template class LineLoopClickChecker<dim::Dim2, uint8>;
    extern template class LineLoopClickChecker<dim::Dim3, uint8>;

}

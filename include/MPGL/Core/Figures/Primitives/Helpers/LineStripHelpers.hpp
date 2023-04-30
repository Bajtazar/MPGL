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
     * Represents a line strip
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec = void>
    class LineStrip;

    /**
     * Functor responsible for drawing line strip on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineStripDrawer;

    /**
     * Functor responsible for drawing 2D line strip on the screen
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim2> Spec>
    struct LineStripDrawer<dim::Dim2, Spec> {
        /**
         * Draws 2D line strip on the screen
         *
         * @param lineStrip a constant reference to the
         * line strip object
         */
        void operator() (
            LineStrip<dim::Dim2, Spec> const& lineStrip) const noexcept;
    };

    /**
     * Functor responsible for drawing 3D line strip on the screen
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec>
    struct LineStripDrawer<dim::Dim3, Spec> {
        /**
         * Draws 3D line strip on the screen
         *
         * @param lineStrip a constant reference to the line strip
         * object
         */
        void operator() (
            LineStrip<dim::Dim3, Spec> const& lineStrip) const noexcept;
    };

    extern template class LineStripDrawer<dim::Dim2, void>;
    extern template class LineStripDrawer<dim::Dim3, void>;
    extern template class LineStripDrawer<dim::Dim2, uint8>;
    extern template class LineStripDrawer<dim::Dim3, uint8>;

    /**
     * Functor responsible for normalizing the vertices range
     * inside the line strip click checker functor
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineStripClickCheckerNormalizer;

    /**
     * Functor responsible for checking whether given point is
     * inside a line strip
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    struct LineStripClickChecker {
        /**
         * Functor responsible for normalizing the vertices range
         * inside the line strip click checker functor
         */
        using Normalizer = LineStripClickCheckerNormalizer<Dim, Spec>;

        /**
         * Checks whether the given point is inside a default line
         * strip
         *
         * @param lineStrip a constant reference to the line strip
         * object
         * @param position a constant reference to the position object
         * @return if the given point is inside a default line strip
         */
        [[nodiscard]] bool operator() (
            LineStrip<Dim, Spec> const& lineStrip,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 2D line strip click checker functor
     *
     * @tparam Dim the dimension of the space
     */
    template <AngularTraitSpecifier<dim::Dim2> Spec>
    struct LineStripClickCheckerNormalizer<dim::Dim2, Spec> {
        /**
         * Normalizes the line strip's vertex positions and returns
         * a view to it
         *
         * @param lineStrip a constant reference to the line strip
         * object
         * @return the view to normalized line strip's vertex
         * positions
         */
        [[nodiscard]] auto operator() (
            LineStrip<dim::Dim2, Spec> const& lineStrip
            ) const noexcept;
    };

    /**
     * Functor responsible for normalizing the vertices range
     * inside the 3D line strip click checker functor
     *
     * @tparam Dim the dimension of the space
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec>
    struct LineStripClickCheckerNormalizer<dim::Dim3, Spec> {
        /**
         * Normalizes the line strip's projected vertex positions
         * and returns a view to it
         *
         * @param lineStrip a constant reference to the line strip
         * object
         * @return the view to normalized line strip's projected
         * vertex positions
         */
        [[nodiscard]] auto operator() (
            LineStrip<dim::Dim3, Spec> const& lineStrip
            ) const noexcept;
    };

    extern template class LineStripClickCheckerNormalizer<dim::Dim2, void>;
    extern template class LineStripClickCheckerNormalizer<dim::Dim3, void>;
    extern template class LineStripClickCheckerNormalizer<dim::Dim2, uint8>;
    extern template class LineStripClickCheckerNormalizer<dim::Dim3, uint8>;

    extern template class LineStripClickChecker<dim::Dim2, void>;
    extern template class LineStripClickChecker<dim::Dim3, void>;
    extern template class LineStripClickChecker<dim::Dim2, uint8>;
    extern template class LineStripClickChecker<dim::Dim3, uint8>;

}

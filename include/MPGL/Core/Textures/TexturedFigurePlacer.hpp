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

#include <MPGL/Core/Figures/Primitives/Triangle.hpp>
#include <MPGL/Core/Figures/Primitives/Tetragon.hpp>

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

}

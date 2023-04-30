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

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    template <
        class ColorTp,
        AllConvertible<
            typename ResizableAngular<Dim, Spec>::Vector>... Args>
                requires std::constructible_from<Color, ColorTp>
    ResizableAngular<Dim, Spec>::ResizableAngular(
        ColorTp&& color,
        Args&&... positions)
            : Angular<Dim, Spec>{{
                VertexTraits::buildVertex(
                    std::forward<Args>(positions),
                    std::forward<ColorTp>(color))...}} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    template <
        AllConvertible<
            typename ResizableAngular<Dim, Spec>::Vector>... Args>
    ResizableAngular<Dim, Spec>::ResizableAngular(Args&&... positions)
        : Angular<Dim, Spec>{{VertexTraits::buildVertex(
                std::forward<Args>(positions), Color{})...}} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    template <typename... Args>
        requires std::constructible_from<
            typename ResizableAngular<Dim, Spec>::Vertex, Args...>
    void ResizableAngular<Dim, Spec>::emplace(Args&&... args) {
        this->vertices.push_back(Vertex{std::forward<Args>(args)...});
    }

}


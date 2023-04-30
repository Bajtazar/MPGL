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

#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] auto
        LineLoopClickCheckerNormalizer<dim::Dim2, Spec>::operator() (
            LineLoop<dim::Dim2, Spec> const& lineLoop) const noexcept
    {
        auto normalizer = [](Vector2f const& value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return lineLoop.vertices | views::position
            | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] auto
        LineLoopClickCheckerNormalizer<dim::Dim3, Spec>::operator() (
            LineLoop<dim::Dim3, Spec> const& lineLoop) const noexcept
    {
        auto normalizer = [](auto value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return lineLoop.vertices | views::position | views::project(
            lineLoop.model) | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void LineLoopDrawer<dim::Dim2, Spec>::operator() (
        LineLoop<dim::Dim2, Spec> const& lineLoop) const noexcept
    {
        lineLoop.actualizeBufferBeforeDraw();
        lineLoop.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{lineLoop.vertexArray};
        lineLoop.vertexArray.drawArrays(
            VertexArray::DrawMode::LineLoop,
            lineLoop.vertices.size());
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void LineLoopDrawer<dim::Dim3, Spec>::operator() (
        LineLoop<dim::Dim3, Spec> const& lineLoop) const noexcept
    {
        lineLoop.actualizeBufferBeforeDraw();
        lineLoop.shaderProgram->use();
        lineLoop.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{lineLoop.vertexArray};
        lineLoop.vertexArray.drawArrays(
            VertexArray::DrawMode::LineLoop,
            lineLoop.vertices.size());
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool
        LineLoopClickChecker<Dim, Spec>::operator() (
            LineLoop<Dim, Spec> const& lineLoop,
            Vector2u const& position) const noexcept
    {
        Vector2f pos = Adapter2D{position}.get();
        auto range = Normalizer{}(lineLoop);
        auto first = *range.begin();
        for (Vector2f last : range | std::views::drop(1)) {
            if (isOnLine(pos, first, last))
                return true;
            first = last;
        }
        return isOnLine(pos, first, *range.begin());
    }

}

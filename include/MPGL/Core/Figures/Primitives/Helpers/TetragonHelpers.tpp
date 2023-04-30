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
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void TetragonDrawer<dim::Dim2, Spec>::operator() (
        Tetragon<dim::Dim2, Spec> const& tetragon) const noexcept
    {
        tetragon.actualizeBufferBeforeDraw();
        tetragon.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{tetragon.vertexArray};
        tetragon.vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void TetragonDrawer<dim::Dim3, Spec>::operator() (
        Tetragon<dim::Dim3, Spec> const& tetragon) const noexcept
    {
        tetragon.actualizeBufferBeforeDraw();
        tetragon.shaderProgram->use();
        tetragon.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{tetragon.vertexArray};
        tetragon.vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool TetragonClickChecker<dim::Dim2, Spec>::operator() (
        Tetragon<dim::Dim2, Spec> const& tetragon,
        Vector2u const& position) const noexcept
    {
        auto const p = Adapter2D{position}.get();
        auto const v0 = get<"position">(tetragon.vertices[0]).get();
        auto const v1 = get<"position">(tetragon.vertices[1]).get();
        auto const v2 = get<"position">(tetragon.vertices[2]).get();
        auto const v3 = get<"position">(tetragon.vertices[3]).get();
        return isInsideTriangle(p, v0, v1, v2) ||
            isInsideTriangle(p, v0, v2, v3);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] bool TetragonClickChecker<dim::Dim3, Spec>::operator() (
        Tetragon<dim::Dim3, Spec> const& tetragon,
        Vector2u const& position) const noexcept
    {
        auto iter = (tetragon.vertices | views::position
            | views::project(tetragon.model)).begin();
        auto const p = Adapter2D{position}.get();
        auto const v0 = Adapter2D{*iter++}.get();
        auto const v1 = Adapter2D{*iter++}.get();
        auto const v2 = Adapter2D{*iter++}.get();
        auto const v3 = Adapter2D{*iter++}.get();
        return isInsideTriangle(p, v0, v1, v2) ||
            isInsideTriangle(p, v0, v2, v3);
    }

}

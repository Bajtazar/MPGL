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
    void TriangleDrawer<dim::Dim2, Spec>::operator() (
        Triangle<dim::Dim2, Spec> const& triangle) const noexcept
    {
        triangle.actualizeBufferBeforeDraw();
        triangle.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{triangle.vertexArray};
        triangle.vertexArray.drawArrays(
            VertexArray::DrawMode::Triangles, 3);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void TriangleDrawer<dim::Dim3, Spec>::operator() (
        Triangle<dim::Dim3, Spec> const& triangle) const noexcept
    {
        triangle.actualizeBufferBeforeDraw();
        triangle.shaderProgram->use();
        triangle.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{triangle.vertexArray};
        triangle.vertexArray.drawArrays(
            VertexArray::DrawMode::Triangles, 3);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool TriangleClickChecker<dim::Dim2, Spec>::operator() (
        Triangle<dim::Dim2, Spec> const& triangle,
        Vector2u const& position) const noexcept
    {
        return isInsideTriangle(Adapter2D{position}.get(),
            get<"position">(triangle.vertices[0]).get(),
            get<"position">(triangle.vertices[1]).get(),
            get<"position">(triangle.vertices[2]).get());
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool TriangleClickChecker<dim::Dim3, Spec>::operator() (
        Triangle<dim::Dim3, Spec> const& triangle,
        Vector2u const& position) const noexcept
    {
        auto iter = (triangle.vertices | views::position
            | views::project(triangle.model)).begin();
        return isInsideTriangle(Adapter2D{position}.get(),
            Adapter2D{*iter++}.get(), Adapter2D{*iter++}.get(),
            Adapter2D{*iter++}.get());
    }

}

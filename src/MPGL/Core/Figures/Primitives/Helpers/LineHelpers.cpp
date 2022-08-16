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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Primitives/Line.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void LineDrawer<dim::Dim2, Spec>::operator() (
        Line<dim::Dim2, Spec> const& line) const noexcept
    {
        line.actualizeBufferBeforeDraw();
        line.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{line.vertexArray};
        line.vertexArray.drawArrays(VertexArray::DrawMode::Lines, 2);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void LineDrawer<dim::Dim3, Spec>::operator() (
        Line<dim::Dim3, Spec> const& line) const noexcept
    {
        line.actualizeBufferBeforeDraw();
        line.shaderProgram->use();
        line.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{line.vertexArray};
        line.vertexArray.drawArrays(VertexArray::DrawMode::Lines, 2);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool LineClickChecker<dim::Dim2, Spec>::operator() (
        Line<dim::Dim2, Spec> const& line,
        Vector2u const& position) const noexcept
    {
        return isOnLine(Adapter2D{position}.get(),
            get<"position">(line.vertices.front()).get(),
            get<"position">(line.vertices.back()).get());
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] bool LineClickChecker<dim::Dim3, Spec>::operator() (
        Line<dim::Dim3, Spec> const& line,
        Vector2u const& position) const noexcept
    {
        auto iter = (line.vertices | views::position
            | views::project(line.model)).begin();
        auto begin = Adapter2D{*iter++}.get();
        auto end = Adapter2D{*iter++}.get();
        return isOnLine(Adapter2D{position}.get(), begin, end);
    }

}

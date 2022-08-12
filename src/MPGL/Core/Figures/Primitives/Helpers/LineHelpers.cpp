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
#include <MPGL/Core/Figures/Views.hpp>

#include <limits>

namespace mpgl {

    void LineDrawer<dim::Dim2, void>::operator() (
        Line<dim::Dim2, void> const& line) const noexcept
    {
        line.actualizeBufferBeforeDraw();
        line.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{line.vertexArray};
        line.vertexArray.drawArrays(VertexArray::DrawMode::Lines, 2);
    }

    void LineDrawer<dim::Dim3, void>::operator() (
        Line<dim::Dim3, void> const& line) const noexcept
    {
        line.actualizeBufferBeforeDraw();
        line.shaderProgram->use();
        line.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{line.vertexArray};
        line.vertexArray.drawArrays(VertexArray::DrawMode::Lines, 2);
    }

    [[nodiscard]] bool LineClickChecker<dim::Dim2, void>::operator() (
        Line<dim::Dim2, void> const& line,
        Vector2u const& position) const noexcept
    {
        Vector2f normalized = Adapter2D{position}.get();
        Vector2f begin = get<"position">(line.vertices.front()).get();
        Vector2f end = get<"position">(line.vertices.back()).get();
        if (!between(begin, end, normalized))
            return false;
        return std::fabs(cross(normalized - begin, end - begin))
            < std::numeric_limits<float>::epsilon();
    }

    [[nodiscard]] bool LineClickChecker<dim::Dim3, void>::operator() (
        Line<dim::Dim3, void> const& line,
        Vector2u const& position) const noexcept
    {
        auto iter = (line.vertices | views::position
            | views::project(line.model)).begin();
        Vector2f normalized = Adapter2D{position}.get();
        Vector2f begin = Adapter2D{*iter++}.get();
        Vector2f end = Adapter2D{*iter++}.get();
        if (!between(begin, end, normalized))
            return false;
        return std::fabs(cross(normalized - begin, end - begin))
            < std::numeric_limits<float>::epsilon();
    }

}

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
#include <MPGL/Core/Figures/Primitives/Polygon.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    [[nodiscard]] auto
        PolygonClickChecker<dim::Dim2, void>::Normalizer::operator() (
            Polygon<dim::Dim2, void> const& polygon) const noexcept
    {
        auto normalizer = [](Vector2f const& value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return polygon | views::position
            | std::views::transform(normalizer);
    }

    [[nodiscard]] auto
        PolygonClickChecker<dim::Dim3, void>::Normalizer::operator() (
            Polygon<dim::Dim3, void> const& polygon) const noexcept
    {
        auto normalizer = [](auto value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return polygon | views::position | views::project(
            polygon.model) | std::views::transform(normalizer);
    }

    void PolygonDrawer<dim::Dim2, void>::operator() (
        Polygon<dim::Dim2, void> const& polygon) const noexcept
    {
        polygon.actualizeBufferBeforeDraw();
        polygon.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{polygon.vertexArray};
        polygon.vertexArray.drawArrays(
            VertexArray::DrawMode::TriangleFan,
            polygon.vertices.size());
    }

    void PolygonDrawer<dim::Dim3, void>::operator() (
        Polygon<dim::Dim3, void> const& polygon) const noexcept
    {
        polygon.actualizeBufferBeforeDraw();
        polygon.shaderProgram->use();
        polygon.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{polygon.vertexArray};
        polygon.vertexArray.drawArrays(
            VertexArray::DrawMode::TriangleFan,
            polygon.vertices.size());
    }

    template <Dimension Dim>
    [[nodiscard]] bool
        PolygonClickChecker<Dim, void>::operator() (
            Polygon<Dim, void> const& polygon,
            Vector2u const& position) const noexcept
    {
        Vector2d normalized = Adapter2D{position}.get();
        auto range = Normalizer{}(polygon);
        auto begin = *range.begin(), last = *(++range.begin());
        for (auto pos : range | std::views::drop(2)) {
            if (isInsideTriangle(normalized, begin, last, pos))
                return true;
            last = pos;
        }
        return false;
    }

}

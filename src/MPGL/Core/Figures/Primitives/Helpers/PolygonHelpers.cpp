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

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] auto
        PolygonClickCheckerNormalizer<dim::Dim2, Spec>::operator() (
            Polygon<dim::Dim2, Spec> const& polygon) const noexcept
    {
        auto normalizer = [](Vector2f const& value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return polygon.vertices | views::position
            | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] auto
        PolygonClickCheckerNormalizer<dim::Dim3, Spec>::operator() (
            Polygon<dim::Dim3, Spec> const& polygon) const noexcept
    {
        auto normalizer = [](auto value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return polygon.vertices | views::position | views::project(
            polygon.model) | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void PolygonDrawer<dim::Dim2, Spec>::operator() (
        Polygon<dim::Dim2, Spec> const& polygon) const noexcept
    {
        polygon.actualizeBufferBeforeDraw();
        polygon.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{polygon.vertexArray};
        polygon.vertexArray.drawArrays(
            VertexArray::DrawMode::TriangleFan,
            polygon.vertices.size());
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void PolygonDrawer<dim::Dim3, Spec>::operator() (
        Polygon<dim::Dim3, Spec> const& polygon) const noexcept
    {
        polygon.actualizeBufferBeforeDraw();
        polygon.shaderProgram->use();
        polygon.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{polygon.vertexArray};
        polygon.vertexArray.drawArrays(
            VertexArray::DrawMode::TriangleFan,
            polygon.vertices.size());
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool
        PolygonClickChecker<Dim, Spec>::operator() (
            Polygon<Dim, Spec> const& polygon,
            Vector2u const& position) const noexcept
    {
        Vector2f normalized = Adapter2D{position}.get();
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

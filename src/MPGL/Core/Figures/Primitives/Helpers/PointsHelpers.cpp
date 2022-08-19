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
#include <MPGL/Core/Figures/Primitives/Points.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] auto
        PointsClickCheckerNormalizer<dim::Dim2, Spec>::operator() (
            Points<dim::Dim2, Spec> const& points) const noexcept
    {
        auto normalizer = [](Vector2f const& value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return points.vertices | views::position
            | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] auto
        PointsClickCheckerNormalizer<dim::Dim3, Spec>::operator() (
            Points<dim::Dim3, Spec> const& points) const noexcept
    {
        auto normalizer = [](auto value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return points.vertices | views::position | views::project(
            points.model) | std::views::transform(normalizer);
    }

    template <AngularTraitSpecifier<dim::Dim2> Spec>
    void PointsDrawer<dim::Dim2, Spec>::operator() (
        Points<dim::Dim2, Spec> const& points) const noexcept
    {
        points.actualizeBufferBeforeDraw();
        points.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{points.vertexArray};
        points.vertexArray.drawArrays(VertexArray::DrawMode::Points,
            points.vertices.size());
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void PointsDrawer<dim::Dim3, Spec>::operator() (
        Points<dim::Dim3, Spec> const& points) const noexcept
    {
        points.actualizeBufferBeforeDraw();
        points.shaderProgram->use();
        points.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{points.vertexArray};
        points.vertexArray.drawArrays(VertexArray::DrawMode::Points,
            points.vertices.size());
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool
        PointsClickChecker<Dim, Spec>::operator() (
            Points<Dim, Spec> const& points,
            Vector2u const& position) const noexcept
    {
        Vector2f normalized = Adapter2D{position}.get();
        for (auto pos : Normalizer{}(points))
            if (pos == normalized)
                return true;
        return false;
    }

}

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
#include <MPGL/Core/Figures/Primitives/LineStrip.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    [[nodiscard]] auto
        LineStripClickChecker<dim::Dim2, void>::Normalizer::operator() (
            LineStrip<dim::Dim2, void> const& lineStrip
                ) const noexcept
    {
        auto normalizer = [](Vector2f const& value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return lineStrip | views::position
            | std::views::transform(normalizer);
    }

    [[nodiscard]] auto
        LineStripClickChecker<dim::Dim3, void>::Normalizer::operator() (
            LineStrip<dim::Dim3, void> const& lineStrip
                ) const noexcept
    {
        auto normalizer = [](auto value) -> Vector2f {
            return Adapter2D{value}.get();
        };
        return lineStrip | views::position | views::project(
            lineStrip.model) | std::views::transform(normalizer);
    }

    void LineStripDrawer<dim::Dim2, void>::operator() (
        LineStrip<dim::Dim2, void> const& lineStrip) const noexcept
    {
        lineStrip.actualizeBufferBeforeDraw();
        lineStrip.shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{lineStrip.vertexArray};
        lineStrip.vertexArray.drawArrays(
            VertexArray::DrawMode::LineStrip,
            lineStrip.vertices.size());
    }

    void LineStripDrawer<dim::Dim3, void>::operator() (
        LineStrip<dim::Dim3, void> const& lineStrip) const noexcept
    {
        lineStrip.actualizeBufferBeforeDraw();
        lineStrip.shaderProgram->use();
        lineStrip.actualizeLocations();
        BindGuard<VertexArray> vaoGuard{lineStrip.vertexArray};
        lineStrip.vertexArray.drawArrays(
            VertexArray::DrawMode::LineStrip,
            lineStrip.vertices.size());
    }

    template <Dimension Dim>
    [[nodiscard]] bool
        LineStripClickChecker<Dim, void>::operator() (
            LineStrip<Dim, void> const& lineStrip,
            Vector2u const& position) const noexcept
    {
        Vector2f pos = Adapter2D{position}.get();
        auto range = Normalizer{}(lineStrip);
        auto first = *range.begin();
        for (Vector2f last : range | std::views::drop(1)) {
            if (isOnLine(pos, first, last))
                return true;
            first = last;
        }
        return false;
    }

}

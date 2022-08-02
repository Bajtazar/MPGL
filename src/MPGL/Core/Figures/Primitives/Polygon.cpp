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

#include <numbers>
#include <iterator>

namespace mpgl {

    Polygon::Polygon(Vector2f const& center,
        float32 radius, std::size_t segments,
        Color const& color)
            : ResizableAngular{segments + 1, color}
    {
        float32 increment = 2.f *
            std::numbers::pi_v<float32> / (segments - 1), angle = 0.f;
        get<"position">(vertices.front()) = center;
        for (auto& position : vertices | std::views::drop(1) |
            views::position)
        {
            position = polarToCartesian(Vector2f{radius, angle}) + center;
            angle += increment;
        }
    }

    void Polygon::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawArrays(VertexArray::DrawMode::TriangleFan,
            vertices.size());
    }

    [[nodiscard]] bool Polygon::insideSubtriangle(
        Vector2d const& position,
        Vector2d const& firstVertex,
        Vector2d const& secondVertex,
        Vector2d const& thirdVertex) const noexcept
    {
        Vector2d v1 = secondVertex - firstVertex;
        Vector2d v2 = thirdVertex - firstVertex;
        double base = cross(v1, v2);
        double a = (cross(position, v2) - cross(firstVertex, v2)) / base;
        double b = (cross(firstVertex, v1) - cross(position, v1)) / base;
        return (a >= 0) && (b >= 0) && (a + b <= 1);
    }

    [[nodiscard]] bool Polygon::contains(
        Vector2f const& position) const noexcept
    {
        Vector2d normalized = Adapter2D{position}.get();
        for (std::size_t i = 2; i < size(); ++i)
            if (insideSubtriangle(normalized,
                get<"position">(vertices[0]).get(),
                get<"position">(vertices[i - 1]).get(),
                get<"position">(vertices[i]).get()))
                    return true;
        return false;
    }

}
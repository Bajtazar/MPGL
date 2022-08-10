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
#include <MPGL/Core/Figures/Primitives/Triangle.hpp>

#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

namespace mpgl {

    Triangle::Triangle(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color)
            : Angular{{Vertex{firstVertex, color},
                Vertex{secondVertex, color},
                Vertex{thirdVertex, color}}} {}

    Triangle::Triangle(Color const& color)
        : Angular{3, color} {}

    void Triangle::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawArrays(VertexArray::DrawMode::Triangles, 3);
    }

    [[nodiscard]] bool Triangle::contains(
        Vector2u const& position) const noexcept
    {
        Vector2d v = Adapter2D{position}.get();
        Vector2d v0 = get<"position">(vertices[0]).get();
        Vector2d v1 = Vector2d{get<"position">(vertices[1]).get()} - v0;
        Vector2d v2 = Vector2d{get<"position">(vertices[2]).get()} - v0;
        double base = cross(v1, v2);
        double a = (cross(v, v2) - cross(v0, v2)) / base;
        double b = (cross(v0, v1) - cross(v, v1)) / base;
        return (a >= 0) && (b >= 0) && (a + b <= 1);
    }

}

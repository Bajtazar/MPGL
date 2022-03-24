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
#include "Tetragon.hpp"

#include "../../Context/Buffers/BindGuard.hpp"

#include <iostream>

namespace mpgl {

    Tetragon::Tetragon(Color const& color) : Angular{4, color} {
        initElementBuffer();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color)
            : Angular{Vertices{
                Vertex{firstVertex, color},
                Vertex{firstVertex + 1._y * dimensions[1], color},
                Vertex{firstVertex + dimensions, color},
                Vertex{firstVertex + 1._x * dimensions[0], color}
            }}
    {
        initElementBuffer();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color)
            : Angular{Vertices{
                Vertex{firstVertex, color},
                Vertex{secondVertex, color},
                Vertex{thirdVertex, color},
                Vertex{firstVertex - secondVertex
                    + thirdVertex, color}
            }}
    {
        initElementBuffer();
    }

    Tetragon::Tetragon(Tetragon const& tetragon)
        : Angular{tetragon}
    {
        initElementBuffer();
    }

    void Tetragon::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
    }

    void Tetragon::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

    [[nodiscard]] bool Tetragon::insideSubtriangle(
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

    [[nodiscard]] bool Tetragon::contains(
        Vector2f const& position) const noexcept
    {
        Vector2d normalized = Adapter<Vector2f>{position}.get();
        bool first = insideSubtriangle(
            normalized, get<"position">(vertices[0]).get(),
            get<"position">(vertices[1]).get(),
            get<"position">(vertices[2]).get());
        bool second = insideSubtriangle(
            normalized, get<"position">(vertices[0]).get(),
            get<"position">(vertices[2]).get(),
            get<"position">(vertices[3]).get());
        return first || second;
    }

}

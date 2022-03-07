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
#include "Ellipse.hpp"
#include "../Views.hpp"
#include "../../../Mathematics/Systems.hpp"
#include "../../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    Ellipse::Executable const Ellipse::shaderExec
        = [](ProgramPtr& program)
    {
        program->use();
        program->setUniform("aafactor", (float)
            context.windowOptions.antiAliasingSamples / 4.f);
    };

    Ellipse::Vertices Ellipse::ellipseVertices(Vector2f const& center,
        Vector2f const& semiAxis, float32 angle)
    {
        Matrix2f rotation = rotationMatrix<float32>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            Vertex{center - rot2},
            Vertex{center + rot1},
            Vertex{center + rot2},
            Vertex{center - rot1}
        };
    }

    Ellipse::Vertices Ellipse::circleVertices(Vector2f const& center,
        float32 radius)
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            Vertex{center - semiMajor + semiMinor},
            Vertex{center + semiMajor + semiMinor},
            Vertex{center + semiMajor - semiMinor},
            Vertex{center - semiMajor - semiMinor}
        };
    }

    Ellipse::Ellipse(Vector2f const& center, Vector2f const& semiAxis,
        Color const& color, float angle)
            : Elliptic{ellipseVertices(center, semiAxis, angle), "2DEllipse",
                shaderExec, color}
    {
        actualizeOutline();
    }

    Ellipse::Ellipse(Vector2f const& center, float radius,
        Color const& color)
            : Elliptic{circleVertices(center, radius), "2DEllipse",
                shaderExec, color}
    {
        actualizeOutline();
    }

    void Ellipse::actualizeOutline(void) noexcept {
        outlineTransform = *invert(transpose(
            Matrix2f{Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])},
            Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}}));
    }

    Vector2f Ellipse::getCenter(void) const noexcept {
        return (Vector2f{get<"position">(vertices[3])}
            + Vector2f{get<"position">(vertices[1])}) / 2.f;
    }

    Vector2f Ellipse::getSemiAxis(void) const noexcept {
        return {
            (Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])}).length(),
            (Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}).length()
        };
    }

    void Ellipse::onScreenTransformation(Vector2u const& oldDimensions) noexcept {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(oldDimensions)
                / static_cast<Vector2f>(context.windowDimensions) - 1.f;
        }
        actualizeOutline();
        isModified = true;
    }

    void Ellipse::translate(Vector2f const& shift) noexcept {
        for (auto& vertexPosition : vertices | views::position)
            vertexPosition = Vector2f(vertexPosition) + shift;
        actualizeOutline();
        isModified = true;
    }

    void Ellipse::scale(Vector2f const& center, float32 factor) noexcept {
        for (auto& vertexPosition : vertices | views::position)
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        actualizeOutline();
        isModified = true;
    }

    void Ellipse::rotate(Vector2f const& center, float32 angle) noexcept {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Ellipse::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotation * radius + center;
        }
        actualizeOutline();
        isModified = true;
    }

    void Ellipse::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        shaderProgram->setUniform("color", color);
        shaderProgram->setUniform("shift",
            Vector2f{get<"position">(vertices.front())});
        shaderProgram->setUniform("transform", outlineTransform);
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

}

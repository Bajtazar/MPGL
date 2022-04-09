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
#include <MPGL/Core/Figures/Primitives/Ring.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    Ring::InnerEllipse::Vertices
        Ring::InnerEllipse::ellipseVertices(Vector2f const& center,
            Vector2f const& semiAxis, float32 angle)
    {
        Matrix2f rotation = rotationMatrix<float32>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            center - rot2,
            center + rot1,
            center + rot2,
            center - rot1
        };
    }

    Ring::InnerEllipse::Vertices
        Ring::InnerEllipse::circleVertices(Vector2f const& center,
            float32 radius)
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            center - semiMajor + semiMinor,
            center + semiMajor + semiMinor,
            center + semiMajor - semiMinor,
            center - semiMajor - semiMinor
        };
    }

    Ring::InnerEllipse::InnerEllipse(Vector2f const& center,
        Vector2f const& semiAxis, float32 angle)
            : vertices{ellipseVertices(center, semiAxis, angle)}
    {
        actualizeMatrices();
    }

    Ring::InnerEllipse::InnerEllipse(Vector2f const& center,
        float radius)
            : vertices{circleVertices(center, radius)}
    {
        actualizeMatrices();
    }

    void Ring::InnerEllipse::actualizeMatrices(void) noexcept {
        outline = *invert(transpose(
            Matrix2f{
                Vector2f{vertices[1]} - Vector2f{vertices[0]},
                Vector2f{vertices[3]} - Vector2f{vertices[0]}
            }
        ));
    }

    [[nodiscard]] Vector2f
        Ring::InnerEllipse::getCenter(void) const noexcept
    {
        return (Vector2f{vertices[3]} + Vector2f{vertices[1]}) / 2.f;
    }

    [[nodiscard]] Vector2f
        Ring::InnerEllipse::getSemiAxis(void) const noexcept
    {
        return {
            (Vector2f{vertices[1]} - Vector2f{vertices[0]}).length(),
            (Vector2f{vertices[3]} - Vector2f{vertices[0]}).length()
        };
    }

    void Ring::InnerEllipse::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        for (auto& position : vertices | std::views::transform(
            (Vector2f&(Vertex::*)(void))&Vertex::get))
        {
            position = (position + 1.f) * static_cast<Vector2f>(
                oldDimensions) / static_cast<Vector2f>(
                    context.windowDimensions) - 1.f;
        }
        actualizeMatrices();
    }

    void Ring::InnerEllipse::translate(
        Vector2f const& shift) noexcept
    {
        for (auto& position : vertices)
            position = Vector2f(position) + shift;
        actualizeMatrices();
    }

    void Ring::InnerEllipse::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& position : vertices)
            position = (static_cast<Vector2f>(position)
                - center) * factor + center;
        actualizeMatrices();
    }

    void Ring::InnerEllipse::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Ring::InnerEllipse::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& position : vertices)
            position = rotation * (
                Vector2f(position) - center) + center;
        actualizeMatrices();
    }

    Ring::Executable const Ring::shaderExec
        = [](ProgramPtr& program)
    {
        program->use();
        program->setUniform("aafactor", (float32)
            context.windowOptions.antiAliasingSamples / 4.f);
    };


    Ring::Ring(
        Vector2f const& center,
        Vector2f const& semiAxis,
        InnerEllipse const& innerEllipse,
        Color const& color,
        float32 angle)
            : Elliptic{ellipseVertices(center, semiAxis, angle),
                "MPGL/2D/Ring", shaderExec, color},
            innerEllipse{innerEllipse}
    {
        actualizeMatrices();
    }

    Ring::Ring(
        Vector2f const& center,
        float32 radius,
        InnerEllipse const& innerEllipse,
        Color const& color)
            : Elliptic{circleVertices(center, radius),
                "MPGL/2D/Ring", shaderExec, color},
            innerEllipse{innerEllipse}
    {
        actualizeMatrices();
    }

    Ring::Ring(
        Vector2f const& center,
        Vector2f const& outerSemiAxis,
        Vector2f const& innerSemiAxis,
        Color const& color,
        float32 angle) : Ring{center, outerSemiAxis,
            InnerEllipse{center, innerSemiAxis,
                angle}, color, angle} {}

    Ring::Ring(
        Vector2f const& center,
        float32 outerRadius,
        float32 innerRadius,
        Color const& color) : Ring{center, outerRadius,
            InnerEllipse{center, innerRadius}, color} {}

    void Ring::actualizeMatrices(void) noexcept {
        outline = *invert(transpose(
            Matrix2f{Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])},
            Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}}));
    }

    [[nodiscard]] Vector2f Ring::getCenter(void) const noexcept {
        return (Vector2f{get<"position">(vertices[3])}
            + Vector2f{get<"position">(vertices[1])}) / 2.f;
    }

    [[nodiscard]] Vector2f Ring::getOuterSemiAxis(
        void) const noexcept
    {
        return {
            (Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])}).length(),
            (Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}).length()
        };
    }

    void Ring::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        innerEllipse.onScreenTransformation(oldDimensions);
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(
                oldDimensions) / static_cast<Vector2f>(
                    context.windowDimensions) - 1.f;
        }
        actualizeMatrices();
        isModified = true;
    }

    void Ring::translate(Vector2f const& shift) noexcept {
        innerEllipse.translate(shift);
        for (auto& position : vertices | views::position)
            position = Vector2f(position) + shift;
        actualizeMatrices();
        isModified = true;
    }

    void Ring::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        innerEllipse.scale(center, factor);
        for (auto& position : vertices | views::position)
            position = (static_cast<Vector2f>(position)
                - center) * factor + center;
        actualizeMatrices();
        isModified = true;
    }

    void Ring::rotate(Vector2f const& center, float32 angle) noexcept {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Ring::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        innerEllipse.rotate(center, rotation);
        for (auto& position : vertices | views::position)
            position = rotation * (
                Vector2f(position) - center) + center;
        actualizeMatrices();
        isModified = true;
    }

    void Ring::setUniforms(void) const noexcept {
        shaderProgram->setUniform("color", color);
        shaderProgram->setUniform("outerShift",
            Vector2f{get<"position">(vertices.front())});
        shaderProgram->setUniform("innerShift",
            Vector2f{innerEllipse.vertices.front()});
        shaderProgram->setUniform("outerTransform", outline);
        shaderProgram->setUniform("innerTransform", innerEllipse.outline);
    }

    void Ring::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        setUniforms();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

    [[nodiscard]] bool Ring::insideSystem(
            Vector2f const& position,
            Vector2f const& shift,
            Matrix2f const& transform) const noexcept
    {
        Vector2f local = transform * (position - shift);
        return (local - 0.5f).length() <= 0.5;
    }

    [[nodiscard]] bool Ring::contains(
        Vector2f const& position) const noexcept
    {
        bool outring = insideSystem(position,
            get<"position">(vertices.front()), this->outline);
        bool inring = insideSystem(position,
            innerEllipse.vertices.front(), innerEllipse.outline);
        return outring && (!inring);
    }

}

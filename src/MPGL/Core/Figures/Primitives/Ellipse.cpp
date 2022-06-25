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
#include <MPGL/Core/Figures/Primitives/Ellipse.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Utility/DelegationWrapper.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    Ellipse::Executable const Ellipse::shaderExec
        = [](ShaderProgram const& program)
    {
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    };

    void Ellipse::setLocations(void) {
        Shadeable::setLocations(DelegationWrapper{
            shaderProgram, locations}([](auto program, auto locations)
        {
            locations->color = ShaderLocation{*program, "color"};
            locations->shift = ShaderLocation{*program, "shift"};
            locations->transform
                = ShaderLocation{*program, "transform"};
        }));
    }

    Ellipse::Ellipse(Vector2f const& center, Vector2f const& semiAxis,
        Color const& color, float angle)
            : Elliptic{ellipseVertices(center, semiAxis, angle),
                "MPGL/2D/Ellipse", shaderExec, color},
            locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    Ellipse::Ellipse(Vector2f const& center, float radius,
        Color const& color)
            : Elliptic{circleVertices(center, radius),
                "MPGL/2D/Ellipse", shaderExec, color},
            locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    void Ellipse::actualizeMatrices(void) noexcept {
        outlineTransform = *invert(transpose(
            Matrix2f{Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])},
            Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}}));
    }

    [[nodiscard]] Vector2f Ellipse::getCenter(void) const noexcept {
        return (Vector2f{get<"position">(vertices[3])}
            + Vector2f{get<"position">(vertices[1])}) / 2.f;
    }

    [[nodiscard]] Vector2f Ellipse::getSemiAxis(void) const noexcept {
        return {
            (Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])}).length(),
            (Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}).length()
        };
    }

    void Ellipse::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(
                oldDimensions) / static_cast<Vector2f>(
                    context.windowDimensions) - 1.f;
        }
        actualizeMatrices();
        isModified = true;
    }

    void Ellipse::translate(Vector2f const& shift) noexcept {
        for (auto& position : vertices | views::position)
            position = Vector2f(position) + shift;
        actualizeMatrices();
        isModified = true;
    }

    void Ellipse::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& position : vertices | views::position)
            position = (static_cast<Vector2f>(position)
                - center) * factor + center;
        actualizeMatrices();
        isModified = true;
    }

    void Ellipse::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Ellipse::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& position : vertices | views::position)
            position = rotation * (
                Vector2f(position) - center) + center;
        actualizeMatrices();
        isModified = true;
    }

    void Ellipse::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        locations->color(color);
        locations->shift(Vector2f{get<"position">(vertices.front())});
        locations->transform(outlineTransform);
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

    void Ellipse::setShader(ShaderProgram const& program) noexcept {
        Shadeable::setShader(program);
        shaderExec(program);
    }

    void Ellipse::setShader(ShaderProgram&& program) noexcept {
        Shadeable::setShader(std::move(program));
        shaderExec(*this->shaderProgram);
    }

    void Ellipse::setShader(std::string const& name) {
        Shadeable::setShader(name, shaderExec);
    }

    [[nodiscard]] bool Ellipse::contains(
        Vector2f const& position) const noexcept
    {
        Vector2f local = outlineTransform * (position -
            Vector2f{get<"position">(vertices.front())});
        return (local - 0.5f).length() <= 0.5f;
    }

}

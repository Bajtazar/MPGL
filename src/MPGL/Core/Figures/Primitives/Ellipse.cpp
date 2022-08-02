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
#include <MPGL/Utility/Deferred/DeferredExecutionWrapper.hpp>
#include <MPGL/Core/Figures/Primitives/Ellipse.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
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
        Shadeable::setLocations(DeferredExecutionWrapper{
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

    std::optional<Matrix2f> Ellipse::calculateNewOutline(
        void) const noexcept
    {
        return invert(transpose(
            Matrix2f{Vector2f{get<"position">(vertices[1])}
                - Vector2f{get<"position">(vertices[0])},
            Vector2f{get<"position">(vertices[3])}
                - Vector2f{get<"position">(vertices[0])}}));
    }

    void Ellipse::actualizeMatrices(void) noexcept {
        if (auto outline = calculateNewOutline())
            outlineTransform = *outline;
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
        Layout& layout,
        Vector2u const& oldDimensions) noexcept
    {
        any::InputRange<Adapter2D> positions{
            vertices | views::position};
        layout(positions, oldDimensions);
        actualizeMatrices();
        isModified = true;
    }

    void Ellipse::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{
            vertices | views::position};
        transformator(positions);
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

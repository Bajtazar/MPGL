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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Textures/EllipseSprite.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <bool IsColorable>
    std::string EllipseSprite<IsColorable>::shaderName(void) noexcept {
        if constexpr (IsColorable)
            return "MPGL/2D/CTEllipse";
        else
            return "MPGL/2D/TEllipse";
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setLocations(void) {
        Shadeable::setLocations(DeferredExecutionWrapper{
            this->shaderProgram, locations}(
                [](auto program, auto locations)
            {
                locations->shift = ShaderLocation{*program, "shift"};
                locations->transform
                    = ShaderLocation{*program, "transform"};
            }
        ));
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& semiAxis,
        float angle)
            : EllipticSprite<IsColorable>{
                this->ellipsePositions(center, semiAxis, angle),
                texture, shaderName()}, locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& semiAxis,
        float angle,
        Color const& color) requires (IsColorable)
            : EllipticSprite<IsColorable>{
                this->ellipsePositions(center, semiAxis, angle),
                texture, shaderName(), color}, locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        float radius)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName()}, locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        float radius,
        Color const& color) requires (IsColorable)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName(), color}, locations{new Locations}
    {
        actualizeMatrices();
        setLocations();
    }

    template <bool IsColorable>
    std::optional<Matrix2f>
        EllipseSprite<IsColorable>::calculateNewOutline(
            void) const noexcept
    {
        return invert(transpose(
            Matrix2f{Vector2f{get<"position">(this->vertices[1])}
                - Vector2f{get<"position">(this->vertices[0])},
            Vector2f{get<"position">(this->vertices[3])}
                - Vector2f{get<"position">(this->vertices[0])}}));
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::actualizeMatrices(void) noexcept {
        if (auto outline = calculateNewOutline())
            this->outline = *outline;
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        EllipseSprite<IsColorable>::getCenter(void) const noexcept
    {
        return (Vector2f{get<"position">(this->vertices[3])}
            + Vector2f{get<"position">(this->vertices[1])}) / 2.f;
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        EllipseSprite<IsColorable>::getSemiAxis(void) const noexcept
    {
        return {
            (Vector2f{get<"position">(this->vertices[1])}
                - Vector2f{get<"position">(this->vertices[0])}).length(),
            (Vector2f{get<"position">(this->vertices[3])}
                - Vector2f{get<"position">(this->vertices[0])}).length()
        };
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::transform(
        Transformation2D const& transformator) noexcept
    {
        EllipticSprite<IsColorable>::transform(transformator);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        EllipticSprite<IsColorable>::setShader(program);
        setLocations();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        EllipticSprite<IsColorable>::setShader(std::move(program));
        setLocations();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setShader(
        std::string const& name)
    {
        EllipticSprite<IsColorable>::setShader(name);
        setLocations();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(IsColorable ?
            "MPGL/2D/CTEllipseConv" : "MPGL/2D/TEllipseConv");
        Shadeable::setLocations(DeferredExecutionWrapper{this->shaderProgram}(
            [](auto program, auto convolution, auto dimensions) {
                ShaderLocation{*program, "convolution"}(convolution);
                ShaderLocation{*program, "screen"}(dimensions);
            }, convolution, this->texture.getTextureDimensions()
        ));
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::resetConvolution(void) {
        this->setShader(shaderName());
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::draw(void) const noexcept {
        auto const& textureBuffer = this->texture.getTextureBuffer();
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        locations->shift(
            Vector2f{get<"position">(this->vertices.front())});
        locations->transform(outline);
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <bool IsColorable>
    [[nodiscard]] bool EllipseSprite<IsColorable>::contains(
        Vector2u const& position) const noexcept
    {
        Vector2f local = outline * (
            vectorCast<float32>(position) -
            Vector2f{get<"position">(this->vertices.front())});
        return (local - 0.5f).length() <= 0.5f;
    }

}

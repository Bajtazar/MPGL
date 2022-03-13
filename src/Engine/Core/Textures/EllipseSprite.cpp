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
#include "EllipseSprite.hpp"

#include "../../Mathematics/Systems.hpp"
#include "../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    template <bool IsColorable>
    std::string EllipseSprite<IsColorable>::shaderName(void) noexcept {
        if constexpr (IsColorable)
            return "2DCTEllipse";
        else
            return "2DTEllipse";
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& semiAxis,
        float angle)
            : EllipticSprite<IsColorable>{
                this->ellipsePositions(center, semiAxis, angle),
                texture, shaderName()}
    {
        actualizeMatrices();
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
                texture, shaderName(), color}
    {
        actualizeMatrices();
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        float radius)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName()}
    {
        actualizeMatrices();
    }

    template <bool IsColorable>
    EllipseSprite<IsColorable>::EllipseSprite(
        Texture const& texture,
        Vector2f const& center,
        float radius,
        Color const& color) requires (IsColorable)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName(), color}
    {
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::actualizeMatrices(void) noexcept {
        outline = *invert(transpose(
            Matrix2f{Vector2f{get<"position">(this->vertices[1])}
                - Vector2f{get<"position">(this->vertices[0])},
            Vector2f{get<"position">(this->vertices[3])}
                - Vector2f{get<"position">(this->vertices[0])}}));
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
    void EllipseSprite<IsColorable>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        EllipticSprite<IsColorable>::onScreenTransformation(
            oldDimensions);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        EllipticSprite<IsColorable>::translate(shift);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        EllipticSprite<IsColorable>::scale(center, factor);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        EllipticSprite<IsColorable>::rotate(center, angle);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        EllipticSprite<IsColorable>::rotate(center, rotation);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void EllipseSprite<IsColorable>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(
            IsColorable ? "2DCTEllipseConv" : "2DTEllipseConv");
        this->shaderProgram->setUniform("convolution", convolution);
        this->shaderProgram->setUniform(
            "screen", this->texture.getTextureDimensions());
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
        this->shaderProgram->setUniform("shift",
            Vector2f{get<"position">(this->vertices.front())});
        this->shaderProgram->setUniform("transform", outline);
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

}

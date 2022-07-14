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
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Core/Textures/Sprite.hpp>

namespace mpgl {

    template <bool IsColorable>
    std::string Sprite<IsColorable>::shaderName(void) noexcept {
        if constexpr (IsColorable)
            return "MPGL/2D/CTexture";
        else
            return "MPGL/2D/Texture";
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Executable const
        Sprite<IsColorable>::shaderExec
            = [](ShaderProgram const& program)
    {
        ShaderLocation{program, "tex"}(0);
    };

    template <bool IsColorable>
    void Sprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        Shadeable::setShader(program);
        shaderExec(*this->shaderProgram);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        Shadeable::setShader(std::move(program));
        shaderExec(*this->shaderProgram);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setShader(
        std::string const& name)
    {
        Shadeable::setShader(name, shaderExec);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture)
        : ShadeableSprite<IsColorable>{texture, shaderName(),
            shaderExec} {}

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Texture const& texture,
        Color const& color) requires (IsColorable)
            : ShadeableSprite<IsColorable>{
                texture, shaderName(), shaderExec, color} {}

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex)
            : ShadeableSprite<IsColorable>{{
                firstVertex,
                secondVertex,
                thirdVertex + firstVertex - secondVertex,
                thirdVertex
            }, texture, shaderName(), shaderExec} {}

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color) requires (IsColorable)
            : ShadeableSprite<IsColorable>{{
                firstVertex,
                secondVertex,
                thirdVertex + firstVertex - secondVertex,
                thirdVertex
            }, texture, shaderName(), shaderExec, color} {}

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions)
            : ShadeableSprite<IsColorable>{{
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            }, texture, shaderName(), shaderExec} {}

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color) requires (IsColorable)
            : ShadeableSprite<IsColorable>{{
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            }, texture, shaderName(), shaderExec, color} {}

    template <bool IsColorable>
    void Sprite<IsColorable>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(IsColorable ?
            "MPGL/2D/CTextureConv" : "MPGL/2D/TextureConv");
        Shadeable::setLocations(DeferredExecutionWrapper{this->shaderProgram}(
            [](auto program, auto convolution, auto dimensions) {
                ShaderLocation{*program, "convolution"}(convolution);
                ShaderLocation{*program, "screen"}(dimensions);
            }, convolution, this->texture.getTextureDimensions()
        ));
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::resetConvolution(void) {
        setShader(shaderName());
    }

    template <bool IsColorable>
    [[nodiscard]] bool Sprite<IsColorable>::insideSubtriangle(
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

    template <bool IsColorable>
    [[nodiscard]] bool Sprite<IsColorable>::contains(
        Vector2f const& position) const noexcept
    {
        Vector2d normalized = Adapter<Vector2f>{position}.get();
        bool first = insideSubtriangle(
            normalized, get<"position">(this->vertices[0]).get(),
            get<"position">(this->vertices[1]).get(),
            get<"position">(this->vertices[2]).get());
        bool second = insideSubtriangle(
            normalized, get<"position">(this->vertices[0]).get(),
            get<"position">(this->vertices[2]).get(),
            get<"position">(this->vertices[3]).get());
        return first || second;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        auto const& textureBuffer = this->texture.getTextureBuffer();
        this->shaderProgram->use();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

}
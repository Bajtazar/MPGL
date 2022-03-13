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
#include "Sprite.hpp"

#include "../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    template <bool IsColorable>
    std::string Sprite<IsColorable>::shaderName(void) noexcept {
        if constexpr (IsColorable)
            return "2DCTexture";
        else
            return "2DTexture";
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Executable const
        Sprite<IsColorable>::shaderExec
            = [](Shadeable::ProgramPtr& program)
    {
        program->use();
        program->setUniform("tex", 0);
    };

    template <bool IsColorable>
    void Sprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        Shadeable::setShader(program);
        shaderExec(this->shaderProgram);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        Shadeable::setShader(std::move(program));
        shaderExec(this->shaderProgram);
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
                thirdVertex + secondVertex - firstVertex,
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
                thirdVertex + secondVertex - firstVertex,
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
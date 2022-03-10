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
#include "ShadeableSprite.hpp"

namespace mpgl {

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::Executable const
        ShadeableSprite<IsColorable>::shaderExec
            = [](ProgramPtr& program)
    {
        program->use();
        program->setUniform("tex", 0);
    };

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        Shadeable::setShader(program);
        shaderExec(shaderProgram);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        Shadeable::setShader(std::move(program));
        shaderExec(shaderProgram);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::setShader(
        std::string const& name)
    {
        Shadeable::setShader(name, shaderExec);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::onScreenTransformation(
         Vector2u const& oldDimensions) noexcept
    {
        Texturable<IsColorable>::onScreenTransformation(oldDimensions);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        Texturable<IsColorable>::translate(shift);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        Texturable<IsColorable>::scale(center, factor);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        Texturable<IsColorable>::rotate(center, angle);
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        Texturable<IsColorable>::rotate(center, rotation);
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Texture const& texture,
        std::string const& shaderName)
            : Texturable<IsColorable>{texture},
                Figure{shaderName, shaderExec} {}

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Texture const& texture,
        std::string const& shaderName,
        Color const& color) requires (IsColorable)
            : Texturable<IsColorable>{texture, color},
                Figure{shaderName, shaderExec} {}

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Positions positions,
        Texture const& texture,
        std::string const& shaderName)
            : Texturable<IsColorable>{positions, texture},
                Figure{shaderName, shaderExec} {}

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Positions positions,
        Texture const& texture,
        std::string const& shaderName,
        Color const& color) requires (IsColorable)
            : Texturable<IsColorable>{positions, texture, color},
                Figure{shaderName, shaderExec} {}

    template <bool IsColorable>
    ShadeableSprite<IsColorable>&
        ShadeableSprite<IsColorable>::operator=(
            ShadeableSprite&& sprite) noexcept
    {
        this->vertices = std::move(sprite.vertices);
        shaderProgram = std::move(sprite.shaderProgram);
        vertexArray = std::move(sprite.vertexArray);
        vertexBuffer = std::move(sprite.vertexBuffer);
        this->elementBuffer = std::move(sprite.elementBuffer);
        this->texture = std::move(sprite.texture);
        return *this;
    }

}

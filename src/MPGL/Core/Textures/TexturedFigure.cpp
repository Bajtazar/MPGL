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
#include <MPGL/Core/Textures/TexturedFigure.hpp>

namespace mpgl {

    template <InstanceOf<Figure> Base>
    TexturedFigure<Base>::Executable const
        TexturedFigure<Base>::executable =
    [](ShaderProgram const& program) {
        ShaderLocation{program, "tex"}(0);
    };

    template <InstanceOf<Figure> Base>
    TexturedFigure<Base>::Placer const TexturedFigure<Base>::placer{};

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::draw(void) const noexcept {
        auto const& textureBuffer = this->texture.getTextureBuffer();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        Base::draw();
    }

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::setConvolution(
        Matrix3f const& convolution)
    {
        if constexpr (InstanceOf<Base, Elliptic>) {
            this->setShader(Base::ShaderManager::convolutionShader);
        } else {
            this->setShader(VertexTraits::convolutionShader());
        }
        Shadeable::setLocations(DeferredExecutionWrapper{
            this->shaderProgram}(
            [](auto program, auto convolution, auto dimensions) {
                ShaderLocation{*program, "convolution"}(convolution);
                ShaderLocation{*program, "screen"}(dimensions);
            }, convolution, this->texture.getTextureDimensions()
        ));
    }

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::resetConvolution(void) {
        if constexpr (InstanceOf<Base, Elliptic>) {
            this->setShader(Base::ShaderManager::shader);
        } else {
            this->setShader(VertexTraits::convolutionShader());
        }
    }

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::setShader(
        ShaderProgram const& program) noexcept
    {
        Base::setShader(program);
        executable(*this->shaderProgram);
    }

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::setShader(
        ShaderProgram&& program) noexcept
    {
        Base::setShader(std::move(program));
        executable(*this->shaderProgram);
    }

    template <InstanceOf<Figure> Base>
    void TexturedFigure<Base>::setShader(
        std::string const& name) noexcept
    {
        Base::setShader(name);
        Shadeable::setLocations(DeferredExecutionWrapper{
            this->shaderProgram}([](auto program)
        {
            executable(*program);
        }));
    }

}

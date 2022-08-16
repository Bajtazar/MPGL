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

namespace mpgl::exp {

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    TexturedFigure<Base<Dim, Spec>>::Executable const
        TexturedFigure<Base<Dim, Spec>>::executable =
    [](ShaderProgram const& program) {
        ShaderLocation{program, "tex"}(0);
    };

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    TexturedFigure<Base<Dim, Spec>>::Placer const
        TexturedFigure<Base<Dim, Spec>>::placer{};

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::draw(void) const noexcept {
        auto const& textureBuffer = this->texture.getTextureBuffer();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        Base<Dim, Spec>::draw();
    }

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(VertexTraits::convolutionShader());
        Shadeable::setLocations(DeferredExecutionWrapper{
            this->shaderProgram}(
            [](auto program, auto convolution, auto dimensions) {
                ShaderLocation{*program, "convolution"}(convolution);
                ShaderLocation{*program, "screen"}(dimensions);
            }, convolution, this->texture.getTextureDimensions()
        ));
    }

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::resetConvolution(void) {
        setShader(VertexTraits::shader());
    }

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::setShader(
        ShaderProgram const& program) noexcept
    {
        Base<Dim, Spec>::setShader(program);
        executable(*this->shaderProgram);
    }

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::setShader(
        ShaderProgram&& program) noexcept
    {
        Base<Dim, Spec>::setShader(std::move(program));
        executable(*this->shaderProgram);
    }

    template <
        template <class, typename> class Base,
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Angular>
    void TexturedFigure<Base<Dim, Spec>>::setShader(
        std::string const& name) noexcept
    {
        Base<Dim, Spec>::setShader(name, executable);
    }

}

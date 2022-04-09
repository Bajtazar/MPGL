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
#include <MPGL/Core/Textures/EllipticSprite.hpp>

#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <bool IsColorable>
    EllipticSprite<IsColorable>::Positions
        EllipticSprite<IsColorable>::ellipsePositions(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle)
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

    template <bool IsColorable>
    EllipticSprite<IsColorable>::Positions
        EllipticSprite<IsColorable>::circlePositions(
            Vector2f const& center,
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

    template <bool IsColorable>
    EllipticSprite<IsColorable>::Executable const
        EllipticSprite<IsColorable>::shaderExec
            = [](Shadeable::ProgramPtr& program)
    {
        program->use();
        program->setUniform("tex", 0);
        program->setUniform("aafactor", (float32)
            GraphicalObject::context.windowOptions.antiAliasingSamples / 4.f);
    };

    template <bool IsColorable>
    void EllipticSprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        Shadeable::setShader(program);
        shaderExec(this->shaderProgram);
    }

    template <bool IsColorable>
    void EllipticSprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        Shadeable::setShader(std::move(program));
        shaderExec(this->shaderProgram);
    }

    template <bool IsColorable>
    void EllipticSprite<IsColorable>::setShader(
        std::string const& name)
    {
        Shadeable::setShader(name, shaderExec);
    }

    template <bool IsColorable>
    EllipticSprite<IsColorable>::EllipticSprite(
        Positions positions,
        Texture const& texture,
        std::string const& programName)
            : ShadeableSprite<IsColorable>{
                positions, texture, programName, shaderExec} {}

    template <bool IsColorable>
    EllipticSprite<IsColorable>::EllipticSprite(
        Positions positions,
        Texture const& texture,
        std::string const& programName,
        Color const& color) requires (IsColorable)
            : ShadeableSprite<IsColorable>{
                positions, texture, programName, shaderExec, color} {}

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::VertexView
        EllipticSprite<IsColorable>::operator[] (
            std::size_t index) noexcept
    {
        this->isModified = true;
        return *(iterator{this->vertices.begin() + index});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::ConstVertexView
        EllipticSprite<IsColorable>::operator[] (
            std::size_t index) const noexcept
    {
        return *(const_iterator{this->vertices.cbegin() + index});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::VertexView
        EllipticSprite<IsColorable>::front(void) noexcept
    {
        this->isModified = true;
        return *(iterator{this->vertices.begin()});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::ConstVertexView
        EllipticSprite<IsColorable>::front(void) const noexcept
    {
        return *(const_iterator{this->vertices.begin()});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::VertexView
        EllipticSprite<IsColorable>::back(void) noexcept
    {
        this->isModified = true;
        return *(iterator{this->vertices.end() - 1});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::ConstVertexView
        EllipticSprite<IsColorable>::back(void) const noexcept
    {
        return *(const_iterator{this->vertices.end() - 1});
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::iterator
        EllipticSprite<IsColorable>::begin(void) noexcept
    {
        this->isModified = true;
        return iterator{this->vertices.begin()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::iterator
        EllipticSprite<IsColorable>::end(void) noexcept
    {
        this->isModified = true;
        return iterator{this->vertices.end()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_iterator
        EllipticSprite<IsColorable>::begin(void) const noexcept
    {
        return const_iterator{this->vertices.begin()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_iterator
        EllipticSprite<IsColorable>::end(void) const noexcept
    {
        return const_iterator{this->vertices.end()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_iterator
        EllipticSprite<IsColorable>::cbegin(void) const noexcept
    {
        return const_iterator{this->vertices.cbegin()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_iterator
        EllipticSprite<IsColorable>::cend(void) const noexcept
    {
        return const_iterator{this->vertices.cend()};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::reverse_iterator
        EllipticSprite<IsColorable>::rbegin(void) noexcept
    {
        this->isModified = true;
        return reverse_iterator{iterator{this->vertices.end() - 1}};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::reverse_iterator
        EllipticSprite<IsColorable>::rend(void) noexcept
    {
        this->isModified = true;
        return reverse_iterator{iterator{this->vertices.begin() - 1}};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_reverse_iterator
        EllipticSprite<IsColorable>::rbegin(void) const noexcept
    {
        return const_reverse_iterator{
            const_iterator{this->vertices.end() - 1}};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_reverse_iterator
        EllipticSprite<IsColorable>::rend(void) const noexcept
    {
        return const_reverse_iterator{
            const_iterator{this->vertices.begin() - 1}};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_reverse_iterator
        EllipticSprite<IsColorable>::crbegin(void) const noexcept
    {
        return const_reverse_iterator{
            const_iterator{this->vertices.end() - 1}};
    }

    template <bool IsColorable>
    [[nodiscard]] EllipticSprite<IsColorable>::const_reverse_iterator
        EllipticSprite<IsColorable>::crend(void) const noexcept
    {
        return const_reverse_iterator{
            const_iterator{this->vertices.begin() - 1}};
    }

}

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
#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Core/Figures/Primitives/Ellipse.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::OutlineCalculator const
        Ellipse<Dim, Spec>::outlineCalc = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::Clicker const
        Ellipse<Dim, Spec>::clicker = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::ShaderManager const
        Ellipse<Dim, Spec>::shaderManager = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::setLocations(void) {
        auto& program = *this->shaderProgram;
        locations.shift = ShaderLocation{program, "shift"};
        locations.transform
            = ShaderLocation{program, "transform"};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::Ellipse(
        Vector2f const& center,
        Vector2f const& semiAxis,
        Color const& color,
        float angle) requires TwoDimensional<Dim>
            : Elliptic<Dim, Spec>{
                Elliptic<Dim, Spec>::ellipseVertices(
                    center, semiAxis, angle, color),
                shaderManager.shader}
    {
        shaderManager(*this->shaderProgram);
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::Ellipse(
        Vector2f const& center,
        float radius,
        Color const& color) requires TwoDimensional<Dim>
            : Elliptic<Dim, Spec>{
                Elliptic<Dim, Spec>::circleVertices(
                    center, radius, color),
                shaderManager.shader}
    {
        shaderManager(*this->shaderProgram);
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ellipse<Dim, Spec>::Ellipse(
        Vector3f const& center,
        Vector3f const& minorAxis,
        Vector3f const& majorAxis,
        Color const& color) requires ThreeDimensional<Dim>
            : Elliptic<Dim, Spec>{{
                VertexTraits::buildVertex(center - majorAxis - minorAxis, color),
                VertexTraits::buildVertex(center - majorAxis + minorAxis, color),
                VertexTraits::buildVertex(center + majorAxis + minorAxis, color),
                VertexTraits::buildVertex(center + majorAxis - minorAxis, color)
            }, shaderManager.shader}
    {
        if (dot(minorAxis, majorAxis))
            throw NotPerpendicularException{minorAxis, majorAxis};
        shaderManager(*this->shaderProgram);
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::actualizeMatrices(void) noexcept {
        if (auto outline = outlineCalc(*this))
            outlineTransform = *outline;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Ellipse<Dim, Spec>::Vector
        Ellipse<Dim, Spec>::getCenter(void) const noexcept
    {
        return (Vector{get<"position">(this->vertices[3])}
            + Vector{get<"position">(this->vertices[1])}) / 2.f;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Vector2f Ellipse<Dim, Spec>::getSemiAxis(
        void) const noexcept
    {
        return {
            (Vector{get<"position">(this->vertices[1])}
                - Vector{get<"position">(this->vertices[0])}).length(),
            (Vector{get<"position">(this->vertices[3])}
                - Vector{get<"position">(this->vertices[0])}).length()
        };
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<Adapter> positions{
            this->vertices | views::position};
        transformator(positions);
        actualizeMatrices();
        this->isModified = true;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::actualizeLocations(void) const noexcept {
        Elliptic<Dim, Spec>::actualizeLocations();
        locations.shift(Vector{get<"position">(
            this->vertices.front())});
        locations.transform(outlineTransform);
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::setShader(
        ShaderProgram const& program) noexcept
    {
        Elliptic<Dim, Spec>::setShader(program);
        shaderManager(program);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::setShader(
        ShaderProgram&& program) noexcept
    {
        Elliptic<Dim, Spec>::setShader(std::move(program));
        shaderManager(*this->shaderProgram);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ellipse<Dim, Spec>::setShader(std::string const& name) {
        Elliptic<Dim, Spec>::setShader(name);
        shaderManager(*this->shaderProgram);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool Ellipse<Dim, Spec>::contains(
        Vector2u const& position) const noexcept
    {
        return clicker(*this, position);
    }

}

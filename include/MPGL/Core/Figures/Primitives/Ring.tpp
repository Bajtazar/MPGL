/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#pragma once

#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Exceptions/DifferentPlanesException.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::OutlineCalculator const
        Ring<Dim, Spec>::outlineCalc = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Clicker const
        Ring<Dim, Spec>::clicker = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::ShaderManager const
        Ring<Dim, Spec>::shaderManager = {};

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::InnerEllipse::Vertices
        Ring<Dim, Spec>::InnerEllipse::ellipseVertices(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle) requires TwoDimensional<Dim>
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

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::InnerEllipse::Vertices
        Ring<Dim, Spec>::InnerEllipse::circleVertices(
            Vector2f const& center,
            float32 radius) requires TwoDimensional<Dim>
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

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::InnerEllipse::InnerEllipse(
        Vector2f const& center,
        Vector2f const& semiAxis,
        float32 angle) requires TwoDimensional<Dim>
            : vertices{ellipseVertices(center, semiAxis, angle)}
    {
        actualizeMatrices();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::InnerEllipse::InnerEllipse(
        Vector2f const& center,
        float radius) requires TwoDimensional<Dim>
            : vertices{circleVertices(center, radius)}
    {
        actualizeMatrices();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::InnerEllipse::InnerEllipse(
        Vector3f const& center,
        Vector3f const& minorAxis,
        Vector3f const& majorAxis) requires ThreeDimensional<Dim>
            : vertices{
                center - majorAxis - minorAxis,
                center - majorAxis + minorAxis,
                center + majorAxis + minorAxis,
                center + majorAxis - minorAxis
            }
    {
        if (dot(minorAxis, majorAxis))
            throw NotPerpendicularException{minorAxis, majorAxis};
        actualizeMatrices();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::InnerEllipse::actualizeMatrices(
        void) noexcept
    {
        if (auto outline = outlineCalc(vertices))
            this->outline = *outline;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Ring<Dim, Spec>::Vector
        Ring<Dim, Spec>::InnerEllipse::getCenter(void) const noexcept
    {
        return (static_cast<Vector>(vertices[3]) + static_cast<Vector>(vertices[1])) / 2.f;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Vector2f
        Ring<Dim, Spec>::InnerEllipse::getSemiAxis(void) const noexcept
    {
        return {
            (static_cast<Vector>(vertices[1]) - static_cast<Vector>(vertices[0])).length(),
            (static_cast<Vector>(vertices[3]) - static_cast<Vector>(vertices[0])).length()
        };
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::InnerEllipse::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<Adapter> positions{vertices};
        transformator(positions);
        actualizeMatrices();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::setLocations(void) {
        auto& program = *this->shaderProgram;
        locations.outerShift
            = ShaderLocation{program, "outerShift"};
        locations.innerShift
            = ShaderLocation{program, "innerShift"};
        locations.outerTransform
            = ShaderLocation{program, "outerTransform"};
        locations.innerTransform
            = ShaderLocation{program, "innerTransform"};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Ring(
        Vector2f const& center,
        Vector2f const& semiAxis,
        InnerEllipse const& innerEllipse,
        Color const& color,
        float32 angle) requires TwoDimensional<Dim>
            : Elliptic<Dim, Spec>{Elliptic<Dim, Spec>::ellipseVertices(
                center, semiAxis, angle, color),
                shaderManager.shader},
                innerEllipse{innerEllipse}
    {
        shaderManager(*this->shaderProgram);
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Ring(
        Vector2f const& center,
        float32 radius,
        InnerEllipse const& innerEllipse,
        Color const& color) requires TwoDimensional<Dim>
            : Elliptic<Dim, Spec>{Elliptic<Dim, Spec>::circleVertices(
                center, radius, color),
                shaderManager.shader},
            innerEllipse{innerEllipse}
    {
        shaderManager(*this->shaderProgram);
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Ring(
        Vector3f const& center,
        Vector3f const& minorAxis,
        Vector3f const& majorAxis,
        InnerEllipse const& innerEllipse,
        Color const& color) requires ThreeDimensional<Dim>
            : Elliptic<Dim, Spec>{{
                VertexTraits::buildVertex(center - majorAxis - minorAxis, color),
                VertexTraits::buildVertex(center - majorAxis + minorAxis, color),
                VertexTraits::buildVertex(center + majorAxis + minorAxis, color),
                VertexTraits::buildVertex(center + majorAxis - minorAxis, color)
            }, shaderManager.shader}, innerEllipse{innerEllipse}
    {
        if (dot(minorAxis, majorAxis))
            throw NotPerpendicularException{minorAxis, majorAxis};
        shaderManager(*this->shaderProgram);
        checkInnerAndOuterPlanes();
        actualizeMatrices();
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Ring(
        Vector2f const& center,
        Vector2f const& outerSemiAxis,
        Vector2f const& innerSemiAxis,
        Color const& color,
        float32 angle) requires TwoDimensional<Dim>
            : Ring<Dim, Spec>{center, outerSemiAxis,
                InnerEllipse{center, innerSemiAxis,
                angle}, color, angle} {}

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Ring<Dim, Spec>::Ring(
        Vector2f const& center,
        float32 outerRadius,
        float32 innerRadius,
        Color const& color) requires TwoDimensional<Dim>
            : Ring{center, outerRadius, InnerEllipse{
                center, innerRadius}, color} {}

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::checkInnerAndOuterPlanes(
        void) const requires ThreeDimensional<Dim>
    {
        Vector4f coeffs = planeCoefficients<float32>(
            get<"position">(this->vertices[0]),
            get<"position">(this->vertices[1]),
            get<"position">(this->vertices[3]));
        for (uint8 i = 0; i < 3; ++i) {
            Vector3f const vertex = innerEllipse.vertices[i];
            if (!isOnPlane(coeffs, vertex))
                throw DifferentPlanesException{coeffs, vertex};
        }
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::actualizeMatrices(void) noexcept {
        if (auto outline = outlineCalc(this->vertices | views::position))
            this->outline = *outline;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Ring<Dim, Spec>::Vector
        Ring<Dim, Spec>::getCenter(void) const noexcept
    {
        return (static_cast<Vector>(get<"position">(this->vertices[3]))
            + static_cast<Vector>(get<"position">(this->vertices[1]))) / 2.f;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Vector2f Ring<Dim, Spec>::getOuterSemiAxis(
        void) const noexcept
    {
        return {
            (static_cast<Vector>(get<"position">(this->vertices[1]))
                - static_cast<Vector>(get<"position">(this->vertices[0]))).length(),
            (static_cast<Vector>(get<"position">(this->vertices[3]))
                - static_cast<Vector>(get<"position">(this->vertices[0]))).length()
        };
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        innerEllipse.transform(transformator);
        any::InputRange<Adapter> positions{
            this->vertices | views::position};
        transformator(positions);
        actualizeMatrices();
        this->isModified = true;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::actualizeLocations(void) const noexcept {
        Elliptic<Dim, Spec>::actualizeLocations();
        locations.outerShift(
            static_cast<Vector>(get<"position">(this->vertices.front())));
        locations.innerShift(
            static_cast<Vector>(innerEllipse.vertices.front()));
        locations.outerTransform(outline);
        locations.innerTransform(innerEllipse.outline);
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::setShader(
        ShaderProgram const& program) noexcept
    {
        Elliptic<Dim, Spec>::setShader(program);
        shaderManager(program);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::setShader(ShaderProgram&& program) noexcept {
        Elliptic<Dim, Spec>::setShader(std::move(program));
        shaderManager(*this->shaderProgram);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Ring<Dim, Spec>::setShader(std::string const& name) {
        Elliptic<Dim, Spec>::setShader(name);
        shaderManager(*this->shaderProgram);
        setLocations();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool Ring<Dim, Spec>::contains(
        Vector2u const& position) const noexcept
    {
        return clicker(*this, position);
    }

}

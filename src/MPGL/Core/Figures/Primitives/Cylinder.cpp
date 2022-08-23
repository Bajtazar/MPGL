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
#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Core/Figures/Primitives/Cylinder.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

#include <numbers>
#include <math.h>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::generateCircle(
        Vertices& vertices,
        Color const& color,
        Vector3f const& middle,
        Vector3f const& radiusVector,
        Vector3f const& normal,
        size_t segments)
    {
        for (size_t i = 0; i < segments; ++i) {
            float32 angle = float32(i) * 2. * std::numbers::pi / segments;
            // use quaternions
            vertices.push_back(VertexTraits::buildVertex(middle
                + radiusVector * cos(angle)
                + cross(normal, radiusVector) * sin(angle), color));
        }
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Cylinder<Spec>::Vertices
        Cylinder<Spec>::generateVertices(
            Color const& color,
            Vector3f const& position,
            Vector3f const& radiusVector,
            Vector3f const& heightVector,
            size_t segments)
    {
        Vertices vertices;
        vertices.reserve(2 * segments + 2);
        vertices.push_back(VertexTraits::buildVertex(position, color));
        Vector3f const normal = normalize(heightVector);
        generateCircle(vertices, color, position, radiusVector, normal,
            segments);
        vertices.push_back(VertexTraits::buildVertex(
            position + heightVector, color));
        generateCircle(vertices, color, position + heightVector,
            radiusVector, normal, segments);
        return vertices;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::addTriangle(
        Indicies& indicies,
        size_t first,
        size_t second,
        size_t third)
    {
        indicies.push_back(first);
        indicies.push_back(second);
        indicies.push_back(third);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::generateCircleIndicies(
        Indicies& indicies,
        size_t startIndex,
        size_t segments)
    {
        for (auto i = startIndex + 1; i < startIndex + segments; ++i)
            addTriangle(indicies, startIndex, i, i + 1);
        addTriangle(indicies, startIndex, startIndex + 1,
            startIndex + segments);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::generateFacesIndicies(
        Indicies& indicies,
        size_t segments)
    {
        for (size_t i = 0; i < segments - 1; ++i) {
            addTriangle(indicies, i + 1, i + 2, i + segments + 2);
            addTriangle(indicies, i + 2, i + segments + 2,
                i + segments + 3);
        }
        addTriangle(indicies, 1, segments, segments + 2);
        addTriangle(indicies, segments, segments + 2, 2 * segments + 1);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Cylinder<Spec>::Indicies
        Cylinder<Spec>::generateIndicies(size_t segments)
    {
        Indicies indicies;
        indicies.reserve(12 * segments);
        generateCircleIndicies(indicies, 0, segments);
        generateCircleIndicies(indicies, segments + 1, segments);
        generateFacesIndicies(indicies, segments);
        return indicies;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Cylinder<Spec>::Cylinder(
        Color const& color,
        size_t baseSegments) : Angular<dim::Dim3, Spec>{
            baseSegments * 2 + 2, color}, indicies{
                generateIndicies(baseSegments)}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Cylinder<Spec>::Cylinder(Cylinder const& cylinder)
        : Angular<dim::Dim3, Spec>{cylinder}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Cylinder<Spec>::Cylinder(
        Vector3f const& position,
        Vector3f const& radiusVector,
        Vector3f const& heightVector,
        Color const& color,
        size_t baseSegments) : Angular<dim::Dim3, Spec>{
            generateVertices(color, position, radiusVector,
            heightVector, baseSegments)},
                indicies{generateIndicies(baseSegments)}
    {
        if (dot(radiusVector, heightVector))
            throw NotPerpendicularException{radiusVector, heightVector};
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Cylinder<Spec>::Cylinder(
        Vector3f const& position,
        float32 radius,
        float32 height,
        Color const& color,
        size_t baseSegments) : Angular<dim::Dim3, Spec>{
            generateVertices(color, position, {1._x * radius},
            {1._y * height}, baseSegments)},
                indicies{generateIndicies(baseSegments)}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indicies);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Cylinder<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, indicies.size(),
            DataType::UInt32);
    }

}

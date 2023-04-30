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

#include <MPGL/Core/Vertex/Tessellation/CatmullClarkTessellator.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Torus<Spec>::Indices const Torus<Spec>::BaseIndices {
        {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7},
        {4, 5, 9, 8}, {5, 6, 10, 9}, {6, 7, 11, 10}, {7, 4, 8, 11},
        {8, 9, 13, 12}, {9, 10, 14, 13}, {10, 11, 15, 14}, {11, 8, 12, 15},
        {12, 13, 1, 0}, {13, 14, 2, 1}, {14, 15, 3, 2}, {15, 12, 0, 3}
    };

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Torus<Spec>::Vertices
        Torus<Spec>::generateBaseVertices(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color)
    {
        Vertices vertices;
        vertices.reserve(16);
        generateRing(vertices, position, 1._x, radius, ringRadius, color);
        generateRing(vertices, position, 1._y, radius, ringRadius, color);
        generateRing(vertices, position, -1._x, radius, ringRadius, color);
        generateRing(vertices, position, -1._y, radius, ringRadius, color);
        return vertices;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Torus<Spec>::generateRing(
        Vertices& vertices,
        Vector3f const& position,
        Vector3f const& versor,
        float32 radius,
        float32 ringRadius,
        Color const& color)
    {
        vertices.push_back(VertexTraits::buildVertex(
            position + versor * (radius + ringRadius), color));
        vertices.push_back(VertexTraits::buildVertex(
            position + versor * radius - 1._z * ringRadius, color));
        vertices.push_back(VertexTraits::buildVertex(
            position + versor * (radius - ringRadius), color));
        vertices.push_back(VertexTraits::buildVertex(
            position + versor * radius + 1._z * ringRadius, color));
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Torus<Spec>::TessellationResult
        Torus<Spec>::tessellateBase(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color,
            uint8 tessellationSteps)
    {
        auto builder = [&color](Vector3f const& position) -> Vertex {
            return VertexTraits::buildVertex(position, color);
        };
        return catmullClarkTessellator(generateBaseVertices(
            position, radius, ringRadius, color),
            Indices{BaseIndices}, tessellationSteps, builder);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Torus<Spec>::Torus(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color,
            uint8 tessellationSteps)
            : Torus{tessellateBase(position, radius, ringRadius,
                color, tessellationSteps)}
    {
        reloadElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Torus<Spec>::Torus(TessellationResult&& result)
        : Angular<dim::Dim3, Spec>{std::move(result.first)},
        indices{std::move(result.second)}
    {
        reloadElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Torus<Spec>::Torus(Torus const& torus)
        : Angular<dim::Dim3, Spec>{torus}, indices{torus.indices}
    {
        reloadElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Torus<Spec>& Torus<Spec>::operator=(Torus const& torus) {
        Angular<dim::Dim3, Spec>::operator=(torus);
        indices = torus.indices;
        reloadElementBuffer();
        return *this;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Torus<Spec>::reloadElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Torus<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles,
            6 * indices.size(),
            DataType::UInt32);
    }

}

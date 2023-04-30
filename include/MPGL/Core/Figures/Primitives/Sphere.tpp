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

#include <MPGL/Core/Vertex/Tessellation/SubdivisionTessellator.hpp>
#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Vertex/VertexCast.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>::Indices const Sphere<Spec>::IcosahedronIndices {{
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
        {1, 7, 2}, {2, 8, 3}, {3, 9, 4}, {4, 10, 5}, {5, 6, 1},
        {6, 1, 7}, {7, 2, 8}, {8, 3, 9}, {9, 4, 10}, {10, 5, 6},
        {11, 6, 7}, {11, 7, 8}, {11, 8, 9}, {11, 9, 10}, {11, 10, 6}
    }};

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    float32 const Sphere<Spec>::ATan = std::atan(0.5f);

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::generateIcosahedronFaces(
        Vertices& vertices,
        Vector3f const& position,
        float32 radius,
        Color const& color)
    {
        for (uint8 i = 0; i < 5; ++i)
            vertices.push_back(VertexTraits::buildVertex(
                position + sphericalToCartesian(Vector3f{radius, ATan,
                static_cast<float32>(std::numbers::pi * (72. * i + 36.) / 180.)}),
            color));
        for (uint8 i = 0; i < 5; ++i)
            vertices.push_back(VertexTraits::buildVertex(
                position + Vector3f{1, 1, -1} *
                    sphericalToCartesian(Vector3f{radius, ATan,
                        static_cast<float32>(std::numbers::pi * (72. * i) / 180.)}),
            color));
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Sphere<Spec>::Vertices
        Sphere<Spec>::generateIcosahedron(
            Vector3f const& position,
            float32 radius,
            Color const& color)
    {
        Vertices vertices;
        vertices.reserve(12);
        vertices.push_back(
            VertexTraits::buildVertex(position + 1._z * radius, color));
        generateIcosahedronFaces(vertices, position, radius, color);
        vertices.push_back(
            VertexTraits::buildVertex(position - 1._z * radius, color));
        return vertices;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>::Sphere(
        Vector3f const& position,
        float32 radius,
        Color const& color,
        uint8 tessellationSteps)
            : Sphere{tessellateIcosahedron(position, radius, color,
                tessellationSteps)} {}

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>::Sphere(TessellationResult&& result)
        : Angular<dim::Dim3, Spec>{std::move(result.first)},
        indices{std::move(result.second)}
    {
        reloadElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] Sphere<Spec>::TessellationResult
        Sphere<Spec>::tessellateIcosahedron(
            Vector3f const& position,
            float32 radius,
            Color const& color,
            uint8 steps)
    {
        auto predicate = [&position, &radius, &color](
            Vertex const& left, Vertex const& right)
        {
            auto inner = static_cast<Vector3f>(left | cast::position)
                + static_cast<Vector3f>(right | cast::position) - 2.f * position;
            inner *= radius / inner.length();
            return VertexTraits::buildVertex(inner + position, color);
        };
        return subdivisionTessellator(
            generateIcosahedron(position, radius, color),
            IcosahedronIndices, steps, predicate);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>::Sphere(Sphere const& sphere)
        : Angular<dim::Dim3, Spec>{sphere}, indices{sphere.indices}
    {
        reloadElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>& Sphere<Spec>::operator=(Sphere const& sphere) {
        Angular<dim::Dim3, Spec>::operator=(sphere);
        indices = sphere.indices;
        reloadElementBuffer();
        return *this;
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::reloadElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 3 * indices.size(),
            DataType::UInt32);
    }

}

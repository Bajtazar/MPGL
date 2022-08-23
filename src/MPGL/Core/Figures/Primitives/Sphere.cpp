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
#include <MPGL/Core/Vertex/Tessellation/SubdivisionTessellator.hpp>
#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Primitives/Sphere.hpp>
#include <MPGL/Core/Vertex/VertexCast.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    float64 const Sphere<Spec>::ATan = std::atan(0.5);

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
                std::numbers::pi * (72. * i + 36.) / 180.}),
            color));
        for (uint8 i = 0; i < 5; ++i)
            vertices.push_back(VertexTraits::buildVertex(
                position + Vector3f{1, 1, -1} *
                    sphericalToCartesian(Vector3f{radius, ATan,
                        std::numbers::pi * (72. * i) / 180.}),
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
            : Angular<dim::Dim3, Spec>{generateIcosahedron(
                position, radius, color)},
            indicies{IcosahedronIndicies.begin(),
                IcosahedronIndicies.end()}
    {
        tessellateIcosahedron(position, radius, color, tessellationSteps);
        reshapeBuffer();
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::tessellateIcosahedron(
        Vector3f const& position,
        float32 radius,
        Color const& color,
        uint8 steps)
    {
        auto predicate = [&position, &radius, &color](
            Vertex const& left, Vertex const& right)
        {
            auto inner = Vector3f{left | cast::position}
                + Vector3f{right | cast::position} - 2.f * position;
            inner *= radius / inner.length();
            return VertexTraits::buildVertex(inner + position, color);
        };
        auto&& [newVertices, newIndicies] = subdivisionTessellator(
            this->vertices, indicies, steps, predicate);
        this->vertices = std::move(newVertices);
        this->indicies = std::move(newIndicies);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::reshapeBuffer(void) const noexcept {
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        this->vertexBuffer.setBufferData(this->vertices);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Sphere<Spec>::Sphere(Sphere const& sphere)
        : Angular<dim::Dim3, Spec>{sphere}, indicies{sphere.indicies}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indicies);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Sphere<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 3 * indicies.size(),
            DataType::UInt32);
    }

}

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
#include <MPGL/Core/Figures/Primitives/Tetrahedron.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

namespace mpgl {

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Tetrahedron<Spec>::Tetrahedron(Color const& color)
        : Angular<dim::Dim3, Spec>{4, color}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Tetrahedron<Spec>::Tetrahedron(
        Vector3f const& firstVertex,
        Vector3f const& secondVertex,
        Vector3f const& thirdVertex,
        Vector3f const& fourthVertex,
        Color const& color)
            : Angular<dim::Dim3, Spec>{{
                VertexTraits::buildVertex(firstVertex, color),
                VertexTraits::buildVertex(secondVertex, color),
                VertexTraits::buildVertex(thirdVertex, color),
                VertexTraits::buildVertex(fourthVertex, color)}}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    Tetrahedron<Spec>::Tetrahedron(Tetrahedron const& tetrahedron)
        : Angular<dim::Dim3, Spec>{tetrahedron}
    {
        initElementBuffer();
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Tetrahedron<Spec>::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
    }

    template <AngularTraitSpecifier<dim::Dim3> Spec>
    void Tetrahedron<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 12, DataType::UInt32);
    }

}

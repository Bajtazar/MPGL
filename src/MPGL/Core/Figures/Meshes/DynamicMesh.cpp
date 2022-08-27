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
#include <MPGL/Core/Figures/Meshes/DynamicMesh.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <MeshTraitSpecifier Spec>
    DynamicMesh<Spec>::DynamicMesh(
        Vertices&& vertices,
        Indices&& indices,
        std::string const& program)
            : Figure3D{program}, vertices{std::move(vertices)},
            indices{std::move(indices)}
    {
        initializeBuffers();
        buildVertexViews();
    }

    template <MeshTraitSpecifier Spec>
    DynamicMesh<Spec>::DynamicMesh(DynamicMesh const& dynamicMesh)
        : Figure3D{dynamicMesh}, vertices{dynamicMesh.vertices},
        indices{dynamicMesh.indices},
        emptyVertices{dynamicMesh.emptyVertices}
    {
        initializeBuffers();
        buildVertexViews();
    }

    template <MeshTraitSpecifier Spec>
    DynamicMesh<Spec>& DynamicMesh<Spec>::operator=(
        DynamicMesh const& dynamicMesh)
    {
        Figure3D::operator=(dynamicMesh);
        vertices = dynamicMesh.vertices;
        indices = dynamicMesh.indices;
        emptyVertices = dynamicMesh.emptyVertices;
        reloadElementBuffer();
        verticesView.clear();
        buildVertexViews();
        return *this;
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(
            VertexArray::VertexTag<Vertex>{});
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::reloadElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::buildVertexViews(void) {
        uint32 id = 0;
        for (Vertex const& vertex : vertices)
            verticesView.emplace_back(std::ref(*this), id++);
        id = 0;
        for (IndicesTriangle const& triangle : indices) {
            verticesView[triangle.firstVertex].emplaceTriangle(id);
            verticesView[triangle.secondVertex].emplaceTriangle(id);
            verticesView[triangle.thirdVertex].emplaceTriangle(id++);
        }
        for (uint32 removedID : emptyVertices | std::views::reverse)
            verticesView.erase(verticesView.begin() + removedID);
    }

}

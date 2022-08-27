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

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 3 * indices.size(),
            DataType::UInt32);
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::transform(
        Transformation3D const& transformator) noexcept
    {
        any::InputRange<Adapter3D> positions{
            vertices | views::position};
        transformator(positions);
        this->isModified = true;
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::Vertex&
        DynamicMesh<Spec>::VertexView::getVertex(void) noexcept
    {
        parent.isModified = true;
        return parent.vertices[vertexID];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::Vertex const&
        DynamicMesh<Spec>::VertexView::getVertex(void) const noexcept
    {
        return parent.vertices[vertexID];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle&
        DynamicMesh<Spec>::VertexView::operator[] (
            std::size_t index) noexcept
    {
        return parent.indices[indicesIDs[index]];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle const&
        DynamicMesh<Spec>::VertexView::operator[] (
            std::size_t index) const noexcept
    {
        return parent.indices[indicesIDs[index]];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle&
        DynamicMesh<Spec>::VertexView::front(void) noexcept
    {
        return parent.indices[indicesIDs.front()];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle const&
        DynamicMesh<Spec>::VertexView::front(void) const noexcept
    {
        return parent.indices[indicesIDs.front()];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle&
        DynamicMesh<Spec>::VertexView::back(void) noexcept
    {
        return parent.indices[indicesIDs.back()];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] IndicesTriangle const&
        DynamicMesh<Spec>::VertexView::back(void) const noexcept
    {
        return parent.indices[indicesIDs.back()];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::iterator
        DynamicMesh<Spec>::VertexView::begin(void) noexcept
    {
        return iterator{indicesIDs.begin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::iterator
        DynamicMesh<Spec>::VertexView::end(void) noexcept
    {
        return iterator{indicesIDs.end(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_iterator
        DynamicMesh<Spec>::VertexView::begin(void) const noexcept
    {
        return const_iterator{indicesIDs.begin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_iterator
        DynamicMesh<Spec>::VertexView::end(void) const noexcept
    {
        return const_iterator{indicesIDs.end(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_iterator
        DynamicMesh<Spec>::VertexView::cbegin(void) const noexcept
    {
        return const_iterator{indicesIDs.cbegin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_iterator
        DynamicMesh<Spec>::VertexView::cend(void) const noexcept
    {
        return const_iterator{indicesIDs.cend(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::reverse_iterator
        DynamicMesh<Spec>::VertexView::rbegin(void) noexcept
    {
        return reverse_iterator{indicesIDs.rbegin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::reverse_iterator
        DynamicMesh<Spec>::VertexView::rend(void) noexcept
    {
        return reverse_iterator{indicesIDs.rend(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_reverse_iterator
        DynamicMesh<Spec>::VertexView::rbegin(void) const noexcept
    {
        return const_reverse_iterator{indicesIDs.rbegin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_reverse_iterator
        DynamicMesh<Spec>::VertexView::rend(void) const noexcept
    {
        return const_reverse_iterator{indicesIDs.rend(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_reverse_iterator
        DynamicMesh<Spec>::VertexView::crbegin(void) const noexcept
    {
        return const_reverse_iterator{indicesIDs.crbegin(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView::const_reverse_iterator
        DynamicMesh<Spec>::VertexView::crend(void) const noexcept
    {
        return const_reverse_iterator{indicesIDs.crend(), &parent};
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::VertexView::reserve(std::size_t size) {
        indicesIDs.reserve(size);
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::VertexView::shrinkToFit(void) {
        indicesIDs.shrink_to_fit();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] bool DynamicMesh<Spec>::VertexView::empty(
        void) const noexcept
    {
        return indicesIDs.empty();
    }

    template <MeshTraitSpecifier Spec>
    DynamicMesh<Spec>::VertexView::VertexView(
        DynamicMesh& parent,
        uint32 vertexID) : parent{parent}, vertexID{vertexID} {}

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::VertexView::emplaceTriangle(
        uint32 triangleID)
    {
        indicesIDs.push_back(triangleID);
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView&
        DynamicMesh<Spec>::operator[] (std::size_t index) noexcept
    {
        return verticesView[index];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView const&
        DynamicMesh<Spec>::operator[] (std::size_t index) const noexcept
    {
        return verticesView[index];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView&
        DynamicMesh<Spec>::front(void) noexcept
    {
        return verticesView.front();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView const&
        DynamicMesh<Spec>::front(void) const noexcept
    {
        return verticesView.front();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView&
        DynamicMesh<Spec>::back(void) noexcept
    {
        return verticesView.back();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::VertexView const&
        DynamicMesh<Spec>::back(void) const noexcept
    {
        return verticesView.back();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::iterator
        DynamicMesh<Spec>::begin(void) noexcept
    {
        return iterator{verticesView.begin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::iterator
        DynamicMesh<Spec>::end(void) noexcept
    {
        return iterator{verticesView.end()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_iterator
        DynamicMesh<Spec>::begin(void) const noexcept
    {
        return const_iterator{verticesView.begin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_iterator
        DynamicMesh<Spec>::end(void) const noexcept
    {
        return const_iterator{verticesView.end()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_iterator
        DynamicMesh<Spec>::cbegin(void) const noexcept
    {
        return const_iterator{verticesView.cbegin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_iterator
        DynamicMesh<Spec>::cend(void) const noexcept
    {
        return const_iterator{verticesView.cend()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::reverse_iterator
        DynamicMesh<Spec>::rbegin(void) noexcept
    {
        return reverse_iterator{verticesView.rbegin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::reverse_iterator
        DynamicMesh<Spec>::rend(void) noexcept
    {
        return reverse_iterator{verticesView.rend()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_reverse_iterator
        DynamicMesh<Spec>::rbegin(void) const noexcept
    {
        return const_reverse_iterator{verticesView.rbegin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_reverse_iterator
        DynamicMesh<Spec>::rend(void) const noexcept
    {
        return const_reverse_iterator{verticesView.rend()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_reverse_iterator
        DynamicMesh<Spec>::crbegin(void) const noexcept
    {
        return const_reverse_iterator{verticesView.crbegin()};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] DynamicMesh<Spec>::const_reverse_iterator
        DynamicMesh<Spec>::crend(void) const noexcept
    {
        return const_reverse_iterator{verticesView.crend()};
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::reserve(std::size_t size) {
        vertices.reserve(size);
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::shrinkToFit(void) {
        uint32 counter = 0;
        for (uint32 element : emptyVertices | std::views::reverse) {
            vertices[element] = std::move(vertices.back());
            vertices.pop_back();
            adjustVertexView(getAddress(vertices.size()) + counter++,
                element);
        }
        emptyVertices.clear();
        vertices.shrink_to_fit();
        isExtended = true;
    }

    template <MeshTraitSpecifier Spec>
    void DynamicMesh<Spec>::adjustVertexView(
        uint32 oldAddress,
        uint32 newAddress) noexcept
    {
        VertexView view = verticesView[oldAddress];
        for (IndicesTriangle& triangle : view) {
            if (triangle.firstVertex == view.vertexID)
                triangle.firstVertex = newAddress;
            else if (triangle.secondVertex == view.vertexID)
                triangle.secondVertex = newAddress;
            else
                triangle.thirdVertex = newAddress;
        }
        view.vertexID = newAddress;
        verticesView.erase(verticesView.begin() + oldAddress);
        verticesView.insert(verticesView.begin() + newAddress, view);
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] uint32 DynamicMesh<Spec>::getAddress(
        uint32 vertexID) const noexcept
    {
        auto iter = std::ranges::lower_bound(emptyVertices, vertexID);
        return vertexID - std::distance(emptyVertices.begin(), iter);
    }

}

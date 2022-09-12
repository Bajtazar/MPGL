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
#include <MPGL/Core/Figures/Meshes/StaticMesh.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <MeshTraitSpecifier Spec>
    StaticMesh<Spec>::StaticMesh(
        Vertices&& vertices,
        Indices&& indices,
        std::string const& program)
            : Figure3D{program}, vertices{std::move(vertices)},
            indices{std::move(indices)}
    {
        initializeBuffers();
    }

    template <MeshTraitSpecifier Spec>
    StaticMesh<Spec>::StaticMesh(StaticMesh const& staticMesh)
        : Figure3D{staticMesh}, vertices{staticMesh.vertices},
        indices{staticMesh.indices}
    {
        initializeBuffers();
    }

    template <MeshTraitSpecifier Spec>
    StaticMesh<Spec>& StaticMesh<Spec>::operator=(
        StaticMesh const& staticMesh)
    {
        Figure3D::operator=(staticMesh);
        vertices = staticMesh.vertices;
        indices = staticMesh.indices;
        reloadElementBuffer();
        return *this;
    }

    template <MeshTraitSpecifier Spec>
    void StaticMesh<Spec>::actualizeBufferBeforeDraw(
        void) const noexcept
    {
        if (this->isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
                this->vertexBuffer.changeBufferData(vertices);
            }
            this->isModified = false;
        }
    }

    template <MeshTraitSpecifier Spec>
    void StaticMesh<Spec>::transform(
        Transformation3D const& transformator) noexcept
    {
        any::InputRange<Adapter3D> positions{
            vertices | views::position};
        transformator(positions);
        this->isModified = true;
    }

    template <MeshTraitSpecifier Spec>
    void StaticMesh<Spec>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(
            VertexArray::VertexTag<Vertex>{});
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <MeshTraitSpecifier Spec>
    void StaticMesh<Spec>::reloadElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex&
        StaticMesh<Spec>::operator[] (std::size_t index) noexcept
    {
        this->isModified = true;
        return vertices[index];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex const&
        StaticMesh<Spec>::operator[] (
            std::size_t index) const noexcept
    {
        return vertices[index];
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex& StaticMesh<Spec>::front(
        void) noexcept
    {
        this->isModified = true;
        return vertices.front();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex const&
        StaticMesh<Spec>::front(void) const noexcept
    {
        return vertices.front();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex& StaticMesh<Spec>::back(
        void) noexcept
    {
        this->isModified = true;
        return vertices.back();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::Vertex const&
        StaticMesh<Spec>::back(void) const noexcept
    {
        return vertices.back();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::iterator
        StaticMesh<Spec>::begin(void) noexcept
    {
        return iterator{vertices.begin(), this->isModified};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::iterator StaticMesh<Spec>::end(
        void) noexcept
    {
        return iterator{vertices.end(), this->isModified};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_iterator
        StaticMesh<Spec>::begin(void) const noexcept
    {
        return vertices.begin();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_iterator
        StaticMesh<Spec>::end(void) const noexcept
    {
        return vertices.end();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_iterator
        StaticMesh<Spec>::cbegin(void) const noexcept
    {
        return vertices.begin();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_iterator
        StaticMesh<Spec>::cend(void) const noexcept
    {
        return vertices.end();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::reverse_iterator
        StaticMesh<Spec>::rbegin(void) noexcept
    {
        return reverse_iterator{vertices.rbegin(), this->isModified};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::reverse_iterator
        StaticMesh<Spec>::rend(void) noexcept
    {
        return reverse_iterator{vertices.rend(), this->isModified};
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_reverse_iterator
        StaticMesh<Spec>::rbegin(void) const noexcept
    {
        return vertices.rbegin();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_reverse_iterator
        StaticMesh<Spec>::rend(void) const noexcept
    {
        return vertices.rend();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_reverse_iterator
        StaticMesh<Spec>::crbegin(void) const noexcept
    {
        return vertices.crbegin();
    }

    template <MeshTraitSpecifier Spec>
    [[nodiscard]] StaticMesh<Spec>::const_reverse_iterator
        StaticMesh<Spec>::crend(void) const noexcept
    {
        return vertices.crend();
    }

    template <MeshTraitSpecifier Spec>
    void StaticMesh<Spec>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        this->actualizeLocations();
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 3 * indices.size(),
            DataType::UInt32);
    }

}

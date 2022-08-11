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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Angular.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    namespace details {

        [[nodiscard]] std::string AngluarVertices<dim::Dim2>::shader(
            void) noexcept
        {
            return "MPGL/2D/Default";
        }

        [[nodiscard]] std::string AngluarVertices<dim::Dim3>::shader(
            void) noexcept
        {
            return "MPGL/3D/Default";
        }

    }

    template <Dimension Dim>
    void Angular<Dim>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(vertices.front());
    }

    template <Dimension Dim>
    Angular<Dim>::Angular(size_t size, Color const& color)
        : Figure<Dim>{VertexTraits::shader()}, vertices{size,
            Vertex{typename VertexTraits::Vector{}, color}}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    Angular<Dim>::Angular(Vertices vertices)
        : Figure<Dim>{VertexTraits::shader()},
        vertices{std::move(vertices)}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    Angular<Dim>::Angular(Angular const& shape)
        : Figure<Dim>{shape}, vertices{shape.vertices}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex&
        Angular<Dim>::operator[] (std::size_t index) noexcept
    {
        this->isModified = true;
        return vertices[index];
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex const&
        Angular<Dim>::operator[] (std::size_t index) const noexcept
    {
        return vertices[index];
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex& Angular<Dim>::front(
        void) noexcept
    {
        this->isModified = true;
        return vertices.front();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex const& Angular<Dim>::front(
        void) const noexcept
    {
        return vertices.front();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex& Angular<Dim>::back(
        void) noexcept
    {   this->isModified = true;
        return vertices.back();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::Vertex const& Angular<Dim>::back(
        void) const noexcept
    {
        return vertices.back();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::iterator Angular<Dim>::begin(
        void) noexcept
    {
        this->isModified = true;
        return vertices.begin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::iterator Angular<Dim>::end(
        void) noexcept
    {
        this->isModified = true;
        return vertices.end();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_iterator Angular<Dim>::begin(
        void) const noexcept
    {
        return vertices.begin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_iterator Angular<Dim>::end(
        void) const noexcept
    {
        return vertices.end();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_iterator Angular<Dim>::cbegin(
        void) const noexcept
    {
        return vertices.begin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_iterator Angular<Dim>::cend(
        void) const noexcept
    {
        return vertices.end();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::reverse_iterator
        Angular<Dim>::rbegin(void) noexcept
    {
        this->isModified = true;
        return vertices.rbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::reverse_iterator
        Angular<Dim>::rend(void) noexcept
    {
        this->isModified = true;
        return vertices.rend();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_reverse_iterator
        Angular<Dim>::rbegin(void) const noexcept
    {
        return vertices.rbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_reverse_iterator
        Angular<Dim>::rend(void) const noexcept
    {
        return vertices.rend();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_reverse_iterator
        Angular<Dim>::crbegin(void) const noexcept
    {
        return vertices.crbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] Angular<Dim>::const_reverse_iterator
        Angular<Dim>::crend(void) const noexcept
    {
        return vertices.crend();
    }

    template <Dimension Dim>
    void Angular<Dim>::actualizeBufferBeforeDraw(void) const noexcept {
        if (this->isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
                this->vertexBuffer.changeBufferData(vertices);
            }
            this->isModified = false;
        }
    }

    template <Dimension Dim>
    Angular<Dim>& Angular<Dim>::operator=(Angular<Dim> const& shape) {
        Figure<Dim>::operator=(shape);
        vertices.clear();
        vertices.reserve(shape.vertices.size());
        std::ranges::copy(shape.vertices, std::back_inserter(vertices));
        return *this;
    }

    template <Dimension Dim>
    void Angular<Dim>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<typename VertexTraits::Adapter> positions{
            vertices | views::position};
        transformator(positions);
        this->isModified = true;
    }

}

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

#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Angular<Dim, Spec>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(
            VertexArray::VertexTag<Vertex>{});
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Angular<Dim, Spec>::Angular(size_t size, Color const& color)
        : Figure<Dim>{VertexTraits::shader()}, vertices{size,
            VertexTraits::defaultVertex(color)}
    {
        initializeBuffers();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Angular<Dim, Spec>::Angular(Vertices vertices)
        : Figure<Dim>{VertexTraits::shader()},
        vertices{std::move(vertices)}
    {
        initializeBuffers();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Angular<Dim, Spec>::Angular(Angular const& shape)
        : Figure<Dim>{shape}, vertices{shape.vertices}
    {
        initializeBuffers();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex&
        Angular<Dim, Spec>::operator[] (std::size_t index) noexcept
    {
        this->isModified = true;
        return vertices[index];
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex const&
        Angular<Dim, Spec>::operator[] (
            std::size_t index) const noexcept
    {
        return vertices[index];
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex& Angular<Dim, Spec>::front(
        void) noexcept
    {
        this->isModified = true;
        return vertices.front();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex const&
        Angular<Dim, Spec>::front(void) const noexcept
    {
        return vertices.front();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex& Angular<Dim, Spec>::back(
        void) noexcept
    {
        this->isModified = true;
        return vertices.back();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::Vertex const&
        Angular<Dim, Spec>::back(void) const noexcept
    {
        return vertices.back();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::iterator
        Angular<Dim, Spec>::begin(void) noexcept
    {
        return iterator{vertices.begin(), this->isModified};
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::iterator Angular<Dim, Spec>::end(
        void) noexcept
    {
        return iterator{vertices.end(), this->isModified};
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_iterator
        Angular<Dim, Spec>::begin(void) const noexcept
    {
        return vertices.begin();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_iterator
        Angular<Dim, Spec>::end(void) const noexcept
    {
        return vertices.end();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_iterator
        Angular<Dim, Spec>::cbegin(void) const noexcept
    {
        return vertices.begin();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_iterator
        Angular<Dim, Spec>::cend(void) const noexcept
    {
        return vertices.end();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::reverse_iterator
        Angular<Dim, Spec>::rbegin(void) noexcept
    {
        return reverse_iterator{vertices.rbegin(), this->isModified};
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::reverse_iterator
        Angular<Dim, Spec>::rend(void) noexcept
    {
        return reverse_iterator{vertices.rend(), this->isModified};
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_reverse_iterator
        Angular<Dim, Spec>::rbegin(void) const noexcept
    {
        return vertices.rbegin();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_reverse_iterator
        Angular<Dim, Spec>::rend(void) const noexcept
    {
        return vertices.rend();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_reverse_iterator
        Angular<Dim, Spec>::crbegin(void) const noexcept
    {
        return vertices.crbegin();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Angular<Dim, Spec>::const_reverse_iterator
        Angular<Dim, Spec>::crend(void) const noexcept
    {
        return vertices.crend();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Angular<Dim, Spec>::actualizeBufferBeforeDraw(
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

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Angular<Dim, Spec>& Angular<Dim, Spec>::operator=(
        Angular const& shape)
    {
        Figure<Dim>::operator=(shape);
        vertices = shape.vertices;
        return *this;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Angular<Dim, Spec>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<typename VertexTraits::Adapter> positions{
            vertices | views::position};
        transformator(positions);
        this->isModified = true;
    }

}

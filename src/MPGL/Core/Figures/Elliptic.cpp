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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Elliptic.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Elliptic<Dim, Spec>::Elliptic(
        Vertices vertices,
        std::string const& programName)
            : Figure<Dim>{programName},
            vertices{std::move(vertices)}
    {
        initializeBuffers();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Elliptic<Dim, Spec>::Elliptic(Elliptic const& shape)
        : Figure<Dim>{shape}, vertices{shape.vertices}
    {
        initializeBuffers();
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Elliptic<Dim, Spec>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        elementBuffer.bind();
        elementBuffer.setBufferData(indices);
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(
            VertexArray::VertexTag<Vertex>{});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Elliptic<Dim, Spec>& Elliptic<Dim, Spec>::operator=(
        Elliptic const& shape)
    {
        Figure<Dim>::operator=(shape);
        vertices = shape.vertices;
        return *this;
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Elliptic<Dim, Spec>::actualizeBufferBeforeDraw(
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

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    void Elliptic<Dim, Spec>::actualizeLocations(void) const noexcept {
        if constexpr (ThreeDimensional<Dim>) {
            Figure3D::actualizeLocations();
        }
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::vertex_view
        Elliptic<Dim, Spec>::operator[] (
            std::size_t index
            ) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(iterator{acr_iterator{this->vertices.begin() + index,
            this->isModified}});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_vertex_view
        Elliptic<Dim, Spec>::operator[] (
            std::size_t index
            ) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(const_iterator{this->vertices.cbegin() + index});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::vertex_view
        Elliptic<Dim, Spec>::front(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(iterator{acr_iterator{
            this->vertices.begin(), this->isModified}});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_vertex_view
        Elliptic<Dim, Spec>::front(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(const_iterator{this->vertices.begin()});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::vertex_view
        Elliptic<Dim, Spec>::back(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(iterator{acr_iterator{
            this->vertices.end() - 1, this->isModified}});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_vertex_view
        Elliptic<Dim, Spec>::back(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return *(const_iterator{this->vertices.end() - 1});
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::iterator
        Elliptic<Dim, Spec>::begin(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return iterator{acr_iterator{
            this->vertices.begin(), this->isModified}};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::iterator
        Elliptic<Dim, Spec>::end(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return iterator{acr_iterator{
            this->vertices.end(), this->isModified}};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_iterator
        Elliptic<Dim, Spec>::begin(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_iterator{this->vertices.begin()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_iterator
        Elliptic<Dim, Spec>::end(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_iterator{this->vertices.end()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_iterator
        Elliptic<Dim, Spec>::cbegin(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_iterator{this->vertices.cbegin()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_iterator
        Elliptic<Dim, Spec>::cend(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_iterator{this->vertices.cend()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::reverse_iterator
        Elliptic<Dim, Spec>::rbegin(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return reverse_iterator{racr_iterator{this->vertices.rbegin(),
            this->isModified}};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::reverse_iterator
        Elliptic<Dim, Spec>::rend(
            void) noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return reverse_iterator{racr_iterator{
            this->vertices.rend(), this->isModified}};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_reverse_iterator
        Elliptic<Dim, Spec>::rbegin(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_reverse_iterator{this->vertices.rbegin()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_reverse_iterator
        Elliptic<Dim, Spec>::rend(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_reverse_iterator{this->vertices.rend()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_reverse_iterator
        Elliptic<Dim, Spec>::crbegin(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_reverse_iterator{this->vertices.rbegin()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    [[nodiscard]] Elliptic<Dim, Spec>::const_reverse_iterator
        Elliptic<Dim, Spec>::crend(
            void) const noexcept requires Elliptic<Dim, Spec>::Iterable
    {
        return const_reverse_iterator{this->vertices.rend()};
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Elliptic<Dim, Spec>::Vertices
        Elliptic<Dim, Spec>::ellipseVertices(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle,
            Color const& color) requires TwoDimensional<Dim>
    {
        Matrix2f rotation = rotationMatrix<float32>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            VertexTraits::buildVertex(center - rot2, color),
            VertexTraits::buildVertex(center + rot1, color),
            VertexTraits::buildVertex(center + rot2, color),
            VertexTraits::buildVertex(center - rot1, color)
        };
    }

    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    Elliptic<Dim, Spec>::Vertices
        Elliptic<Dim, Spec>::circleVertices(
            Vector2f const& center,
            float32 radius,
            Color const& color) requires TwoDimensional<Dim>
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            VertexTraits::buildVertex(center - semiMajor + semiMinor, color),
            VertexTraits::buildVertex(center + semiMajor + semiMinor, color),
            VertexTraits::buildVertex(center + semiMajor - semiMinor, color),
            VertexTraits::buildVertex(center - semiMajor - semiMinor, color)
        };
    }

}

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
#include <MPGL/Core/Figures/ResizableAngular.hpp>
#include <MPGL/Core/Figures/Views.hpp>
#include <MPGL/Utility/Ranges.hpp>

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    ResizableAngular<Dim, Spec>::ResizableAngular(
        size_t size,
        Color const& color)
            : Angular<Dim, Spec>{size, color} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    ResizableAngular<Dim, Spec>::ResizableAngular(Vertices vertices)
        : Angular<Dim, Spec>{vertices} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    ResizableAngular<Dim, Spec>&
        ResizableAngular<Dim, Spec>::operator= (
            ResizableAngular const& shape)
    {
        Angular<Dim, Spec>::operator=(shape);
        isExtended = true;
        return *this;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::shrinkToFit(void) {
        this->vertices.shrink_to_fit();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] std::size_t
        ResizableAngular<Dim, Spec>::capacity(void) const noexcept
    {
        return this->vertices.capacity();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool ResizableAngular<Dim, Spec>::empty(
        void) const noexcept
    {
        return this->vertices.empty();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::erase(
        iterator const& position)
    {
        this->vertices.erase(position.get());
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::erase(
        const_iterator const& position)
    {
        this->vertices.erase(position);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::erase(
        iterator const& first,
        iterator const& last)
    {
        this->vertices.erase(first.get(), last.get());
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::erase(
        const_iterator const& first,
        const_iterator const& last)
    {
        this->vertices.erase(first, last);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] ResizableAngular<Dim, Spec>::Optional
        ResizableAngular<Dim, Spec>::getCenter(void) const noexcept
    {
        if (!this->vertices.size())
            return {};
        return { accumulate(this->vertices | views::position, Vector{},
            [](auto const& x){ return std::forward<Vector>(x); })
                / static_cast<float32>(this->vertices.size()) };
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::resize(size_type size) {
        this->vertices.resize(size, VertexTraits::buildVertex({}, {}));
        isExtended = true;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::reserve(size_type size) {
        this->vertices.reserve(size);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::push(Vertex vertex) {
        this->vertices.push_back(std::move(vertex));
        isExtended = true;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::emplace(
        Vector const& position,
        Color const& color)
    {
        this->vertices.emplace_back(VertexTraits::buildVertex(
            position, color));
        isExtended = true;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::pop(void) noexcept {
        this->vertices.pop_back();
        isExtended = true;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void ResizableAngular<Dim, Spec>::actualizeBufferBeforeDraw(
        void) const noexcept
    {
        if (isExtended) {
            {
                BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
                this->vertexBuffer.setBufferData(this->vertices);
            }
            isExtended = this->isModified = false;
        } else
            Angular<Dim, Spec>::actualizeBufferBeforeDraw();
    }

}

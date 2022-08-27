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
#pragma once

namespace mpgl {

    template <MeshTraitSpecifier Spec>
    template <
        std::ranges::input_range VRange,
        std::ranges::input_range IRange>
    requires (SameRangeType<VRange, typename DynamicMesh<Spec>::Vertex>,
        SameRangeType<IRange, IndicesTriangle>)
    DynamicMesh<Spec>::DynamicMesh(
        VRange&& vertices,
        IRange&& indices,
        std::string const& program)
            : DynamicMesh{
                std::ranges::begin(vertices),
                std::ranges::end(vertices),
                std::ranges::begin(indices),
                std::ranges::end(indices),
                program} {}

    template <MeshTraitSpecifier Spec>
    template <
        std::input_iterator VIter,
        std::input_iterator IIter,
        std::sentinel_for<VIter> VSent,
        std::sentinel_for<IIter> ISent>
    requires (SameIterType<VIter, typename DynamicMesh<Spec>::Vertex>,
        SameIterType<IIter, IndicesTriangle>)
    DynamicMesh<Spec>::DynamicMesh(
        VIter const& verticesBegin,
        VSent const& verticesEnd,
        IIter const& indicesBegin,
        ISent const& indicesEnd,
        std::string const& program)
            : Figure3D{program}, vertices{verticesBegin, verticesEnd},
            indices{indicesBegin, indicesEnd}
    {
        initializeBuffers();
        buildVertexViews();
    }

    template <MeshTraitSpecifier Spec>
    template <typename... Args>
        requires std::constructible_from<
            typename DynamicMesh<Spec>::Vertex, Args...>
    void DynamicMesh<Spec>::emplace(Args&&... args) {
        if (emptyVertices.size()) {
            vertices[emptyVertices.front()] = Vertex{
                std::forward<Args>(args)...};
            verticesView.insert(verticesView.begin()
                + emptyVertices.front(),
                VertexView{std::ref(*this), emptyVertices.front()});
            emptyVertices.pop_front();
        } else {
            verticesView.emplace_back(std::ref(*this), vertices.size());
            vertices.emplace_back(std::forward<Args>(args)...);
        }
    }

}

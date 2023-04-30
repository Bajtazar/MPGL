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

#include <MPGL/Core/Vertex/VertexCast.hpp>

namespace mpgl {

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndicesTriangle> IRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    [[nodiscard]] std::pair<VRange, IRange>
        SubdivisionTessellator::operator() (
            VRange vertices,
            IRange indices,
            uint8 tessellationSteps,
            Predicate pred) const
    {
        for (uint8 i = 0; i < tessellationSteps; ++i)
            indices = Algorithm{vertices, pred}(indices);
        return { vertices, indices };
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    SubdivisionTessellator::Algorithm<VRange, Pred>::Algorithm(
            VRange& vertices,
            Pred predicate)
                : predicate{std::move(predicate)},
                counter{static_cast<uint32>(vertices.size())},
                vertices{vertices} {}

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    template <UnderlyingRange<IndicesTriangle> IRange>
    [[nodiscard]] IRange SubdivisionTessellator::
        Algorithm<VRange, Pred>::operator() (IRange const& indices)
    {
        vertices.reserve(vertices.size() + 3 * indices.size() / 2);
        IRange newIndices;
        newIndices.reserve(4 * indices.size());
        for (auto const& triangle : indices)
            tessellateFace(newIndices, triangle);
        return newIndices;
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    template <UnderlyingRange<IndicesTriangle> IRange>
    void SubdivisionTessellator::Algorithm<VRange, Pred>
        ::tessellateFace(
            IRange& indices,
            IndicesTriangle const& triangle)
    {
        uint32 const n1 = getOrConstructVertex(
            triangle.firstVertex, triangle.secondVertex);;
        uint32 const n2 = getOrConstructVertex(
            triangle.secondVertex, triangle.thirdVertex);
        uint32 const n3 = getOrConstructVertex(
            triangle.firstVertex, triangle.thirdVertex);
        addTriangles(indices, triangle, n1, n2, n3);
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    template <UnderlyingRange<IndicesTriangle> IRange>
    void SubdivisionTessellator::Algorithm<VRange, Pred>
        ::addTriangles(
            IRange& indices,
            IndicesTriangle const& triangle,
            uint32 const new1,
            uint32 const new2,
            uint32 const new3) const
    {
        uint32 const v1 = triangle.firstVertex;
        uint32 const v2 = triangle.secondVertex;
        uint32 const v3 = triangle.thirdVertex;
        indices.push_back(IndicesTriangle{v1, new1, new3});
        indices.push_back(IndicesTriangle{new1, v2, new2});
        indices.push_back(IndicesTriangle{new2, v3, new3});
        indices.push_back(IndicesTriangle{new1, new2, new3});
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    uint32 SubdivisionTessellator::Algorithm<VRange, Pred>
        ::getOrConstructVertex(
            uint32 firstVertex,
            uint32 secondVertex)
    {
        uint64 tag = edgeTag(firstVertex, secondVertex);
        auto iter = verticesIDs.find(tag);
        if (iter != verticesIDs.end())
            return iter->second;
        auto id = counter++;
        generateVertex(tag);
        verticesIDs.emplace(tag, id);
        return id;
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    void SubdivisionTessellator::Algorithm<VRange, Pred>::
        generateVertex(uint64 tag)
    {
        uint64 firstVertexID = tag >> 32u;
        uint64 secondVertexID = tag & LowerMask;
        vertices.push_back(std::invoke(predicate,
            vertices[firstVertexID],
            vertices[secondVertexID]));
    }

    template <
        FlexibleRange VRange,
        std::invocable<
            std::ranges::range_value_t<VRange> const&,
            std::ranges::range_value_t<VRange> const&> Pred>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Pred,
                        std::ranges::range_value_t<VRange> const&,
                        std::ranges::range_value_t<VRange> const&>>)
    uint64 SubdivisionTessellator::Algorithm<VRange, Pred>
        ::edgeTag(
            uint64 firstVertex,
            uint64 secondVertex) noexcept
    {
        if (firstVertex > secondVertex)
            return (firstVertex << 32u) | secondVertex;
        return (secondVertex << 32u) | firstVertex;
    }

}

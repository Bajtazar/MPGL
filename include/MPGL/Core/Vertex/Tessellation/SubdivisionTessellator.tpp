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

#include <MPGL/Core/Vertex/VertexCast.hpp>

namespace mpgl {

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTriangle> IRange,
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
            IRange indicies,
            size_t tessellationSteps,
            Predicate pred) const
    {
        for (size_t i = 0; i < tessellationSteps; ++i)
            indicies = Algorithm{vertices, pred}(indicies);
        return { vertices, indicies };
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
                counter{vertices.size()},
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
    template <UnderlyingRange<IndiciesTriangle> IRange>
    [[nodiscard]] IRange SubdivisionTessellator::
        Algorithm<VRange, Pred>::operator() (IRange const& indicies)
    {
        vertices.reserve(vertices.size() + indicies.size() / 2);
        IRange newIndicies;
        newIndicies.reserve(4 * indicies.size());
        for (auto const& triangle : indicies)
            tessellateFace(newIndicies, triangle);
        return newIndicies;
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
    template <UnderlyingRange<IndiciesTriangle> IRange>
    void SubdivisionTessellator::Algorithm<VRange, Pred>
        ::tessellateFace(
            IRange& indicies,
            IndiciesTriangle const& triangle)
    {
        uint32 const n1 = getOrConstructVertex(
            triangle.firstVertex, triangle.secondVertex);;
        uint32 const n2 = getOrConstructVertex(
            triangle.secondVertex, triangle.thirdVertex);
        uint32 const n3 = getOrConstructVertex(
            triangle.firstVertex, triangle.thirdVertex);
        addTriangles(indicies, triangle, n1, n2, n3);
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
    template <UnderlyingRange<IndiciesTriangle> IRange>
    void SubdivisionTessellator::Algorithm<VRange, Pred>
        ::addTriangles(
            IRange& indicies,
            IndiciesTriangle const& triangle,
            uint32 const new1,
            uint32 const new2,
            uint32 const new3) const
    {
        uint32 const v1 = triangle.firstVertex();
        uint32 const v2 = triangle.secondVertex();
        uint32 const v3 = triangle.thirdVertex();
        indicies.push_back(IndiciesTriangle{v1, new1, new3});
        indicies.push_back(IndiciesTriangle{new1, v2, new2});
        indicies.push_back(IndiciesTriangle{new2, v3, new3});
        indicies.push_back(IndiciesTriangle{new1, new2, new3});
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
        return tag;
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
        uint64 firstVertexID = (tag & UpperMask) >> 32u;
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

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

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]] std::pair<VRange, IRange>
        CatmullClarkTessellator::operator() (
            VRange vertices,
            IRange indices,
            uint8 tessellationSteps,
            Predicate pred) const
    {
        for (uint8 i = 0; i < tessellationSteps; ++i) {
            auto&& [newVertices, newIndices] =
                Algorithm<VRange, IRange, Predicate>{pred}(
                    vertices, indices);
            vertices = std::move(newVertices);
            indices = std::move(newIndices);
        }
        return { vertices, indices };
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::Algorithm(Predicate builder)
            : builder{std::move(builder)} {}

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]] std::pair<VRange, IRange>
        CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
            ::operator() (
                VRange const& vertices,
                IRange const& indicies)
    {
        this->vertices.reserve(3 * indices.size() + vertices.size());
        buildFaces(vertices, indices);
        buildEdges(vertices, indices);
        return { this->vertices, this->indices };
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    void CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::buildFaces(
            VRange const& vertices,
            IRange const& indices)
    {
        faces.reserve(indices.size());
        for (IndiciesTetragon const& tetragon : indices) {
            faces.push_back(this->vertices.size());
            this->vertices.push_back(calculateVertex(vertices,
                tetragon.getFirstVertex(),
                tetragon.getSecondVertex(),
                tetragon.getThirdVertex(),
                tetragon.getFourthVertex()));
        }
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    void CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::buildEdges(
            VRange const& vertices,
            IRange const& indices)
    {
        uint32 id = 0;
        for (IndiciesTetragon const& tetragon : indices) {
            buildEdge(vertices, id, tetragon.getFirstVertex(),
                tetragon.getSecondVertex());
            buildEdge(vertices, id, tetragon.getSecondVertex(),
                tetragon.getThirdVertex());
            buildEdge(vertices, id, tetragon.getThirdVertex(),
                tetragon.getFourthVertex());
            buildEdge(vertices, id++, tetragon.getFourthVertex(),
                tetragon.getFirstVertex());
        }
        buildBorderEdges(vertices, indices);
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    void CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::buildEdge(
            VRange const& vertices,
            uint32 tetragonID,
            uint32 firstVertex,
            uint32 secondVertex)
    {
        uint64 const tag = generateTag(firstVertex, secondVertex);
        auto const iter = edgeFaces.find(tag);
        if (iter != edgeFaces.end()) {
            edges.emplace(tag, { this->vertices.size(), tetragonID, iter->second });
            this->vertices.push_back(calculateVertex(firstVertex,
                secondVertex, tetragonID, iter->second));
            edgeFaces.erase(iter);
        } else
            edgeFaces.emplace(tag, tetragonID);
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    void CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::buildBorderEdges(
            VRange const& vertices,
            IRange const& indices)
    {
        for (auto const& [tag, id] : edgeFaces) {
            uint32 first = tag >> 32u;
            uint32 second = tag & LowerMask;
            edges.emplace(tag, { this->vertices.size(), id, Placeholder });
            this->vertices.push_back(calculateVertex(vertices, first, second));
        }
        edgeFaces.clear();
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]]
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::Vertex
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::calculateVertex(
            VRange const& vertices,
            uint32 firstIndex,
            uint32 secondIndex,
            uint32 thirdIndex,
            uint32 fourthIndex)
    {
        auto const facePos = (
            Vector{vertices[firstIndex] | cast::position}
            + Vector{vertices[secondIndex] | cast::position}
            + Vector{vertices[thirdIndex] | cast::position}
            + Vector{vertices[fourthIndex] | cast::position}) / 4.f;
        return std::invoke(builder, facePos);
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]]
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::Vertex
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::calculateVertex(
            VRange const& vertices,
            uint32 firstIndex,
            uint32 secondIndex)
    {
        auto const facePos = (
            Vector{vertices[firstIndex] | cast::position}
            + Vector{vertices[secondIndex] | cast::position}) / 2.f;
        return std::invoke(builder, facePos);
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]]
        uint64 CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
            ::generateTag(
                uint64 first,
                uint64 second) noexcept
    {
        if (first > second)
            return (first << 32u) | second;
        return (second << 32u) | first;
    }

}

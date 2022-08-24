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

#include <algorithm>

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
        generateVerticesDependencies();
        calculateTessellatedVertices();
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
        for (IndiciesTetragon const& tetragon : indices)
            this->vertices.push_back(calculateVertex(vertices,
                tetragon.getFirstVertex(),
                tetragon.getSecondVertex(),
                tetragon.getThirdVertex(),
                tetragon.getFourthVertex()));
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
            this->vertices.push_back(calculateEdgeVertex(firstVertex,
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
    void CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::generateVerticesDependencies(void)
    {
        for (auto const& [tag, edge] : edges) {
            uint32 first = tag >> 32u;
            uint32 second = tag & LowerMask;
            addToGraph(first, &edge);
            addToGraph(second, &edge);
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
        ::addToGraph(uint32 vertex, Edge const* edgePtr)
    {
        if (auto iter = graph.find(vertex); iter != graph.end())
            iter->second.push_back(edgePtr);
        else
            graph.emplace(vertex, { edgePtr });
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
        ::calculateTessellatedVertices(void)
    {
        for (auto const& [vertex, edges] : graph) {
            auto const middleAvg = averageOfEdges(edges);
            auto const facesAvg = averageOfFaces(edges);
            Vector const& current = (facesAvg + 2.f * middleAvg +
                Vector{vertex | cast::position}) / 4.f;
            uint32 id = this->vertices.size();
            this->vertices.push_back(std::invoke(builder, current));
            addTetragonVertices(id, edges);
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
        ::addTetragonVertices(
        uint32 vertex,
        Edges& edges)
    {
        std::ranges::sort(edges, std::greater{}, &Edge::firstFaceID);
        if (edges.front()->firstFaceID != Placeholder)
            findAndAddTetragon(vertex, edges, edges.front());
        for (Edge const* edge : edges | std::views::drop(1))
            findAndAddTetragon(vertex, edges, edge);
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
        ::findAndAddTetragon(
            uint32 vertex,
            Edges& edges,
            Edge const* edge)
    {
        auto iter = std::ranges::binary_search(edges,
            edge->firstFaceID, std::greater{}, &Edge::secondFaceID);
        addTetragon(vertex, *edge, **iter);
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
        ::addTetragon(
            uint32 vertex,
            Edge const& leftEdge,
            Edge const& rightEdge)
    {
        indices.emplace_back(
            vertex,
            leftEdge->vertex,
            leftEdge->firstFaceID,
            rightEdge->vertex
        );
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]] CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::Vector
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::averageOfEdges(Edges const& edges)
    {
        Vector avg;
        for (uint32 index : edges | std::views::transform(&Edge::vertex))
            avg += Vector{vertices[index] | cast::position};
        return avg / avg.size();
    }

    template <
        FlexibleRange VRange,
        UnderlyingRange<IndiciesTetragon> IRange,
        std::invocable<Vector3f const&> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, Vector3f const&>>)
    [[nodiscard]] CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::Vector
    CatmullClarkTessellator::Algorithm<VRange, IRange, Predicate>
        ::averageOfFaces(Edges const& edges)
    {
        Vector avg;
        for (Edge const* edge : edges) {
            avg += Vector{vertices[edge->firstFaceID]
                | cast::position};
            if (edge->secondFaceID != Placeholder)
                avg += Vector{vertices[edge->secondFaceID]
                    | cast::position};
        }
        return avg / avg.size();
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
        ::calculateEdgeVertex(
            VRange const& vertices,
            uint32 firstIndex,
            uint32 secondIndex,
            uint32 thirdIndex,
            uint32 fourthIndex)
    {
        auto const facePos = (
            Vector{vertices[firstIndex] | cast::position}
            + Vector{vertices[secondIndex] | cast::position}
            + Vector{this->vertices[thirdIndex] | cast::position}
            + Vector{this->vertices[fourthIndex] | cast::position}) / 4.f;
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

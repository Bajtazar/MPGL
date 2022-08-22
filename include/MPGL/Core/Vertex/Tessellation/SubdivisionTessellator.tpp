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
        FlexibleRange IRange,
        std::invocable<VRange const&, uint32, uint32> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<IRange, IndiciesTriangle>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, VRange const&, uint32, uint32>>)
    [[nodiscard]] std::pair<VRange, IRange>
        SubdivisionTessellator::operator() (
            VRange vertices,
            IRange indicies,
            size_t tessellationSteps,
            Predicate pred) const
    {
        for (size_t i = 0; i < tessellationSteps; ++i) {
            auto&& [vert, ind] = tessellate(vertices, indicies, pred);
            vertices = std::move(vert);
            indicies = std::move(ind);
        }
        return { vertices, indicies };
    }

    template <
        FlexibleRange VRange,
        FlexibleRange IRange,
        std::invocable<VRange const&, uint32, uint32> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<IRange, IndiciesTriangle>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, VRange const&, uint32, uint32>>)
    [[nodiscard]] std::pair<VRange, IRange>
        SubdivisionTessellator::tessellate(
            VRange const& vertices,
            IRange const& indicies,
            Predicate pred) const
    {
        VRange newVertices;
        IRange newIndicies;
        newVertices.reserve(12 * vertices.size());
        newIndicies.reserve(4 * indicies.size());
        size_t index = 0;
        for (auto const& triangle : indicies) {
            addVertices(vertices, newVertices, triangle);
            addIndicies(newIndicies, index);
        }
        return { newVertices, newIndicies };
    }

    template <
        FlexibleRange VRange,
        std::invocable<VRange const&, uint32, uint32> Predicate>
            requires (
                VertexType<std::ranges::range_value_t<VRange>>
                && SameRangeType<VRange, std::invoke_result_t<
                    Predicate, VRange const&, uint32, uint32>>)
    void SubdivisionTessellator::addVertices(
        VRange const& vertices,
        VRange& newVertices,
        IndiciesTriangle const& triangle,
        Predicate pred) const
    {
        auto const t1 = pred(
            vertices, triangle.firstVertex, triangle.secondVertex);
        auto const t2 = pred(
            vertices, triangle.firstVertex, triangle.thirdVertex);
        auto const t3 = pred(
            vertices, triangle.secondVertex, triangle.thirdVertex);

        newVertices.emplace_back(vertices[triangle.firstVertex]);
        newVertices.emplace_back(t1);
        newVertices.emplace_back(t2);

        newVertices.emplace_back(t1);
        newVertices.emplace_back(vertices[triangle.secondVertex]);
        newVertices.emplace_back(t3);

        newVertices.emplace_back(t2);
        newVertices.emplace_back(t3);
        newVertices.emplace_back(vertices[triangle.thirdVertex]);

        newVertices.emplace_back(t1);
        newVertices.emplace_back(t2);
        newVertices.emplace_back(t3);
    }

    template <FlexibleRange IRange>
        requires (SameRangeType<IRange, IndiciesTriangle>)
    void SubdivisionTessellator::addIndicies(
        IRange& indicies,
        uint32& index) const
    {
        for (uint8 i = 0; i < 4; ++i)
            indicies.emplace_back(
                index + i * 3,
                index + i * 3 + 1,
                index + i * 3 + 2);
        index += 12;
    }

}

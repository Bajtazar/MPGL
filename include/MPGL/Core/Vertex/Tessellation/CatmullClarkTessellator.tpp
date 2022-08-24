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

        buildFaces(vertices, indices);
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
            auto const facePos = (Vector3f{vertices[tetragon.getFirstVertex()] | cast::position}
                + Vector3f{vertices[tetragon.getSecondVertex()] | cast::position}
                + Vector3f{vertices[tetragon.getThirdVertex()] | cast::position}
                + Vector3f{ vertices[tetragon.getFourthVertex()] | cast::position}) / 4.f;
            faces.push_back(std::invoke(builder, facePos));
        }
    }

}

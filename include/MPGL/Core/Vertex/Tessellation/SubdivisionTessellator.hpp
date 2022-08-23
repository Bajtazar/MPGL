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

#include <MPGL/Core/Vertex/Indicies/IndiciesTriangle.hpp>
#include <MPGL/Traits/Concepts.hpp>

#include <unordered_map>

namespace mpgl {

    /**
     * Tessellator that subdivides the triangles into the smaller ones
     * using the given tessellation predicate
     */
    class SubdivisionTessellator {
    public:

        /**
         * Performs tessellation on the given triangle mesh by the given
         * number of times and returns the tessellated mesh. Each step
         * will increase number of verticies by 12
         *
         * @tparam VRange the vertices range type
         * @tparam IRange the inidcies range type
         * @tparam Predicate the tessellation predicate type
         * @param vertices the vertices range
         * @param indicies the indicies range
         * @param tessellationSteps the number of tessellation steps
         * @param pred the tessellation predicate
         */
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
        [[nodiscard]] std::pair<VRange, IRange> operator() (
            VRange vertices,
            IRange indicies,
            size_t tessellationSteps,
            Predicate pred) const;
    private:
        template <
            FlexibleRange VRange,
            std::invocable<
                std::ranges::range_value_t<VRange> const&,
                std::ranges::range_value_t<VRange> const&> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate,
                            std::ranges::range_value_t<VRange> const&,
                            std::ranges::range_value_t<VRange> const&>>)
        class Algorithm {
        public:
            explicit Algorithm(
                VRange& vertices,
                Predicate predicate);

            template <UnderlyingRange<IndiciesTriangle> IRange>
            [[nodiscard]] IRange operator() (IRange const& indicies);

        private:
            using HashMap = std::unordered_map<uint64, uint32>;

            HashMap                                     verticesIDs = {};
            Predicate                                   predicate;
            uint32                                      counter;
            VRange&                                     vertices;

            template <UnderlyingRange<IndiciesTriangle> IRange>
            void tessellateFace(
                IRange& indicies,
                IndiciesTriangle const& triangle);

            template <UnderlyingRange<IndiciesTriangle> IRange>
            void addTriangles(
                IRange& indicies,
                IndiciesTriangle const& triangle,
                uint32 const new1,
                uint32 const new2,
                uint32 const new3) const;

            uint32 getOrConstructVertex(
                uint32 firstVertex,
                uint32 secondVertex);

            void generateVertex(uint64 tag);

            static uint64 edgeTag(
                uint64 firstVertex,
                uint64 secondVertex) noexcept;
        };

        static constexpr uint64                         UpperMask
            = 0xFFFFFFFF00000000;
        static constexpr uint64                         LowerMask
            = 0x00000000FFFFFFFF;
    };

    inline constexpr SubdivisionTessellator             subdivisionTessellator;

}

#include <MPGL/Core/Vertex/Tessellation/SubdivisionTessellator.tpp>

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
            FlexibleRange IRange,
            std::invocable<VRange const&, uint32, uint32> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<IRange, IndiciesTriangle>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, VRange const&, uint32, uint32>>)
        [[nodiscard]] std::pair<VRange, IRange> operator() (
            VRange vertices,
            IRange indicies,
            size_t tessellationSteps,
            Predicate pred) const;
    private:
        /**
         * Performs tessellation on the given triangle mesh and
         * returns the tessellated mesh
         *
         * @tparam VRange the vertices range type
         * @tparam IRange the inidcies range type
         * @tparam Predicate the tessellation predicate type
         * @param vertices the vertices range
         * @param indicies the indicies range
         * @param pred the tessellation predicate
         */
        template <
            FlexibleRange VRange,
            FlexibleRange IRange,
            std::invocable<VRange const&, uint32, uint32> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<IRange, IndiciesTriangle>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, VRange const&, uint32, uint32>>)
        [[nodiscard]] std::pair<VRange, IRange> tessellate(
            VRange const& vertices,
            IRange const& indicies,
            Predicate pred) const;

        /**
         * Apends a tessellated vertices to the mesh
         *
         * @tparam VRange the vertices range type
         * @tparam Predicate the tessellation predicate type
         * @param vertices the old figure vertices
         * @param newVertices the tessellated figure vertices
         * @param triangle the tessellated triangle
         * @param pred the tessellation predicate
         */
        template <
            FlexibleRange VRange,
            std::invocable<VRange const&, uint32, uint32> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, VRange const&, uint32, uint32>>)
        void addVertices(
            VRange const& vertices,
            VRange& newVertices,
            IndiciesTriangle const& triangle,
            Predicate pred) const;

        /**
         * Appends a tessellated vertices to the mesh
         *
         * @tparam IRange the inidcies range type
         * @param indicies the tessellated figure vertices
         * @param index the index of the lastly appended vertex
         */
        template <FlexibleRange IRange>
            requires (SameRangeType<IRange, IndiciesTriangle>)
        void addIndicies(IRange& indicies, uint32& index) const;
    };

}

#include <MPGL/Core/Vertex/Tessellation/SubdivisionTessellator.tpp>

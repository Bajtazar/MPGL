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

#include <MPGL/Core/Vertex/Indicies/IndiciesTetragon.hpp>
#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Core/Vertex/VertexTraits.hpp>
#include <MPGL/Core/Vertex/VertexCast.hpp>

#include <unordered_map>

namespace mpgl {

    class CatmullClarkTessellator {
    public:
        template <
            FlexibleRange VRange,
            UnderlyingRange<IndiciesTetragon> IRange,
            std::invocable<Vector3f const&> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, Vector3f const&>>)
        [[nodiscard]] std::pair<VRange, IRange> operator() (
            VRange vertices,
            IRange indicies,
            uint8 tessellationSteps,
            Predicate pred) const;
    private:
        template <
            FlexibleRange VRange,
            UnderlyingRange<IndiciesTetragon> IRange,
            std::invocable<Vector3f const&> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, Vector3f const&>>)
        class Algorithm {
        public:
            explicit Algorithm(Predicate builder);

            [[nodiscard]] std::pair<VRange, IRange> operator() (
                VRange const& vertices,
                IRange const& indicies);
        private:
            VRange                                      vertices;
            VRange                                      faces;
            IRange                                      indices;
            Predicate                                   builder;

            void buildFaces(
                VRange const& vertices,
                IRange const& indices);
        };
    };

}

#include <MPGL/Core/Vertex/Tessellation/CatmullClarkTessellator.tpp>

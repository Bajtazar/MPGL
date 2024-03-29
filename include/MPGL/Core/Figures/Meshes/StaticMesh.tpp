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

namespace mpgl {

    template <MeshTraitSpecifier Spec>
    template <
        std::ranges::input_range VRange,
        std::ranges::input_range IRange>
    requires (SameRangeType<VRange, typename StaticMesh<Spec>::Vertex>,
        SameRangeType<IRange, IndicesTriangle>)
    StaticMesh<Spec>::StaticMesh(
        VRange&& vertices,
        IRange&& indices,
        std::string const& program)
            : StaticMesh{
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
    requires (SameIterType<VIter, typename StaticMesh<Spec>::Vertex>,
        SameIterType<IIter, IndicesTriangle>)
    StaticMesh<Spec>::StaticMesh(
        VIter const& verticesBegin,
        VSent const& verticesEnd,
        IIter const& indicesBegin,
        ISent const& indicesEnd,
        std::string const& program)
            : Figure3D{program}, vertices{verticesBegin, verticesEnd},
            indices{indicesBegin, indicesEnd}
    {
        initializeBuffers();
    }

}

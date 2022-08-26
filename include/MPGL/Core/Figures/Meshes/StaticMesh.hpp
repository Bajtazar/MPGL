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

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>
#include <MPGL/Core/Vertex/Indices/IndicesTriangle.hpp>
#include <MPGL/Core/Figures/Meshes/MeshVertices.hpp>
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    template <MeshTraitSpecifier Spec = void>
    class StaticMesh : public Figure3D {
    public:
        using Vertex = MeshVertexT<Spec>;
        using Vertices = std::vector<Vertex>;
        using Indices = std::vector<IndicesTriangle>;

        StaticMesh(
            Vertices&& vertices,
            Indices&& Indices,
            ShaderProgram const& program);

        template <
            std::ranges::input_range VRange,
            std::ranges::input_range IRange>
        requires (SameRangeType<VRange, Vertex>,
            SameRangeType<IRange, IndicesTriangle>)
        StaticMesh(
            VRange&& vertices,
            IRange&& Indices,
            ShaderProgram const& program);

        template <
            std::input_iterator VIter,
            std::input_iterator IIter,
            std::sentinel_for<VIter> VSent,
            std::sentinel_for<IIter> ISent>
        requires (SameIterType<VIter, Vertex>,
            SameIterType<IIter, IndicesTriangle>)
        StaticMesh(
            VIter const& verticesBegin,
            VSent const& verticesEnd,
            IIter const& indicesBegin,
            ISent const& indicesEnd,
            ShaderProgram const& program);

        StaticMesh(StaticMesh const& staticMesh);

        StaticMesh(StaticMesh&& staticMesh) noexcept = default;

        StaticMesh& operator=(StaticMesh const& staticMesh);

        StaticMesh& operator=(
            StaticMesh&& staticMesh) noexcept = default;

        virtual void draw(void) const noexcept;

        virtual ~StaticMesh(void) noexcept = default;
    private:
        void reloadElementBuffer(void) const noexcept;

        Indices                                     indices;
        ElementArrayBuffer                          elementBuffer;
    };

    template class StaticMesh<void>;
    template class StaticMesh<uint8>;
    template class StaticMesh<uint16>;
    template class StaticMesh<uint32>;
    template class StaticMesh<int8>;
    template class StaticMesh<int16>;
    template class StaticMesh<int32>;
    template class StaticMesh<int64>;

}

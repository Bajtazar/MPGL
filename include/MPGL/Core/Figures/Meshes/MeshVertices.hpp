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

#include <MPGL/Core/Vertex/Vertex.hpp>
#include <MPGL/Core/Dimensions.hpp>
#include <MPGL/Utility/Adapter.hpp>
#include <MPGL/Core/Color.hpp>

namespace mpgl {

    /**
     * Contains informations about type of vertices used by the
     * given mesh
     *
     * @tparam Specifier the trait specifier
     */
    template <typename Specifier>
    struct MeshVertices;

    /**
     * Contains vertex type of the default mesh
     */
    template <>
    struct MeshVertices<void> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the first variant mesh
     */
    template <>
    struct MeshVertices<uint8> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the second variant mesh
     */
    template <>
    struct MeshVertices<uint16> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"normal", Vector3f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the third variant mesh
     */
    template <>
    struct MeshVertices<uint32> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"normal", Vector3f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the fourth variant mesh
     */
    template <>
    struct MeshVertices<int8> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the fifth variant mesh
     */
    template <>
    struct MeshVertices<int16> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the sixth variant mesh
     */
    template <>
    struct MeshVertices<int32> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"normal", Vector3f, DataType::Float32>
        >;
    };

    /**
     * Contains vertex type of the seventh variant mesh
     */
    template <>
    struct MeshVertices<int64> {
        using Type = Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"normal", Vector3f, DataType::Float32>
        >;
    };

    /**
     * Convinient shortcut for the mesh's vertex type
     *
     * @tparam Specifier the trait specifier
     */
    template <typename Tp>
    using MeshVertexT = typename MeshVertices<Tp>::Type;

    /**
     * Checks whether the given type is a valid mesh trait
     * specifier
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept MeshTraitSpecifier = requires {
        typename MeshVertices<Tp>::Type;
    };

}

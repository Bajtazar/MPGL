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

#include <MPGL/Core/Context/Buffers/Vertex.hpp>
#include <MPGL/Core/Dimensions.hpp>
#include <MPGL/Utility/Adapter.hpp>
#include <MPGL/Core/Color.hpp>

namespace mpgl {

    /**
     * Contains informations about vertices of the given dimensions
     * elliptic shape. Allows to change trait for the same number of
     * dimensions via specifier
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the trait specifier
     */
    template <Dimension Dim, typename Specifier>
    struct EllipticVertices;

    /**
     * Contains informations about vertices of the 2D elliptic
     */
    template <>
    struct EllipticVertices<dim::Dim2, void> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>>;

        using Vector = Vector2f;
        using Adapter = Adapter2D;

        /**
         * Generates a default vertex
         *
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(void) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position) noexcept;
    };

    /**
     * Contains informations about vertices of the 2D elliptic
     * which vertices can be colored indepedently
     */
    template <>
    struct EllipticVertices<dim::Dim2, int32> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>>;

        using Vector = Vector2f;
        using Adapter = Adapter2D;

        /**
         * Generates a default vertex
         *
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(void) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position) noexcept;
    };

    /**
     * Contains informations about vertices of the 3D elliptic
     */
    template <>
    struct EllipticVertices<dim::Dim3, void> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>>;

        using Vector = Vector3f;
        using Adapter = Adapter3D;

        /**
         * Generates a default vertex
         *
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(void) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position) noexcept;
    };

    /**
     * Contains informations about vertices of the 3D elliptic
     * which vertices can be colored indepedently
     */
    template <>
    struct EllipticVertices<dim::Dim3, int32> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>>;

        using Vector = Vector3f;
        using Adapter = Adapter3D;

        /**
         * Generates a default vertex
         *
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(void) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position) noexcept;
    };

    /**
     * Checks whether the given specifier is valid elliptic vertices
     * specifier
     *
     * @tparam Tp the checked specifier
     * @tparam Dim the elliptic vertices' dimension
     */
    template <typename Tp, class Dim>
    concept EllipticTraitSpecifier = Dimension<Dim> &&
        requires (Vector<float32, Dim::orthogonal_space_degree> vector)
        {
            typename EllipticVertices<Dim, Tp>::Vertex;
            typename EllipticVertices<Dim, Tp>::Vector;
            typename EllipticVertices<Dim, Tp>::Adapter;
            EllipticVertices<Dim, Tp>::defaultVertex();
            EllipticVertices<Dim, Tp>::buildVertex(vector);
    };

}

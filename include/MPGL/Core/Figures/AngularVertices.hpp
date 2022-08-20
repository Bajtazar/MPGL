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
     * angluar shape. Allows to change trait for the same number of
     * dimensions via specifier
     *
     * @tparam Dim the dimension of the space
     * @tparam Specifier the trait specifier
     */
    template <Dimension Dim, typename Specifier>
    struct AngluarVertices;

    /**
     * Contains informations about vertices of the 2D angular
     */
    template <>
    struct AngluarVertices<dim::Dim2, void> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        using Vector = Vector2f;
        using Adapter = Adapter2D;

        /**
         * Returns a shader's name used by the angular
         *
         * @return the shader's name used by the angular
         */
        [[nodiscard]] static std::string shader(void) noexcept;

        /**
         * Generates a default vertex
         *
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(
            Color const& color) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position,
            Color const& color) noexcept;
    };

    /**
     * Contains informations about vertices of the 3D angular
     */
    template <>
    struct AngluarVertices<dim::Dim3, void> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        using Vector = Vector3f;
        using Adapter = Adapter3D;

        /**
         * Returns a shader's name used by the angular
         *
         * @return the shader's name used by the angular
         */
        [[nodiscard]] static std::string shader(void) noexcept;

        /**
         * Generates a default vertex
         *
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(
            Color const& color) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position,
            Color const& color) noexcept;
    };

    /**
     * Checks whether the given specifier is valid angular vertices
     * specifier
     *
     * @tparam Tp the checked specifier
     * @tparam Dim the angular vertices' dimension
     */
    template <typename Tp, class Dim>
    concept AngularTraitSpecifier = Dimension<Dim> &&
        requires (
            Color const& color,
            Vector<float32, Dim::orthogonal_space_degree> vector)
        {
            typename AngluarVertices<Dim, Tp>::Vertex;
            typename AngluarVertices<Dim, Tp>::Vector;
            typename AngluarVertices<Dim, Tp>::Adapter;
            { AngluarVertices<Dim, Tp>::shader() }
                -> std::same_as<std::string>;
            AngluarVertices<Dim, Tp>::defaultVertex(color);
            AngluarVertices<Dim, Tp>::buildVertex(vector, color);
    };

    /**
     * Contains informations about vertices of the texturable
     * 2D angular
     */
    template <>
    struct AngluarVertices<dim::Dim2, uint8> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter2D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        using Vector = Vector2f;
        using Adapter = Adapter2D;

        /**
         * Returns a shader's name used by the angular
         *
         * @return the shader's name used by the angular
         */
        [[nodiscard]] static std::string shader(void) noexcept;

        /**
         * Returns a convolution shader's name used by the angular
         *
         * @return the convolution shader's name used by the angular
         */
        [[nodiscard]] static std::string
            convolutionShader(void) noexcept;

        /**
         * Generates a default vertex
         *
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(
            Color const& color) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position,
            Color const& color) noexcept;
    };

    /**
     * Contains informations about vertices of the texturable
     * 3D angular
     */
    template <>
    struct AngluarVertices<dim::Dim3, uint8> {
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter3D, DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        using Vector = Vector3f;
        using Adapter = Adapter3D;

        /**
         * Returns a shader's name used by the angular
         *
         * @return the shader's name used by the angular
         */
        [[nodiscard]] static std::string shader(void) noexcept;

        /**
         * Returns a convolution shader's name used by the angular
         *
         * @return the convolution shader's name used by the angular
         */
        [[nodiscard]] static std::string
            convolutionShader(void) noexcept;

        /**
         * Generates a default vertex
         *
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex defaultVertex(
            Color const& color) noexcept;

        /**
         * Generates vertex
         *
         * @param position the constant reference to the position
         * object
         * @param color the constant reference to the color object
         * @return the default vertex
         */
        [[nodiscard]] static Vertex buildVertex(
            Vector const& position,
            Color const& color) noexcept;
    };

    /**
     * Checks whether the given specifier is valid textured angular
     * vertices specifier
     *
     * @tparam Tp the checked specifier
     * @tparam Dim the angular vertices' dimension
     */
    template <typename Tp, class Dim>
    concept TexturableAngularTraitSpecifier =
        AngularTraitSpecifier<Tp, Dim> && requires
    {
        { AngluarVertices<Dim, Tp>::convolutionShader() }
            -> std::same_as<std::string>;
    };

}

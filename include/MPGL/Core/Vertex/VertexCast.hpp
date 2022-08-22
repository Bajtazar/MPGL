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

#include <MPGL/Core/Vertex/VertexTraits.hpp>

namespace mpgl::cast {

    /**
     * Returns the desired field type from the given vertex
     *
     * @tparam Field the desired field name
     */
    template <TemplateString Field>
    struct VertexCast {

        /**
         * Returns the reference to the field element from
         * the given vertex reference
         *
         * @tparam Components the vertex's components
         * @param vertex the reference to the vertex
         * @return the reference to the field element
         */
        template <VertexComponents... Components>
            requires VertexField<Vertex<Components...>, Field>
        [[nodiscard]] VertexElementT<Field, Vertex<Components...>>&
            operator() (Vertex<Components...>& vertex) const noexcept
                { return get<Field>(vertex); }

        /**
         * Returns the constant reference to the field element from
         * the given vertex constant reference
         *
         * @tparam Components the vertex's components
         * @param vertex the constant reference to the vertex
         * @return the constant reference to the field element
         */
        template <VertexComponents... Components>
            requires VertexField<Vertex<Components...>, Field>
        [[nodiscard]] VertexElementT<Field, Vertex<Components...>> const&
            operator() (Vertex<Components...> const& vertex) const noexcept
                { return get<Field>(vertex); }
    };

    inline constexpr VertexCast<"position">             position;
    inline constexpr VertexCast<"color">                color;
    inline constexpr VertexCast<"texCoords">            texCoords;

    /**
     * Simplifies casting process by using a bit-and operator
     *
     * @tparam Field the name of the field
     * @tparam Components the vertex's components
     * @param vertex the reference to the vertex
     * @param cast the constant reference to the casting object
     * @return the reference to the vertex's field element
     */
    template <TemplateString Field, VertexComponents... Components>
    [[nodiscard]] VertexElementT<Field, Vertex<Components...>>&
        operator| (
            Vertex<Components...>& vertex,
            VertexCast<Field> const& cast) noexcept;

    /**
     * Simplifies casting process by using a bit-and operator
     *
     * @tparam Field the name of the field
     * @tparam Components the vertex's components
     * @param vertex the constant reference to the vertex
     * @param cast the constant reference to the casting object
     * @return the constant reference to the vertex's field element
     */
    template <TemplateString Field, VertexComponents... Components>
    [[nodiscard]] VertexElementT<Field, Vertex<Components...>> const&
        operator| (
            Vertex<Components...> const& vertex,
            VertexCast<Field> const& cast) noexcept;

}

#include <MPGL/Core/Vertex/VertexCast.tpp>

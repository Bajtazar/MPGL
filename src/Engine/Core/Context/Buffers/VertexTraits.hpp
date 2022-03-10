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

#include "Vertex.hpp"

namespace mpgl {

    /**
     * Helper metastruct. Provides information whether the given
     * type is the vertex type
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    class IsVertexTypeHelper {
        /**
         * Method choosen by the compiler when the given type
         * is the vertex type
         *
         * @tparam Components the vertex components types
         * @return std::true_type
         */
        template <VertexComponents... Components>
        static constexpr auto helper(Vertex<Components...>
            ) -> std::true_type;

        /**
         * Method choosen by the compiler when the given type
         * is not the vertex type
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given type is a vertex type
         */
        static constexpr bool value = std::same_as<
            decltype(helper(std::declval<Tp>())), std::true_type>;
    };

    /**
     * Checks whether the given type is the vertex
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept VertexType = IsVertexTypeHelper<Tp>::value;

    /**
     * Helper metastruct. Provides information whether the given
     * type is the vertex type containing given field
     *
     * @tparam Field the field name
     * @tparam Tp the checked type
     */
    template <TemplateString Field, typename Tp>
    class VertexFieldHelper {
        /**
         * Method choosen by the compiler when the given type
         * is the vertex type containing given field
         *
         * @tparam Components the vertex components types
         * @return std::true_type
         */
        template <VertexComponents... Comps>
            requires ((Field == Comps::componentName()) || ...)
        static constexpr auto helper(Vertex<Comps...>
            ) -> std::true_type;

        /**
         * Method choosen by the compiler when the given type
         * is not the vertex type
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr auto helper(...) -> std::false_type;
    public:
        /**
         * Returns whether the given type is a vertex type containing
         * the component with given field
         */
        static constexpr bool value = std::same_as<
            decltype(helper(std::declval<Tp>())), std::true_type>;

    };

    /**
     * Checks whether the given type is the vertex with the given
     * field
     *
     * @tparam Tp the checked type
     */
    template <typename Tp, TemplateString Field>
    concept VertexField = VertexFieldHelper<Field, Tp>::value;

    /**
     * Checks whether a range contains the vertices
     * containing given field
     *
     * @tparam Range the range type
     * @tparam Field the field name
     */
    template <class Range, TemplateString Field>
    concept FieldedVertexCollection =
        std::ranges::input_range<Range>
        && VertexField<std::ranges::range_value_t<Range>, Field>;

}

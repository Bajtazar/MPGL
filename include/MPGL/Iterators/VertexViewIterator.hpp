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

#include <iterator>

namespace mpgl {

    /**
     * Iterator returning a vertices with references to the
     * desired fields of the original vertices
     *
     * @tparam Iter the iterator with base vertices
     * @tparam Fields the fields that are allowed to be used
     */
    template <std::random_access_iterator Iter, TemplateString... Fields>
        requires (VertexField<std::iter_value_t<Iter>, Fields> && ...)
    class VertexViewIterator {
    private:
        using ProjectedVertex = std::iter_value_t<Iter>;
        using IterReturnType = decltype(*std::declval<Iter>());

        template <typename Tp>
        using Projected = std::conditional_t<
            std::is_assignable_v<IterReturnType, IterReturnType>,
            std::remove_reference_t<Tp>&,
            std::remove_reference_t<Tp> const&>;
    public:
        using value_type = Vertex<VertexComponent<Fields,
            Projected<VertexElementT<Fields, ProjectedVertex>>,
            VertexElement<Fields, ProjectedVertex>::ComponentType::vertexType()
        >...>;
        using iterator_category = std::output_iterator_tag;
        using difference_type = typename std::iter_difference_t<Iter>;
        using compare = std::compare_three_way_result_t<Iter, Iter>;

        /**
         * Construct a new Vertex View Iterator object from a
         * given iterator
         *
         * @param iter the projected iterator
         */
        explicit VertexViewIterator(Iter&& iter) noexcept
            : iter{iter} {}

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        VertexViewIterator& operator++(void) noexcept
            { ++iter; return *this; }

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] VertexViewIterator operator++(int) noexcept
            { auto temp = *this; ++iter; return temp; }

        /**
         * Decrements iterator by one
         *
         * @return reference to this object
         */
        VertexViewIterator& operator--(void) noexcept
            { --iter; return *this; }

        /**
         * Post-decrements iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] VertexViewIterator operator--(int) noexcept
            { auto temp = *this; --iter; return temp; }

        /**
         * Returns the view vertex
         *
         * @return the view vertex
         */
        [[nodiscard]] value_type operator*(void) noexcept
            { return value_type{std::ref(get<Fields>(*iter))...}; }

        /**
         * Increments iterator by the given distance
         *
         * @param offset the incremented distance
         * @return reference to this object
         */
        VertexViewIterator& operator+=(
            difference_type offset) noexcept
                { iter += offset; return *this; }

        /**
         * Decrements iterator by the given distance
         *
         * @param offset the decremented distance
         * @return reference to this object
         */
        VertexViewIterator& operator-=(
            difference_type offset) noexcept
                { iter -= offset; return *this; }

        /**
         * Returns a view shifted by the given offset
         *
         * @param offset the incremented distance
         * @return the view shifted by the given offse
         */
        [[nodiscard]] value_type operator[](
            difference_type offset) noexcept
                { return value_type{std::ref(get<Fields>(*(iter + offset)))...}; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted iterator
         */
        [[nodiscard]] friend VertexViewIterator
            operator+ (VertexViewIterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp += right; return temp; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the distance
         * @param right the iterator
         * @return the shifted iterator
         */
        [[nodiscard]] friend VertexViewIterator
            operator+ (difference_type left,
                VertexViewIterator const& right) noexcept
            { auto temp = right; temp += left; return temp; }

        /**
         * Subtracts given distance from iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted operator
         */
        [[nodiscard]] friend VertexViewIterator
            operator- (VertexViewIterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp -= right; return temp; }

        /**
         * Returns distance between iterators
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return difference_type
         */
        [[nodiscard]] friend difference_type
            operator- (VertexViewIterator const& left,
                VertexViewIterator const& right) noexcept
            { return left.iter - right.iter; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend bool
            operator== (VertexViewIterator const& left,
                VertexViewIterator const& right) noexcept
            { return left.iter == right.iter; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend compare
            operator<=> (VertexViewIterator const& left,
                VertexViewIterator const& right) noexcept
            { return left.iter <=> right.iter; }
    private:
        Iter                                    iter;
    };

    /**
     * Creates Vertex View Iterator type from the given
     * iterator type and template string holder type
     *
     * @tparam Iter the random access iterator
     * @tparam Tp the holder type
     */
    template <std::random_access_iterator Iter, typename Tp>
    struct VertexViewIterFromHolder;

    /**
     * Creates Vertex View Iterator type from the given
     * iterator type and template string holder type
     *
     * @tparam Iter the random access iterator
     * @tparam Fields the views fields
     */
    template <std::random_access_iterator Iter,
        TemplateString... Fields>
    struct VertexViewIterFromHolder<Iter, TSHolder<Fields...>> {
        using Type = VertexViewIterator<Iter, Fields...>;
    };

    /**
     * Returns the Vertex View Iterator type from the given
     * random access iterator type and template string
     * holder type
     *
     * @tparam Iter the random access iterator
     * @tparam Tp the holder type
     */
    template <std::random_access_iterator Iter, typename Tp>
    using VertexViewIterFromHolderT = typename
        VertexViewIterFromHolder<Iter, Tp>::Type;

}

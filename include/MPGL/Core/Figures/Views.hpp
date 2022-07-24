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

#include <MPGL/Core/Context/Buffers/VertexTraits.hpp>

namespace mpgl {

    /**
     * Allows to get only elements with the given field name
     * from the range of vertices
     *
     * @tparam Field the field name
     * @tparam Range the range with vertices
     */
    template <TemplateString Field, std::ranges::view Range>
        requires (FieldedVertexCollection<Range, Field>)
    class VertexComponentView
        : public std::ranges::view_interface<
            VertexComponentView<Field, Range>>
    {
    public:
        using ValueType = VertexElementT<Field,
            std::ranges::range_value_t<Range>>;

        /**
         * Construct a new vertex component view object
         */
        constexpr explicit VertexComponentView(
            void) noexcept = default;

        /**
         * Construct a new vertex component view object
         * from the given range object
         *
         * @param baseObj the given range object
         */
        constexpr explicit VertexComponentView(Range baseObj)
            : baseObj{baseObj} {}

        /**
         * The inner view iterator
         */
        class iterator {
        public:
            using inner_iter                  = std::ranges::iterator_t<Range>;
            using value_type                  = ValueType;
            using reference                   = value_type&;
            using pointer                     = value_type*;
            using iterator_category           = std::input_iterator_tag;
            using difference_type             = std::ptrdiff_t;

            /**
             * Construct a new iterator object
             */
            explicit iterator(void) noexcept = default;

            /**
             * Construct a new iterator object from the given
             * inner iter object
             *
             * @param iter the inner iter object
             */
            explicit iterator(inner_iter const& iter) noexcept
                : iter{iter} {}

            /**
             * Pre-increments the inner iterator by one
             *
             * @return the reference to object
             */
            iterator& operator++(void) noexcept
                { ++iter; return *this; }

            /**
             * Post-increments the inner iterator by one
             *
             * @return the copy of the iterator
             */
            [[nodiscard]] iterator operator++(int) noexcept
                { auto tmp = *this; ++iter; return tmp; }

            /**
             * Returns the iterator's value
             *
             * @return the iterator's value
             */
            [[nodiscard]] auto operator*(void) const noexcept
                -> decltype(get<Field>(*std::declval<inner_iter>()))
                    { return get<Field>(*iter); }

            /**
             * Returns a pointer to the iterator's value
             *
             * @return a pointer to the iterator's value
             */
            [[nodiscard]] auto operator->(void) const noexcept
                -> decltype(&get<Field>(*std::declval<inner_iter>()))
                    { return &get<Field>(*iter); }

            /**
             * Returns whether two iterators are equal
             *
             * @param left the left iterator object
             * @param right the right iterator object
             * @return if the iterators are equal
             */
            [[nodiscard]] friend bool operator== (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.iter == right.iter; }
        private:
            inner_iter                          iter;
        };

        /**
         * Returns the constant reference to the range held by
         * the view
         *
         * @return the constant reference to the range
         */
        [[nodiscard]] constexpr Range const& base(void) const &
            { return baseObj; }

        /**
         * Returns the rvalue to the range held by
         * the view
         *
         * @return the rvalue to the range
         */
        [[nodiscard]] constexpr Range&& base(void) &&
            { return std::move(baseObj); }

        /**
         * Returns an iterator to the begining of the range
         *
         * @return the iterator to the begining of the range
         */
        [[nodiscard]] constexpr iterator begin(void) const
            { return iterator{ std::ranges::begin(baseObj) }; }

        /**
         * Returns an iterator to the end of the range
         *
         * @return the iterator to the end of the range
         */
        [[nodiscard]] constexpr iterator end(void) const
            { return iterator{ std::ranges::end(baseObj) }; }

        /**
         * Returns the size of the range
         *
         * @return the size of the range
         */
        [[nodiscard]] constexpr auto size(void) const noexcept
            requires std::ranges::sized_range<Range>
                { return std::ranges::size(baseObj); }
    private:
        Range                                   baseObj;
    };

    template <TemplateString Field, class Range>
    VertexComponentView(Range&& base) ->
        VertexComponentView<Field, std::views::all_t<Range>>;

    namespace details {

        /**
         * The adaptor closure for the vertex component view
         *
         * @tparam Field the field name
         */
        template <TemplateString Field>
        struct VertexComponentViewAdaptorClosure {
            /**
             * Constructs the vertex component view adaptor closure
             */
            constexpr VertexComponentViewAdaptorClosure(
                void) noexcept = default;

            /**
             * Returns a view to the given range
             *
             * @tparam Range the range type
             * @param range the vertices range
             * @return the view of the range
             */
            template <std::ranges::viewable_range Range>
            [[nodiscard]] constexpr auto operator() (
                Range&& range) const noexcept;

        };

        /**
         * The adaptor for the vertex component view
         *
         * @tparam Field the field name
         */
        template <TemplateString Field>
        struct VertexComponentViewAdaptor {
            /**
             * Returns a view to the given range
             *
             * @tparam Range the range type
             * @param range the vertices range
             * @return the view of the range
             */
            template <std::ranges::viewable_range Range>
            [[nodiscard]] constexpr auto operator() (
                Range&& range) const noexcept;

            /**
             * Returns a closure for the given view
             *
             * @return the view closure
             */
            [[nodiscard]] constexpr auto operator() (
                void) const noexcept;

        };

        /**
         * Connects the closure with the range
         *
         * @tparam Range the range type
         * @tparam Field the field name
         * @param range the vertices range
         * @param closure the view closure
         * @return the view of the range
         */
        template <std::ranges::viewable_range Range,
            TemplateString Field>
        [[nodiscard]] constexpr auto operator | (
            Range&& range,
            VertexComponentViewAdaptorClosure<Field> const& closure
            ) noexcept;

        /**
         * Connects the adaptor with the range
         *
         * @tparam Range the range type
         * @tparam Field the field name
         * @param range the vertices range
         * @param adaptor the view adaptor
         * @return the view of the range
         */
        template <std::ranges::viewable_range Range,
            TemplateString Field>
        [[nodiscard]] constexpr auto operator | (
            Range&& range,
            VertexComponentViewAdaptor<Field> const& adaptor) noexcept;

    }

    namespace views {

        /// The position view
        inline constexpr
            details::VertexComponentViewAdaptor<"position">         position;
        /// The color view
        inline constexpr
            details::VertexComponentViewAdaptor<"color">            color;
        /// The texure position view
        inline constexpr
            details::VertexComponentViewAdaptor<"texCoords">        texCoords;

    }

}

#include <MPGL/Core/Figures/Views.tpp>

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

#include <MPGL/Mathematics/Tensors/TensorTraits.hpp>

namespace mpgl {

    template <typename MatrixTp>
    class ColumnView : public std::ranges::view_interface<
        ColumnView<MatrixTp>>
    {
    public:
        using raw_matrix_type = std::remove_reference_t<decltype(
            std::declval<MatrixTp>().base())>;
        using matrix_type = std::remove_cvref_t<raw_matrix_type>;
        using reference = decltype(
                std::declval<raw_matrix_type>()[std::declval<Vector2u>()]);
        using value_type = std::remove_reference_t<reference>;
        using vector_form = Vector<
            std::remove_cv_t<value_type>, matrix_type::width()>;

        constexpr explicit ColumnView(
            MatrixTp&& matrix,
            std::size_t columnID) noexcept;

        constexpr ColumnView& operator=(
            vector_form const& vector) noexcept;

        [[nodiscard]] constexpr operator
            vector_form() const noexcept;

        class iterator {
        public:
            using reference = decltype(
                std::declval<raw_matrix_type>()[std::declval<Vector2u>()]);
            using value_type = std::remove_reference_t<reference>;
            using pointer = value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;

            constexpr explicit iterator(void) noexcept = default;

            constexpr explicit iterator(
                MatrixTp&& matrix,
                std::size_t columnID,
                std::size_t rowID = 0) noexcept;

            constexpr iterator& operator++(void) noexcept;

            [[nodiscard]] constexpr iterator operator++(
                int) noexcept;

            constexpr iterator& operator--(void) noexcept;

            [[nodiscard]] constexpr iterator operator--(
                int) noexcept;

            [[nodiscard]] constexpr reference operator*(
                void) const noexcept;

            [[nodiscard]] constexpr pointer operator->(
                void) const noexcept;

            constexpr iterator& operator+=(difference_type offset
                ) noexcept;

            constexpr iterator& operator-=(difference_type offset
                ) noexcept;

            [[nodiscard]] constexpr reference
                operator[](difference_type offset
                    ) const noexcept;

            [[nodiscard]] friend constexpr iterator operator+ (
                iterator const& left,
                difference_type right) noexcept
                    { return iterator{left.ownerPtr, left.rowId + right}; }

            [[nodiscard]] friend constexpr iterator operator+ (
                difference_type left,
                iterator const& right) noexcept
                    { return iterator{right.ownerPtr, right.rowId + left}; }

            [[nodiscard]] friend constexpr iterator operator- (
                iterator const& left,
                difference_type right) noexcept
                    { return iterator{left.ownerPtr, left.rowId - right}; }

            [[nodiscard]] friend constexpr difference_type
                operator- (
                    iterator const& left,
                    iterator const& right) noexcept
                        { return (difference_type)left.rowID - right.rowID; }

            [[nodiscard]] friend constexpr auto operator<=> (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.rowID <=> right.rowID; }

            [[nodiscard]] friend constexpr bool operator== (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.rowID == right.rowID; }
        private:
            MatrixTp                        matrix = {};
            std::size_t                     columnID = 0;
            std::size_t                     rowID = 0;
        };

        using reverse_iterator = std::reverse_iterator<iterator>;

        [[nodiscard]] constexpr reference operator[] (
            std::size_t index) const noexcept;

        constexpr ColumnView& operator+=(
            ColumnView const& right) noexcept;

        constexpr ColumnView& operator-=(
            ColumnView const& right) noexcept;

        constexpr ColumnView& operator*=(
            ColumnView const& right) noexcept;

        constexpr ColumnView& operator/=(
            ColumnView const& right) noexcept;

        constexpr ColumnView& operator+=(
            value_type const& right) noexcept;

        constexpr ColumnView& operator-=(
            value_type const& right) noexcept;

        constexpr ColumnView& operator*=(
            value_type const& right) noexcept;

        constexpr ColumnView& operator/=(
            value_type const& right) noexcept;

        [[nodiscard]] constexpr MatrixTp const& base(void) const &
            { return matrix; }

        [[nodiscard]] constexpr MatrixTp&& base(void) &&
            { return std::move(matrix); }

        [[nodiscard]] constexpr iterator begin(
            void) const noexcept
                { return iterator{ matrix.base(), columnID }; }

        [[nodiscard]] constexpr iterator end(
            void) const noexcept
                { return iterator{ matrix.base(), columnID, matrix.base().size() }; }

        [[nodiscard]] constexpr reverse_iterator
            rbegin(void) const noexcept
                { return reverse_iterator{ end() - 1 }; }

        [[nodiscard]] constexpr reverse_iterator
            rend(void) const noexcept
                { return reverse_iterator{ begin() - 1 }; }

        [[nodiscard]] static constexpr auto size(
            void) noexcept
                { return matrix_type::size(); }
    private:
        MatrixTp                            matrix;
        std::size_t                         columnID;
    };

    template <typename MatrixTp>
    ColumnView(MatrixTp&& base, size_t rowID) ->
        ColumnView<std::views::all_t<MatrixTp>>;

    template <typename Tp>
    class ColumnRangeView : public std::ranges::view_interface<
        ColumnRangeView<Tp>>
    {
    public:
        using matrix_type = std::remove_cvref_t<decltype(
            std::declval<Tp>().base())>;

        constexpr ColumnRangeView(Tp&& matrix) noexcept
            : matrix{matrix} {}

        class iterator {
        public:
            using value_type = ColumnView<Tp>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::output_iterator_tag;

            constexpr explicit iterator(void) noexcept = default;

            constexpr explicit iterator(
                Tp&& matrix,
                std::size_t columnID = 0) noexcept;

            constexpr iterator& operator++(void) noexcept;

            [[nodiscard]] constexpr iterator operator++(
                int) noexcept;

            [[nodiscard]] constexpr value_type operator*(
                void) const noexcept;

            [[nodiscard]] friend bool operator== (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.columnID == right.columnID; }
        private:
            Tp                              matrix = {};
            std::size_t                     columnID = 0;
        };

        [[nodiscard]] constexpr Tp const& base(void) const & noexcept
            { return matrix; }

        [[nodiscard]] constexpr Tp&& base(void) && noexcept
            { return std::move(matrix); }

        [[nodiscard]] constexpr iterator begin(void) const noexcept
            { return iterator{matrix.base()}; }

        [[nodiscard]] constexpr iterator end(void) const noexcept
            { return iterator{matrix.base(), matrix.base().width()}; }

        [[nodiscard]] static constexpr std::size_t size(
            void) noexcept
                { return matrix_type::width(); }
    private:
        Tp                                  matrix;
    };

    template <typename Tp>
    ColumnRangeView(Tp&& base) ->
        ColumnRangeView<std::views::all_t<Tp>>;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    template <typename MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |);

    namespace details {

        class ColumnViewAdaptorClosure {
        public:
            constexpr ColumnViewAdaptorClosure(
                std::size_t rowID) noexcept : rowID{rowID} {}

            template <typename Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;
        private:
            std::size_t                     rowID;
        };

        struct ColumnViewAdaptor {

            template <typename Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix,
                std::size_t rowID) const noexcept;

            [[nodiscard]] constexpr ColumnViewAdaptorClosure
                operator() (std::size_t rowID) const noexcept;

        };

        struct ColumnRangeViewAdaptorClosure {

            constexpr ColumnRangeViewAdaptorClosure(
                void) noexcept = default;

            template <typename Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;

        };

        struct ColumnRangeViewAdaptor {

            template <typename Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;

            [[nodiscard]] constexpr ColumnRangeViewAdaptorClosure
                operator() (void) const noexcept;

        };

        template <typename Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnViewAdaptorClosure const& closure) noexcept;

        template <typename Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnViewAdaptor const& adaptor) noexcept;

        template <typename Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnRangeViewAdaptorClosure const& closure) noexcept;

        template <typename Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnRangeViewAdaptor const& adaptor) noexcept;

    }

    namespace views {

        /// The singular column view
        inline constexpr details::ColumnViewAdaptor         column;

        /// The column range view
        inline constexpr details::ColumnRangeViewAdaptor    columns;

    }

}

#include <MPGL/Mathematics/Tensors/Views.tpp>

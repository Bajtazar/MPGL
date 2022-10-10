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

    template <MatrixType MatrixTp>
    class ColumnView : public std::ranges::view_interface<
        ColumnView<MatrixTp>>
    {
    public:
        using value_type = std::remove_cvref_t<decltype(
            std::declval<MatrixTp>()[std::declval<Vector2i>()])>;
        using vector_form = Vector<value_type,
            std::remove_cvref_t<MatrixTp>::height()>;

        constexpr explicit ColumnView(
            MatrixTp&& matrix,
            std::size_t columnID) noexcept;

        ColumnView& operator=(vector_form const& vector) noexcept;

        [[nodiscard]] operator vector_form() const noexcept;

        class iterator {
        public:
            using owner = ColumnView*;

            using reference = decltype(
                std::declval<MatrixTp>()[std::declval<Vector2i>()]);
            using value_type = std::remove_cvref_t<reference>;
            using pointer = std::remove_reference_t<reference>*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;

            constexpr explicit iterator(void) noexcept = default;

            constexpr explicit iterator(
                owner const& ownerPtr,
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
                difference_type right) noexcept;

            [[nodiscard]] friend constexpr iterator operator+ (
                difference_type left,
                iterator const& right) noexcept;

            [[nodiscard]] friend constexpr iterator operator- (
                iterator const& left,
                difference_type right) noexcept;

            [[nodiscard]] friend constexpr difference_type
                operator- (
                    iterator const& left,
                    iterator const& right) noexcept;

            [[nodiscard]] friend constexpr auto operator<=> (
                iterator const& left,
                iterator const& right) noexcept;

            [[nodiscard]] friend constexpr bool operator== (
                iterator const& left,
                iterator const& right) noexcept;
        private:
            std::size_t                     rowID = 0;
            owner                           ownerPtr = nullptr;
        };

        using reverse_itertor = std::reverse_iterator<iterator>;

        constexpr ColumnView& operator+=(ColumnView const& right);

        constexpr ColumnView& operator-=(ColumnView const& right);

        constexpr ColumnView& operator*=(ColumnView const& right);

        constexpr ColumnView& operator/=(ColumnView const& right);

        constexpr ColumnView& operator%=(
            ColumnView const& right) requires mpgl_Operable(Tp, %);

        constexpr ColumnView& operator^=(
            ColumnView const& right) requires mpgl_Operable(Tp, ^);

        constexpr ColumnView& operator&=(
            ColumnView const& right) requires mpgl_Operable(Tp, &);

        constexpr ColumnView& operator|=(
            ColumnView const& right) requires mpgl_Operable(Tp, |);

        constexpr ColumnView& operator+=(value_type const& right);

        constexpr ColumnView& operator-=(value_type const& right);

        constexpr ColumnView& operator*=(value_type const& right);

        constexpr ColumnView& operator/=(value_type const& right);

        constexpr ColumnView& operator%=(value_type const& right
            ) requires mpgl_Operable(value_type, %);

        constexpr ColumnView& operator^=(value_type const& right
            ) requires mpgl_Operable(value_type, ^);

        constexpr ColumnView& operator&=(value_type const& right
            ) requires mpgl_Operable(value_type, &);

        constexpr ColumnView& operator|=(value_type const& right
            ) requires mpgl_Operable(value_type, |);

        [[nodiscard]] constexpr MatrixTp const& base(void) const &
            { return matrix; }

        [[nodiscard]] constexpr MatrixTp&& base(void) &&
            { return std::move(matrix); }

        [[nodiscard]] constexpr iterator begin(
            void) const noexcept
                { return iterator{ *this }; }

        [[nodiscard]] constexpr iterator end(
            void) const noexcept
                { return iterator{ *this, matrix.height() }; }

        [[nodiscard]] constexpr reverse_iterator
            rbegin(void) const noexcept
                { return reverse_iterator{ end() - 1 }; }

        [[nodiscard]] constexpr reverse_iterator
            rend(void) const noexcept
                { return reverse_iterator{ begin() - 1 }; }

        [[nodiscard]] constexpr auto size(void) const noexcept
                { return matrix.height(); }
    private:
        MatrixTp                            matrix;
        std::size_t                         columnID;
    };

    template <MatrixType MatrixTp>
    ColumnView(MatrixTp&& base, size_t rowID) ->
        ColumnView<std::views::all_t<MatrixTp>>;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    template <MatrixType MatrixTp>
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

            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;
        private:
            std::size_t                     rowID;
        };

        struct ColumnViewAdaptor {

            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix,
                std::size_t rowID) const noexcept;

            [[nodiscard]] constexpr ColumnViewAdaptorClosure
                operator() (std::size_t rowID) const noexcept;

        };

        template <MatrixType Tp>
        [[nodiscard]] auto operator | (
            Tp&& matrix,
            ColumnViewAdaptorClosure const& closure) noexcept;

        template <MatrixType Tp>
        [[nodiscard]] auto operator | (
            Tp&& matrix,
            ColumnViewAdaptor const& adaptor) noexcept;

    }

    namespace views {

        /// The singular column view
        inline constexpr details::ColumnViewAdaptor     column;

    }

}

#include <MPGL/Mathematics/Tensors/Views.tpp>

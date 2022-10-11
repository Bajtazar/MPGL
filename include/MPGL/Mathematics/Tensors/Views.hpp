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

    namespace details {

        /**
         * Checks whether the given type is a specialization
         * of the matrix template class or if it is its view
         *
         * @tparam Tp the checked type
         */
        template <class Tp>
        concept MatrixView = MatrixType<Tp> || MatrixType<decltype(
            std::declval<Tp>().base())>;

    }

    /**
     * Views a column of a matrx
     *
     * @tparam MatrixTp the matrix specialization type or its
     * view type
     */
    template <details::MatrixView MatrixTp>
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

        /**
         * Constructs a new column view object
         *
         * @param matrix an universal reference to the view
         * of the matrix specialization
         * @param columnID an index of the viewed column
         */
        constexpr explicit ColumnView(
            MatrixTp&& matrix,
            std::size_t columnID) noexcept;

        /**
         * Assigns a vector with the same layout to this
         * column
         *
         * @param vector a constant reference to the assigned
         * vector
         * @return a reference to this object
         */
        constexpr ColumnView& operator=(
            vector_form const& vector) noexcept;

        /**
         * Casts viewed column into its vector form
         *
         * @return the column in its vector form
         */
        [[nodiscard]] constexpr operator
            vector_form() const noexcept;

        /**
         * Iterates over the elements of the matrix's column
         */
        class iterator {
        public:
            using reference = decltype(
                std::declval<raw_matrix_type>()[std::declval<Vector2u>()]);
            using value_type = std::remove_reference_t<reference>;
            using pointer = value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::random_access_iterator_tag;

            /**
             * Constructs a new iterator object
             */
            constexpr explicit iterator(void) noexcept = default;

            /**
             * Constructs a new iterator object
             *
             * @param matrix an universal reference to the view
             * of the matrix specialization
             * @param columnID an index of the viewed column
             * @param rowID an index of the first viewed row
             */
            constexpr explicit iterator(
                MatrixTp&& matrix,
                std::size_t columnID,
                std::size_t rowID = 0) noexcept;

            /**
             * Increments iterator by one
             *
             * @return reference to this object
             */
            constexpr iterator& operator++(void) noexcept;

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr iterator operator++(
                int) noexcept;

            /**
             * Decrements iterator by one
             *
             * @return reference to this object
             */
            constexpr iterator& operator--(void) noexcept;

            /**
             * Post-decrements iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr iterator operator--(
                int) noexcept;

            /**
             * Returns a reference to the matrix's column element
             *
             * @return reference to the matrix's column element
             */
            [[nodiscard]] constexpr reference operator*(
                void) const noexcept;

            /**
             * Returns a pointer to the matrix's column element
             *
             * @return pointer to the matrix's column element
             */
            [[nodiscard]] constexpr pointer operator->(
                void) const noexcept;

            /**
             * Increments iterator by the given distance
             *
             * @param offset the incremented distance
             * @return reference to this object
             */
            constexpr iterator& operator+=(difference_type offset
                ) noexcept;

            /**
             * Decrements iterator by the given distance
             *
             * @param offset the decremented distance
             * @return reference to this object
             */
            constexpr iterator& operator-=(difference_type offset
                ) noexcept;

            /**
             * Returns a reference to the element shifted by
             * the given offset
             *
             * @param offset the shift distance
             * @return the reference to the element shifted by
             * the given offset
             */
            [[nodiscard]] constexpr reference
                operator[](difference_type offset
                    ) const noexcept;

            /**
             * Adds given distance to an iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr iterator operator+ (
                iterator const& left,
                difference_type right) noexcept
                    { return iterator{left.ownerPtr, left.rowId + right}; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the distance
             * @param right the iterator
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr iterator operator+ (
                difference_type left,
                iterator const& right) noexcept
                    { return iterator{right.ownerPtr, right.rowId + left}; }

            /**
             * Subtracts given distance from iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted operator
             */
            [[nodiscard]] friend constexpr iterator operator- (
                iterator const& left,
                difference_type right) noexcept
                    { return iterator{left.ownerPtr, left.rowId - right}; }

            /**
             * Returns distance between iterators
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return difference_type
             */
            [[nodiscard]] friend constexpr difference_type
                operator- (
                    iterator const& left,
                    iterator const& right) noexcept
                        { return (difference_type)left.rowID - right.rowID; }

            /**
             * Compares two iterators to each other
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return the result of compare
             */
            [[nodiscard]] friend constexpr auto operator<=> (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.rowID <=> right.rowID; }

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
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

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference operator[] (
            std::size_t index) const noexcept;

        /**
         * Adds the elements of this column with
         * the given column elements
         *
         * @param right the added column
         * @return the reference to this column
         */
        constexpr ColumnView& operator+=(
            ColumnView const& right) noexcept;

        /**
         * Subtracts the elements of this column with
         * the given column elements
         *
         * @param right the subtracted column
         * @return the reference to this column
         */
        constexpr ColumnView& operator-=(
            ColumnView const& right) noexcept;

       /**
         * Multiplies the elements of this column with
         * the given column elements
         *
         * @param right the multiplicand column
         * @return the reference to this column
         */
        constexpr ColumnView& operator*=(
            ColumnView const& right) noexcept;

        /**
         * Divides the elements of this column by the
         * given column elements
         *
         * @param right the divider column
         * @return the reference to this column
         */
        constexpr ColumnView& operator/=(
            ColumnView const& right) noexcept;

        /**
         * Adds the given scalar value to the elements of
         * this column
         *
         * @param right the added scalar
         * @return the reference to this column
         */
        constexpr ColumnView& operator+=(
            value_type const& right) noexcept;

        /**
         * Subtracts the given scalar value to the elements of
         * this column
         *
         * @param right the subtracted scalar
         * @return the reference to this column
         */
        constexpr ColumnView& operator-=(
            value_type const& right) noexcept;

        /**
         * Multiplies the given scalar column to the elements of
         * this column
         *
         * @param right the multiplied scalar
         * @return the reference to this column
         */
        constexpr ColumnView& operator*=(
            value_type const& right) noexcept;

        /**
         * Divides the elements of this column by the given
         * scalar value
         *
         * @param right the divisor scalar
         * @return the reference to this column
         */
        constexpr ColumnView& operator/=(
            value_type const& right) noexcept;

        /**
         * Returns a constant reference to the underlying
         * matrix specialization's view
         *
         * @return the constant reference to the underlying
         * matrix specialization's view
         */
        [[nodiscard]] constexpr MatrixTp const& base(
            void) const & noexcept
                { return matrix; }

        /**
         * Returns a rvalue reference to the underlying
         * matrix specialization's view
         *
         * @return the rvalue reference to the underlying
         * matrix specialization's view
         */
        [[nodiscard]] constexpr MatrixTp&& base(void) && noexcept
            { return std::move(matrix); }

        /**
         * Returns an iterator to the begining of the column
         *
         * @return the iterator to the begining of the column
         */
        [[nodiscard]] constexpr iterator begin(
            void) const noexcept;

        /**
         * Returns an iterator to the end of the column
         *
         * @return the iterator to the end of the column
         */
        [[nodiscard]] constexpr iterator end(
            void) const noexcept;

        /**
         * Returns a reverse iterator to the end of the column
         *
         * @return the reverse iterator to the end of the column
         */
        [[nodiscard]] constexpr reverse_iterator
            rbegin(void) const noexcept
                { return reverse_iterator{ end() - 1 }; }

        /**
         * Returns a reverse iterator to the begining of
         * the column
         *
         * @return the reverse iterator to the begining of
         * the column
         */
        [[nodiscard]] constexpr reverse_iterator
            rend(void) const noexcept
                { return reverse_iterator{ begin() - 1 }; }

        /**
         * Returns the size of the column view
         *
         * @return the size of the column
         */
        [[nodiscard]] static constexpr auto size(
            void) noexcept
                { return matrix_type::size(); }
    private:
        MatrixTp                            matrix;
        std::size_t                         columnID;
    };

    template <details::MatrixView MatrixTp>
    ColumnView(MatrixTp&& base, size_t rowID) ->
        ColumnView<std::views::all_t<MatrixTp>>;

    /**
     * Views the range of matrix's columns
     *
     * @tparam MatrixTp the matrix specialization type or its
     * view type
     */
    template <details::MatrixView Tp>
    class ColumnRangeView : public std::ranges::view_interface<
        ColumnRangeView<Tp>>
    {
    public:
        using matrix_type = std::remove_cvref_t<decltype(
            std::declval<Tp>().base())>;

        /**
         * Constructs a new column range view object
         *
         * @param matrix an universal reference to the view
         * of the matrix specialization
         */
        constexpr ColumnRangeView(Tp&& matrix) noexcept
            : matrix{matrix} {}

        /**
         * Iterates over the matrix's columns
         */
        class iterator {
        public:
            using value_type = ColumnView<Tp>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::output_iterator_tag;

            /**
             * Constructs a new iterator object
             */
            constexpr explicit iterator(void) noexcept = default;

            /**
             * Constructs a new iterator object
             *
             * @param matrix an universal reference to the view
             * of the matrix specialization
             * @param columnID an index of the viewed column
             */
            constexpr explicit iterator(
                Tp&& matrix,
                std::size_t columnID = 0) noexcept;

            /**
             * Increments iterator by one
             *
             * @return reference to this object
             */
            constexpr iterator& operator++(void) noexcept;

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr iterator operator++(
                int) noexcept;

            /**
             * Returns a view to the matrix's column
             *
             * @return view to the matrix's column
             */
            [[nodiscard]] constexpr value_type operator*(
                void) const noexcept;

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
            [[nodiscard]] friend bool operator== (
                iterator const& left,
                iterator const& right) noexcept
                    { return left.columnID == right.columnID; }
        private:
            Tp                              matrix = {};
            std::size_t                     columnID = 0;
        };

        /**
         * Returns a constant reference to the underlying
         * matrix specialization's view
         *
         * @return the constant reference to the underlying
         * matrix specialization's view
         */
        [[nodiscard]] constexpr Tp const& base(
            void) const & noexcept
                { return matrix; }

        /**
         * Returns a rvalue reference to the underlying
         * matrix specialization's view
         *
         * @return the rvalue reference to the underlying
         * matrix specialization's view
         */
        [[nodiscard]] constexpr Tp&& base(void) && noexcept
            { return std::move(matrix); }

        /**
         * Returns an iterator to the begining of the column
         * range
         *
         * @return the iterator to the begining of the
         * column range
         */
        [[nodiscard]] constexpr iterator begin(void) const noexcept;

        /**
         * Returns an iterator to the end of the column range
         *
         * @return the iterator to the end of the column range
         */
        [[nodiscard]] constexpr iterator end(void) const noexcept;

        /**
         * Returns the size of the column range view
         *
         * @return the size of the column range
         */
        [[nodiscard]] static constexpr std::size_t size(
            void) noexcept
                { return matrix_type::width(); }
    private:
        Tp                                  matrix;
    };

    template <details::MatrixView Tp>
    ColumnRangeView(Tp&& base) ->
        ColumnRangeView<std::views::all_t<Tp>>;

    /**
     * Adds vector and column together
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    /**
     * Subtracts vector from the column
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    /**
     * Multiplies vector and column together
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    /**
     * Divides the elements of vector by the elements of the
     * column
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept;

    /**
     * Calculates the modulo of the vector and column elements
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    /**
     * Calculates the bitwise-xor of the vector and column
     * elements
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    /**
     * Calculates the bitwise-and of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    /**
     * Calculates the bitwise-or of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right column
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |);

    /**
     * Adds vector and column together
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    /**
     * Subtracts vector from the column
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    /**
     * Multiplies vector and column together
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    /**
     * Divides the elements of vector by the elements of the
     * column
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept;

    /**
     * Calculates the modulo of the vector and column elements
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %);

    /**
     * Calculates the bitwise-xor of the vector and column
     * elements
     *
     * @tparam MatrixTp the matrix's view type
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^);

    /**
     * Calculates the bitwise-and of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &);

    /**
     * Calculates the bitwise-or of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left column
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |);

    namespace details {

        /**
         * The adaptor closure for the column view
         */
        class ColumnViewAdaptorClosure {
        public:

            /**
             * Constructs a new column view adaptor closure
             * object
             *
             * @param columnID an index of the viewed column
             */
            constexpr ColumnViewAdaptorClosure(
                std::size_t columnID) noexcept
                    : columnID{columnID} {}

            /**
             * Returns a view to the given matrix
             *
             * @tparam Tp the matrix's type
             * @param range an universal reference to the matrix
             * @return the column view to the range
             */
            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;
        private:
            std::size_t                     columnID;
        };


        /**
         * The adaptor for the column view
         */
        struct ColumnViewAdaptor {

            /**
             * Returns a view to the given matrix
             *
             * @tparam Tp the matrix's type
             * @param range an universal reference to the matrix
             * @param columnID an index of the viewed column
             * @return the column view to the range
             */
            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix,
                std::size_t columnID) const noexcept;

           /**
             * Returns a closure to the given view
             *
             * @param columnID an index of the viewed column
             * @return the view closure
             */
            [[nodiscard]] constexpr ColumnViewAdaptorClosure
                operator() (std::size_t rowID) const noexcept;

        };

        /**
         * The adaptor closure for the column range view
         */
        struct ColumnRangeViewAdaptorClosure {

            /**
             * Constructs a new column range view adaptor
             * closure object
             */
            constexpr ColumnRangeViewAdaptorClosure(
                void) noexcept = default;

            /**
             * Returns a view to the given matrix
             *
             * @tparam Tp the matrix's type
             * @param range an universal reference to the matrix
             * @return the column view to the range
             */
            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;

        };

        /**
         * The adaptor for the column range view
         */
        struct ColumnRangeViewAdaptor {

            /**
             * Returns a view to the given matrix
             *
             * @tparam Tp the matrix's type
             * @param range an universal reference to the matrix
             * @return the column view to the range
             */
            template <MatrixType Tp>
            [[nodiscard]] constexpr auto operator() (
                Tp&& matrix) const noexcept;

           /**
             * Returns a closure to the given view
             *
             * @return the view closure
             */
            [[nodiscard]] constexpr ColumnRangeViewAdaptorClosure
                operator() (void) const noexcept;

        };

        /**
         * Connects the closure with the matrix
         *
         * @tparam Tp the matrix type
         * @tparam Field the field name
         * @param range an universal reference to the
         * matrix object
         * @param closure a constant reference to the
         * closure object
         * @return the view of the matrix
         */
        template <MatrixType Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnViewAdaptorClosure const& closure) noexcept;

        /**
         * Connects the adaptor with the matrix
         *
         * @tparam Tp the matrix type
         * @tparam Field the field name
         * @param range an universal reference to the
         * matrix object
         * @param closure a constant reference to the
         * closure object
         * @return the view of the matrix
         */
        template <MatrixType Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnViewAdaptor const& adaptor) noexcept;

        /**
         * Connects the closure with the matrix
         *
         * @tparam Tp the matrix type
         * @tparam Field the field name
         * @param range an universal reference to the
         * matrix object
         * @param closure a constant reference to the
         * closure object
         * @return the view of the matrix
         */
        template <MatrixType Tp>
        [[nodiscard]] constexpr auto operator | (
            Tp&& matrix,
            ColumnRangeViewAdaptorClosure const& closure) noexcept;

        /**
         * Connects the adaptor with the matrix
         *
         * @tparam Tp the matrix type
         * @tparam Field the field name
         * @param range an universal reference to the
         * matrix object
         * @param closure a constant reference to the
         * closure object
         * @return the view of the matrix
         */
        template <MatrixType Tp>
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

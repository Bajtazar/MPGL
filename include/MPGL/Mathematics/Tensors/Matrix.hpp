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

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Traits/TupleTraits.hpp>
#include <MPGL/Mathematics/Ranges.hpp>

#include <optional>

namespace mpgl {

    /**
     * Represents the mathematical matrix in the memory
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the number of matrix's rows
     * @tparam Cols the number of matrix's columns
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    class Matrix {
    public:
        using value_type = Vector<Tp, Cols>;
        using container = std::array<value_type, Rows>;

        /**
         * Constructs a new matrix object from the given
         * vectors
         *
         * @tparam Rws the types of vectors
         * @param rows the universal references to vectors
         */
        template <AllAbsolutelySame<value_type>... Rws>
            requires (sizeof...(Rws) == Rows)
        constexpr Matrix(Rws&&... rows) noexcept
            : base{ static_cast<value_type>(std::forward<Rws>(rows))... } {}

        /**
         * Constructs a new matrix object
         */
        constexpr Matrix(void) noexcept
            : base{} {}

        /**
         * Inverses the sign of the matrix's elements
         *
         * @return the matrix with sign-inversed elements
         */
        [[nodiscard]] constexpr Matrix operator-(void) const noexcept;

        /**
         * Returns the size of the matrix [the number of its rows]
         *
         * @return the size of the matrix
         */
        [[nodiscard]] static constexpr std::size_t size(void) noexcept
            { return Rows; }

        using iterator = typename container::iterator;
        using const_iterator = typename container::const_iterator;
        using reverse_iterator =
            typename container::reverse_iterator;
        using const_reverse_iterator =
            typename container::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the rows
         *
         * @return the iterator to the begining of the rows
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return base.begin(); }

        /**
         * Returns the iterator to the end of the rows
         *
         * @return the iterator to the end of the rows
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return base.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the rows
         *
         * @return the constant iterator to the begining
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return base.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the rows
         *
         * @return the constant iterator to the end
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return base.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the rows
         *
         * @return the constant iterator to the begining
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return base.cbegin(); }

        /**
         * Returns the constant iterator to the end
         * of the rows
         *
         * @return the constant iterator to the end
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return base.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the rows
         *
         * @return the reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(void) noexcept
            { return base.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the rows
         *
         * @return the reverse iterator to the begining of
         * the rows
         */
        [[nodiscard]] constexpr reverse_iterator rend(void) noexcept
            { return base.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept
                { return base.rbegin(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept
                { return base.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept
                { return base.crbegin(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator crend(
            void) const noexcept
                { return base.crend(); }

        /**
         * Returns the reference to row with the given index
         *
         * @param index the row's index
         * @return the reference to row with the given index
         */
        [[nodiscard]] constexpr value_type& operator[](
            std::size_t index) noexcept
                { return base[index]; }

        /**
         * Returns the constant reference to row with
         * the given index
         *
         * @param index the row's index
         * @return the constant reference to row with
         * the given index
         */
        [[nodiscard]] constexpr value_type const& operator[](
            std::size_t index) const noexcept
                { return base[index]; }

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        template <std::integral Ip>
        [[nodiscard]] constexpr value_type& operator[] (
            Vector2<Ip> const& index) noexcept
                { return base[index[0]][index[1]]; }

        /**
         * Returns the constant reference to element with
         * the given index
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        template <std::integral Ip>
        [[nodiscard]] constexpr value_type const& operator[] (
            Vector2<Ip> const& index) const noexcept
                { return base[index[0]][index[1]]; }

        /**
         * Casts the matrix to the given type
         *
         * @tparam Up the new matrix's element type
         * @return the casted matrix
         */
        template <Arithmetic Up>
        [[nodiscard]] constexpr operator
            Matrix<Up, Rows, Cols>() const noexcept;

        /**
         * Casts the matrix to the given size
         *
         * @tparam URows the new matrix's rows
         * @tparam UCols the new matrix's columns
         * @return the casted matrix
         */
        template <std::size_t URows, std::size_t UCols>
            requires (URows >= Rows && UCols >= Cols &&
                (URows != Rows || UCols != Cols))
        [[nodiscard]] constexpr operator
            Matrix<Tp, URows, UCols>() const noexcept;

        /**
         * Adds the elements of this matrix with
         * the given matrix elements
         *
         * @param right the added matrix
         * @return the reference to this matrix
         */
        constexpr Matrix& operator+=(Matrix const& right);

        /**
         * Subtracts the elements of this matrix with
         * the given matrix elements
         *
         * @param right the subtracted matrix
         * @return the reference to this matrix
         */
        constexpr Matrix& operator-=(Matrix const& right);

        /**
         * Multiplies the elements of this matrix with
         * the given matrix elements
         *
         * @param right the multiplicand matrix
         * @return the reference to this matrix
         */
        constexpr Matrix& operator*=(Matrix const& right);

        /**
         * Divides the elements of this matrix by the
         * given matrix elements
         *
         * @param right the divider matrix
         * @return the reference to this matrix
         */
        constexpr Matrix& operator/=(Matrix const& right);

        /**
         * Calculates the modulo of elements of this matrix
         * and the given matrix elements
         *
         * @param right the matrix with modulos
         * @return the reference to this matrix
         */
        constexpr Matrix& operator%=(
            Matrix const& right) requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * matrix and the given matrix elements
         *
         * @param right the matrix with values
         * @return the reference to this matrix
         */
        constexpr Matrix& operator^=(
            Matrix const& right) requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this matrix
         * and the given matrix elements
         *
         * @param right the matrix with values
         * @return the reference to this matrix
         */
        constexpr Matrix& operator&=(
            Matrix const& right) requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this matrix
         * and the given matrix elements
         *
         * @param right the matrix with values
         * @return the reference to matrix matrix
         */
        constexpr Matrix& operator|=(
            Matrix const& right) requires mpgl_Operable(Tp, |);

        /**
         * Adds the given scalar value to the elements of
         * this matrix
         *
         * @param right the added scalar
         * @return the reference to this matrix
         */
        constexpr Matrix& operator+=(Tp const& right);

        /**
         * Subtracts the given scalar value to the elements of
         * this matrix
         *
         * @param right the subtracted scalar
         * @return the reference to this matrix
         */
        constexpr Matrix& operator-=(Tp const& right);

        /**
         * Multiplies the given scalar value to the elements of
         * this matrix
         *
         * @param right the multiplied scalar
         * @return the reference to this matrix
         */
        constexpr Matrix& operator*=(Tp const& right);

        /**
         * Divides the elements of this matrix by the given
         * scalar value
         *
         * @param right the divisor scalar
         * @return the reference to this matrix
         */
        constexpr Matrix& operator/=(Tp const& right);

        /**
         * Calculates the modulo of elements of this matrix
         * with the given scalar
         *
         * @param right the modulo scalar
         * @return the reference to this matrix
         */
        constexpr Matrix& operator%=(Tp const& right
            ) requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * this and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this this
         */
        constexpr Matrix& operator^=(Tp const& right
            ) requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this
         * column and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this column
         */
        constexpr Matrix& operator&=(Tp const& right
            ) requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this
         * column and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this column
         */
        constexpr Matrix& operator|=(Tp const& right
            ) requires mpgl_Operable(Tp, |);
    private:
        container                           base;
    };

    template <typename MatrixTp>
    class ColumnView : public std::ranges::view_interface<
        ColumnView<Tp, Rows, Cols>>
    {
    public:
        constexpr explicit ColumnView(
            MatrixTp&& matrix,
            std::size_t columnID) noexcept;

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

    private:
        MatrixTp                            matrix;
        std::size_t                         columnID;
    };

    /**
     * Returns the identity matrix of the given size
     *
     * @tparam Tp the type of the matrix element
     * @tparam Rows the number of matrix rows and columns
     * @param diagonal the diagonal values
     * @return the identity matrix
     */
    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Rows> identityMatrix(
        Tp diagonal = {1}) noexcept;

    /**
     * Returns the trasposed matrix
     *
     * @tparam Tp the type of the matrix element
     * @tparam Rows the number of matrix rows
     * @tparam Cols the number of matrix columns
     * @param matrix the matrix to be transposed
     * @return the transposed matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Matrix<Tp, Cols, Rows>
        transpose(Matrix<Tp, Rows, Cols> const& matrix) noexcept;

    /**
     * Multiplies two matrices together
     *
     * @tparam Tp the matrix's element type
     * @tparam LRows the left matrix's rows
     * @tparam LCols the left matrix's columns
     * @tparam RCols the right matrix's columns
     * @param left the constant reference to the left matrix
     * @param right the constant reference to the right matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t LRows,
        std::size_t LCols, std::size_t RCols>
            requires (LRows > 1 && LCols > 1 && RCols > 1)
    [[nodiscard]] Matrix<Tp, LRows, RCols> operator*(
        Matrix<Tp, LRows, LCols> const& left,
        Matrix<Tp, LCols, RCols> const& right);

    /**
     * Adds two matrices together
     *
     * @tparam Tp the matrix's element type
     * @tparam LRows the left matrix's rows
     * @tparam LCols the left matrix's columns
     * @tparam RCols the right matrix's columns
     * @param left the constant reference to the left matrix
     * @param right the constant reference to the right matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator+(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right);

    /**
     * Subtracts two matrices from themselves
     *
     * @tparam Tp the matrix's element type
     * @tparam LRows the left matrix's rows
     * @tparam LCols the left matrix's columns
     * @tparam RCols the right matrix's columns
     * @param left the constant reference to the left matrix
     * @param right the constant reference to the right matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator-(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right);

    /**
     * Multiplies the matrix with the vector
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param matrix the constant reference to the matrix
     * @param vector the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Vector<Tp, Rows> operator*(
        Matrix<Tp, Rows, Cols> const& matrix,
        Vector<Tp, Cols> const& vector);

    /**
     * Adds the scalar value to the matrix
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right);

    /**
     * Subtracts the scalar value from the matrix
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right);

    /**
     * Multiplies the matrix with the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right);

    /**
     * Divides the matrix by the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right);

    /**
     * Calculates the modulo of matrix's elements and the
     * scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of matrix's elements and the
     * scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of matrix's elements and the
     * scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator&(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of matrix's elements and the
     * scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the matrix
     * @param right the constant reference to the scalar
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the matrix
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right);

    /**
     * Subtracts the scalar value from the matrix
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right);

    /**
     * Multiplies the matrix with the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right);

    /**
     * Divides the matrix consisting of scalar value fields
     * by the matrix elements
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right);

    /**
     * Calculates the modulo of the matrix consisting of scalar
     * value fields with the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the matrix elements and
     * the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the matrix elements and
     * the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
         operator&(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the matrix elements and
     * the scalar value
     *
     * @tparam Tp the matrix's element type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the scalar
     * @param right the constant reference to the matrix
     * @return the result matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, |);

    /**
     * Calculates the LUP decompostion of the given matrix
     */
    class LUPDecompositionFn {
    public:
        /**
         * The tag used to determine if the is-even flag
         * should be retured. This flag is used do determine
         * the matrix determinent sign
         */
        static constexpr struct CTag {}         CounterTag{};

        /**
         * Performs the in-place LUP decomposition of the
         * given matrix. Returns an optional with the
         * permutation vector. If the matrix is singular
         * then returns an empty optional
         *
         * @tparam Tp the matrix's type
         * @tparam Rows the matrix's rows and columns size
         * @param matrix the reference to the matrix
         * @return the optional with permutation vector
         */
        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        [[nodiscard]] constexpr std::optional<Vector<Tp, Rows>>
            operator() (
                Matrix<Tp, Rows, Rows>& matrix) const noexcept;

        /**
         * Performs the in-place LUP decomposition of the
         * given matrix. Returns an optional with the
         * is-even flag. If the matrix is singular
         * then returns an empty optional
         *
         * @tparam Tp the matrix's type
         * @tparam Rows the matrix's rows and columns size
         * @param matrix the reference to the matrix
         * @return the optional with is-even flag
         */
        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        [[nodiscard]] constexpr std::optional<bool>
            operator() (
                Matrix<Tp, Rows, Rows>& matrix, CTag) const noexcept;
    private:
        /**
         * Finds the pivot
         *
         * @tparam Tp the matrix's type
         * @tparam Rows the matrix's rows and columns size
         * @param u the reference to the u parameter
         * @param k the reference to the k parameter
         * @param matrix the constant reference to the matrix
         * @return the pivot
         */
        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr bool findPivot(
            std::size_t& u,
            std::size_t& k,
            Matrix<Tp, Rows, Rows> const& matrix) const noexcept;

        /**
         * Permutes the matrix
         *
         * @tparam Tp the matrix's type
         * @tparam Rows the matrix's rows and columns size
         * @param k the reference to the k parameter
         * @param matrix the reference to the matrix
         */
        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr void permute(
            std::size_t& k,
            Matrix<Tp, Rows, Rows>& matrix) const noexcept;
    };

    inline constexpr LUPDecompositionFn         lupDecomposition;

    /**
     * Solves the linear equation system using the LUP method
     *
     * @tparam Tp the matrix's type
     * @tparam Rows the matrix's rows and columns size
     * @tparam Permutations the permutation vector type
     * @tparam Results the result vector type
     * @param luMatrix the constant reference to the LU matrix
     * @param permutations the contant reference to the permutations
     * vector
     * @param results the constant reference to the results vector
     * @return the equations solution
     */
    template <Arithmetic Tp, std::size_t Rows,
        SizedRange<Tp, Rows> Permutations,
        SizedRange<Tp, Rows> Results>
        requires (Rows > 1)
    [[nodiscard]] constexpr Vector<Tp, Rows>
        lupSolve(
            Matrix<Tp, Rows, Rows> const& luMatrix,
            Permutations const& permutations,
            Results const& results) noexcept;

    /**
     * Returns an optional with the inverted matrix. Returns
     * an empty optional if the matrix is singular
     *
     * @tparam Tp the matrix's type
     * @tparam Rows the matrix's rows and columns size
     * @tparam Up the invertex matrix's type
     * @param matrix the constant reference to the matrix
     * @return the invertex matrix
     */
    template <Arithmetic Tp, std::size_t Rows,
        Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Matrix<Up, Rows, Rows>>
        invert(Matrix<Tp, Rows, Rows> const& matrix) noexcept;

    /**
     * Returns the matrix's determinent
     *
     * @tparam Tp the matrix's type
     * @tparam Rows the matrix's rows and columns size
     * @tparam Up the determinent type
     * @param matrix the constant reference to the matrix
     * @return the matrix's determinent
     */
    template <Arithmetic Tp, std::size_t Rows, Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr Up det(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept;

    /**
     * Returns the matrix's trace
     *
     * @tparam Tp the matrix's type
     * @tparam Rows the matrix's rows and columns size
     * @param matrix the constant reference to the matrix
     * @return the matrix's trace
     */
    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr Tp trace(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept;

    template <Arithmetic Tp, std::size_t Size>
    using SquareMatrix                    = Matrix<Tp, Size, Size>;

    template <typename Tp>
    using Matrix2                         = Matrix<Tp, 2, 2>;
    template <typename Tp>
    using Matrix3                         = Matrix<Tp, 3, 3>;
    template <typename Tp>
    using Matrix4                         = Matrix<Tp, 4, 4>;
    template <typename Tp>
    using Matrix8                         = Matrix<Tp, 8, 8>;

    template <std::size_t Rows, std::size_t Cols>
    using MatrixC                         = Matrix<
        std::complex<float64>, Rows, Cols>;

    template class Matrix<float32, 2, 2>;
    template class Matrix<float32, 3, 3>;
    template class Matrix<float32, 4, 4>;

    typedef Matrix<float32, 2, 2>           Matrix2f;
    typedef Matrix<int32, 2, 2>             Matrix2i;
    typedef Matrix<float64, 2, 2>           Matrix2d;
    typedef Matrix<uint32, 2, 2>            Matrix2u;
    typedef MatrixC<2, 2>                   Matrix2c;

    typedef Matrix<float32, 3, 3>           Matrix3f;
    typedef Matrix<int32, 3, 3>             Matrix3i;
    typedef Matrix<float64, 3, 3>           Matrix3d;
    typedef Matrix<uint32, 3, 3>            Matrix3u;
    typedef MatrixC<3, 3>                   Matrix3c;

    typedef Matrix<float32, 4, 4>           Matrix4f;
    typedef Matrix<int32, 4, 4>             Matrix4i;
    typedef Matrix<uint32, 4, 4>            Matrix4u;
    typedef Matrix<float64, 4, 4>           Matrix4d;
    typedef MatrixC<4, 4>                   Matrix4c;

}

#include <MPGL/Mathematics/Tensors/Matrix.tpp>

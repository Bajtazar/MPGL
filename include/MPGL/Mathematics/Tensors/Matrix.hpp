/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
        using transposed_value_type = Vector<Tp, Rows>;
        using element_type = Tp;
        using container = std::array<value_type, Rows>;

        /**
         * Constructs a new matrix object from the given
         * row vectors
         *
         * @tparam Rws the types of vectors
         * @param rows the universal references to vectors
         */
        template <AllAbsolutelySame<value_type>... Rws>
            requires (sizeof...(Rws) == Rows)
        constexpr Matrix(Rws&&... rows) noexcept;

        /**
         * Constructs a new matrix object from the given
         * column vectors
         *
         * @tparam Cls the types of vectors
         * @param tag the tag that indicates that vectors are
         * being used as a columns
         * @param columns the universal references to vectors
         */
        template <AllAbsolutelySame<transposed_value_type>... Cls>
            requires (sizeof...(Cls) == Cols)
        constexpr Matrix(
            [[maybe_unused]] TransposedTag tag,
            Cls&&... columns) noexcept;

        /**
         * Constructs a new matrix object
         */
        constexpr Matrix(void) noexcept = default;

        /**
         * Constructs a new matrix object. Copies the span
         * elements into this matrix object
         *
         * @param span a constant reference to the span object
         */
        explicit constexpr Matrix(
            std::span<value_type const, Rows> const& span) noexcept;

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
        [[nodiscard]] static constexpr std::size_t
            size(void) noexcept
                { return Rows; }

        /**
         * Returns the height of the matrix
         * [the number of its rows]
         *
         * @return the height of the matrix
         */
        [[nodiscard]] static constexpr std::size_t
            height(void) noexcept
                { return Rows; }

        /**
         * Returns the width of the matrix
         * [the number of its columns]
         *
         * @return the width of the matrix
         */
        [[nodiscard]] static constexpr std::size_t
            width(void) noexcept
                { return Cols; }

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
        [[nodiscard]] constexpr element_type& operator[] (
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
        [[nodiscard]] constexpr element_type const& operator[] (
            Vector2<Ip> const& index) const noexcept
                { return base[index[0]][index[1]]; }

        /**
         * Returns a std::span that holds view to this matrix
         *
         * @return the std::span that holds view to this matrix
         */
        [[nodiscard]] constexpr operator std::span<
            value_type const, Rows>() const noexcept;

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
        container                           base = {};
    };

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

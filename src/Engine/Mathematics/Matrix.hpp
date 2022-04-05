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

#include "Vector.hpp"
#include "Ranges.hpp"

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
        #pragma pack(push, 1)
        /**
         * Represents the column of the matrix. Allows to perfrom
         * vector-like operations on entire columns even if
         * they are not represented continuosly in the memory.
         *
         * Does not behave like the covariant vector
         */
        class Column {
        public:
            constexpr explicit Column(void) noexcept = delete;
            constexpr Column(Column const& column) noexcept = delete;

            /**
             * Constructs a new column object from the given
             * rvalue reference to the column object
             *
             * @param column the rvalue reference to the
             * column object
             */
            constexpr Column(Column&& column) noexcept
                : seed{std::move(column.seed)} {}

            constexpr Column& operator=(
                Column const& column) noexcept = delete;

            /**
             * Assigns the given rvalue reference to the column
             * object to this object
             *
             * @param column the rvalue reference to column
             * object
             * @return the reference to this object
             */
            constexpr Column& operator=(Column&& column) noexcept
                    { seed = std::move(column.seed); return *this; }

            /**
             * Assigns the given vector to this object
             *
             * @param vector the constant reference to vector
             * object
             * @return the reference to this object
             */
            constexpr Column& operator=(
                Vector<Tp, Rows> const& vector) noexcept;

            /**
             * Inverses the sign of the columns's elements
             *
             * @return the vector with reversed elements
             */
            [[nodiscard]] constexpr Vector<Tp, Rows>
                operator-(void) const noexcept;

            /**
             * Casts this column object to the vector
             *
             * @return the vector with column data
             */
            [[nodiscard]] constexpr operator Vector<Tp, Rows>(
                ) const noexcept;

            /**
             * Returns the size of the column
             *
             * @return the size of the column
             */
            [[nodiscard]] static constexpr std::size_t size(
                void) noexcept
                    { return Rows; }

            /**
             * Returns the length of the column [equivalent
             * to the square norm in the euclidean space]
             *
             * @tparam Up the result type
             * @param init the initial value
             * @return the length of the column
             */
            template <Arithmetic Up = Tp>
            [[nodiscard]] constexpr Up length(
                Up init = {0}) const noexcept;

            /**
             * Adds the elements of this column with
             * the given column elements
             *
             * @param right the added column
             * @return the reference to this column
             */
            constexpr Column& operator+=(Column const& right);

            /**
             * Subtracts the elements of this column with
             * the given column elements
             *
             * @param right the subtracted column
             * @return the reference to this column
             */
            constexpr Column& operator-=(Column const& right);

            /**
             * Multiplies the elements of this column with
             * the given column elements
             *
             * @param right the multiplicand column
             * @return the reference to this column
             */
            constexpr Column& operator*=(Column const& right);

            /**
             * Divides the elements of this column by the
             * given column elements
             *
             * @param right the divider column
             * @return the reference to this column
             */
            constexpr Column& operator/=(Column const& right);

            /**
             * Calculates the modulo of elements of this column
             * and the given column elements
             *
             * @param right the column with modulos
             * @return the reference to this column
             */
            constexpr Column& operator%=(Column const& right)
                requires mpgl_Operable(Tp, %);

            /**
             * Calculates the bitwise-xor of elements of this
             * column and the given column elements
             *
             * @param right the column with values
             * @return the reference to this column
             */
            constexpr Column& operator^=(Column const& right)
                requires mpgl_Operable(Tp, ^);

            /**
             * Calculates the bitwise-and of elements of this column
             * and the given column elements
             *
             * @param right the column with values
             * @return the reference to this column
             */
            constexpr Column& operator&=(Column const& right)
                requires mpgl_Operable(Tp, &);

            /**
             * Calculates the bitwise-or of elements of this column
             * and the given column elements
             *
             * @param right the column with values
             * @return the reference to this column
             */
            constexpr Column& operator|=(Column const& right)
                requires mpgl_Operable(Tp, |);

            /**
             * Adds the given scalar value to the elements of
             * this column
             *
             * @param right the added scalar
             * @return the reference to this column
             */
            constexpr Column& operator+=(Tp const& right);

            /**
             * Subtracts the given scalar value to the elements of
             * this column
             *
             * @param right the subtracted scalar
             * @return the reference to this column
             */
            constexpr Column& operator-=(Tp const& right);

            /**
             * Multiplies the given scalar value to the elements of
             * this column
             *
             * @param right the multiplied scalar
             * @return the reference to this column
             */
            constexpr Column& operator*=(Tp const& right);

            /**
             * Divides the elements of this column by the given
             * scalar value
             *
             * @param right the divisor scalar
             * @return the reference to this column
             */
            constexpr Column& operator/=(Tp const& right);

            /**
             * Calculates the modulo of elements of this column
             * with the given scalar
             *
             * @param right the modulo scalar
             * @return the reference to this column
             */
            constexpr Column& operator%=(Tp const& right)
                requires mpgl_Operable(Tp, %);

            /**
             * Calculates the bitwise-xor of elements of this
             * column and the given scalar value
             *
             * @param right the scalar value
             * @return the reference to this column
             */
            constexpr Column& operator^=(Tp const& right)
                requires mpgl_Operable(Tp, ^);

            /**
             * Calculates the bitwise-and of elements of this
             * column and the given scalar value
             *
             * @param right the scalar value
             * @return the reference to this column
             */
            constexpr Column& operator&=(Tp const& right)
                requires mpgl_Operable(Tp, &);

            /**
             * Calculates the bitwise-or of elements of this
             * column and the given scalar value
             *
             * @param right the scalar value
             * @return the reference to this column
             */
            constexpr Column& operator|=(Tp const& right)
                requires mpgl_Operable(Tp, |);

            /**
             * Iterares over the elements of the column
             *
             * @tparam value_type the iterator's value type
             */
            template <Arithmetic value_type>
            class Iterator : public std::iterator<
                std::random_access_iterator_tag, value_type>
            {
            public:
                using iterator_category =       std::random_access_iterator_tag;
                using reference =               value_type&;
                using pointer =                 value_type*;
                using const_pointer =           pointer const;
                using difference_type =         decltype(
                    std::declval<pointer>() - std::declval<pointer>());
                using iter =                    Iterator;
                using iter_ref =                Iterator&;
                using iter_cref =               Iterator const&;
                using compare =
                    std::compare_three_way_result_t<pointer, pointer>;

                /**
                 * Constructs a new iterator object from the
                 * given pointer
                 *
                 * @param ptr the pointer to the data
                 */
                constexpr explicit Iterator(pointer ptr) noexcept
                    : ptr{ptr} {}

                /**
                 * Constructs a new iterator object
                 */
                constexpr explicit Iterator(void) noexcept = default;

                /**
                 * Increments iterator by one
                 *
                 * @return reference to this object
                 */
                constexpr iter_ref operator++(void) noexcept
                    { ptr += Cols; return *this; }

                /**
                 * Post-increments iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] constexpr iter operator++(int) noexcept
                    { auto temp = *this; ++(*this); return temp; }

                /**
                 * Decrements iterator by one
                 *
                 * @return reference to this object
                 */
                constexpr iter_ref operator--(void) noexcept
                    { ptr -= Cols; return *this; }

                /**
                 * Post-decrements iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] constexpr iter operator--(int) noexcept
                    { auto temp = *this; --(*this); return temp; }

                /**
                 * Returns the reference to the column's element
                 *
                 * @return the reference to the column's element
                 */
                [[nodiscard]] constexpr reference operator*(
                    void) const noexcept
                        { return *ptr; }

                /**
                 * Returns the pointer to the column's element
                 *
                 * @return the pointer to the column's element
                 */
                [[nodiscard]] constexpr const_pointer operator->(
                    void) const noexcept
                        { return ptr; }

                /**
                 * Increments iterator by the given distance
                 *
                 * @param offset the incremented distance
                 * @return reference to this object
                 */
                constexpr iter_ref operator+=(
                    difference_type offset) noexcept
                        { ptr += offset * Cols; return *this; }

                /**
                 * Decrements iterator by the given distance
                 *
                 * @param offset the decremented distance
                 * @return reference to this object
                 */
                constexpr iter_ref operator-=(
                    difference_type offset) noexcept
                        { ptr -= offset * Cols; return *this; }

                /**
                 * Returns a view shifted by the given offset
                 *
                 * @param offset the incremented distance
                 * @return the view shifted by the given offse
                 */
                [[nodiscard]] constexpr reference operator[](
                    difference_type offset) const noexcept
                        { return *(ptr + offset * Cols); }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted iterator
                 */
                [[nodiscard]] friend constexpr iter operator+(
                    iter_cref left,
                    difference_type right) noexcept
                { auto temp = left; temp.ptr += right; return temp; }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the distance
                 * @param right the iterator
                 * @return the shifted iterator
                 */
                [[nodiscard]] friend constexpr iter operator+(
                    difference_type left,
                    iter_cref right) noexcept
                { auto temp = right; temp.ptr += left; return temp; }

                /**
                 * Subtracts given distance from iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted operator
                 */
                [[nodiscard]] friend constexpr iter operator-(
                    iter_cref left,
                    difference_type right) noexcept
                { auto temp = left; temp.ptr -= right; return temp; }

                /**
                 * Returns distance between iterators
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return difference_type
                 */
                [[nodiscard]] friend constexpr difference_type
                    operator-(
                        iter_cref left,
                        iter_cref right) noexcept
                            { return left.ptr - right.ptr; }

                /**
                 * Checks whether two iterators are equal
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return whether two iterators are equal
                 */
                [[nodiscard]] friend constexpr bool operator==(
                    iter_cref left,
                    iter_cref right) noexcept
                        { return left.ptr == right.ptr; }

                /**
                 * Compares two iterators to each other
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return the result of compare
                 */
                [[nodiscard]] friend constexpr compare operator<=>(
                    iter_cref left,
                    iter_cref right) noexcept
                        { return left.ptr <=> right.ptr; }
            private:
                pointer                         ptr;
            };

            typedef Iterator<Tp>                iterator;
            typedef Iterator<Tp const>          const_iterator;
            typedef std::reverse_iterator<
                iterator>                       reverse_iterator;
            typedef std::reverse_iterator<
                const_iterator>                 const_reverse_iterator;

            /**
             * Returns the iterator to the begining of the column
             *
             * @return the iterator to the begining of the column
             */
            [[nodiscard]] constexpr iterator begin(void) noexcept
                { return iterator{ &seed }; }

            /**
             * Returns the iterator to the end of the column
             *
             * @return the iterator to the end of the column
             */
            [[nodiscard]] constexpr iterator end(void) noexcept
                { return iterator{ &seed + Rows * Cols }; }

            /**
             * Returns the constant iterator to the begining
             * of the column
             *
             * @return the constant iterator to the begining
             * of the column
             */
            [[nodiscard]] constexpr const_iterator begin(
                void) const noexcept
                    { return const_iterator{ &seed }; }

            /**
             * Returns the constant iterator to the end
             * of the column
             *
             * @return the constant iterator to the end
             * of the column
             */
            [[nodiscard]] constexpr const_iterator end(
                void) const noexcept
                    { return const_iterator{ &seed + Rows * Cols }; }

            /**
             * Returns the constant iterator to the begining
             * of the column
             *
             * @return the constant iterator to the begining
             * of the column
             */
            [[nodiscard]] constexpr const_iterator cbegin(
                void) const noexcept
                    { return const_iterator{ &seed }; }

            /**
             * Returns the constant iterator to the end
             * of the column
             *
             * @return the constant iterator to the end
             * of the column
             */
            [[nodiscard]] constexpr const_iterator cend(
                void) const noexcept
                    { return const_iterator{ &seed + Rows * Cols }; }

            /**
             * Returns the reverse iterator to the end of
             * the column
             *
             * @return the reverse iterator to the end of
             * the column
             */
            [[nodiscard]] constexpr reverse_iterator rbegin(
                void) noexcept
                    { return reverse_iterator{ end() - 1 }; }

            /**
             * Returns the reverse iterator to the begining of
             * the column
             *
             * @return the reverse iterator to the begining of
             * the column
             */
            [[nodiscard]] constexpr reverse_iterator rend(
                void) noexcept
                    { return reverse_iterator{ begin() - 1}; }

            /**
             * Returns the constant reverse iterator to the end of
             * the column
             *
             * @return the constant reverse iterator to the end of
             * the column
             */
            [[nodiscard]] constexpr const_reverse_iterator rbegin(
                void) const noexcept
                    { return const_reverse_iterator{ end() - 1 }; }

            /**
             * Returns the constant reverse iterator to the
             * begining of the column
             *
             * @return the constant reverse iterator to the
             * begining of the column
             */
            [[nodiscard]] constexpr const_reverse_iterator rend(
                void) const noexcept
                    { return const_reverse_iterator{ begin() - 1}; }

            /**
             * Returns the constant reverse iterator to the end of
             * the column
             *
             * @return the constant reverse iterator to the end of
             * the column
             */
            [[nodiscard]] constexpr const_reverse_iterator crbegin(
                void) const noexcept
                    { return const_reverse_iterator{ cend() - 1 }; }

            /**
             * Returns the constant reverse iterator to the
             * begining of the column
             *
             * @return the constant reverse iterator to the
             * begining of the column
             */
            [[nodiscard]] constexpr const_reverse_iterator crend(
                void) const noexcept
                    { return const_reverse_iterator{ cbegin() - 1}; }

            /**
             * Returns the reference to element with the given index
             *
             * @param index the element's index
             * @return the reference to element with the given index
             */
            [[nodiscard]] constexpr Tp& operator[] (
                std::size_t index) noexcept
                    { return *(&seed + index * Cols); }

            /**
             * Returns the constant reference to element with
             * the given index
             *
             * @param index the element's index
             * @return the constant reference to element with
             * the given index
             */
            [[nodiscard]] constexpr Tp const& operator[] (
                std::size_t index) const noexcept
                    { return *(&seed + index * Cols); }

            /**
             * Destroys the column object
             */
            constexpr ~Column(void) noexcept = default;
        private:
            Tp                                      seed;
        };
        #pragma pack(pop)
    private:
        typedef TensorTuple<Column, Rows, Cols> ColumnBase;
    public:
        typedef TensorTuple<Tp, Rows, Cols>     NormalBase;
        typedef Vector<Tp, Cols>                value_type;
        typedef Column                          column_value_type;

        /**
         * Constructs a new matrix object from the given
         * vectors
         *
         * @tparam Rws the types of vectors
         */
        template <AllAbsolutelySame<value_type>... Rws>
            requires (sizeof...(Rws) == Rows)
        constexpr Matrix(Rws&&... rows) noexcept;

        /**
         * Constructs a new matrix object from the given
         * base tuple
         *
         * @param base the constant reference to the base
         * object
         */
        constexpr Matrix(NormalBase const& base) noexcept
            : normalBase{base} {}

        /**
         * Constructs a new matrix object from the given
         * base tuple
         *
         * @param base the rvalue reference to the base
         * object
         */
        constexpr Matrix(NormalBase&& base) noexcept
            : normalBase{std::move(base)} {}

        /**
         * Constructs a new matrix object from the given
         * matrix object
         *
         * @param matrix the constant reference to the matrix
         * object
         */
        constexpr Matrix(Matrix const& matrix) noexcept
            : normalBase{matrix.normalBase} {}

        /**
         * Constructs a new matrix object from the given
         * matrix object
         *
         * @param matrix the rvalue reference to the matrix
         * object
         */
        constexpr Matrix(Matrix&& matrix) noexcept
            : normalBase{std::move(matrix.normalBase)} {}

        /**
         * Constructs a new matrix object
         */
        constexpr Matrix(void) noexcept
            : normalBase{} {}

        /**
         * Assigns the given matrix object to this object
         *
         * @param matrix the constant reference to matrix object
         * @return the reference to this object
         */
        constexpr Matrix& operator=(
            Matrix const& matrix) noexcept
                { normalBase = matrix.normalBase; return *this; }

        /**
         * Assigns the given matrix object to this object
         *
         * @param matrix the constant reference to matrix object
         * @return the reference to this object
         */
        constexpr Matrix& operator=(Matrix&& matrix) noexcept;

        /**
         * Inverses the sign of the matrix's elements
         *
         * @return the matrix with reversed elements
         */
        [[nodiscard]] constexpr Matrix operator-(void) const noexcept;

        /**
         * Returns the size of the matrix [the number of its rows]
         *
         * @return the size of the matrix
         */
        [[nodiscard]] static constexpr std::size_t size(void) noexcept
            { return Rows; }

        /**
         * Iterates through the matrix's rows
         *
         * @tparam value_type the iterator's value type
         */
        template <class value_type>
        class Iterator : public std::iterator<
            std::contiguous_iterator_tag, value_type>
        {
        public:
            using pointer                     = value_type*;
            using reference                   = value_type&;
            using difference_type             = std::ptrdiff_t;
            using iterator_category           = std::contiguous_iterator_tag;
            using iter                        = Iterator;
            using iter_ref                    = Iterator&;
            using iter_cref                   = Iterator const&;
            using compare
                = std::compare_three_way_result_t<pointer, pointer>;

            /**
             * Constructs a new iterator object from the
             * given pointer
             *
             * @param ptr the pointer to the data
             */
            constexpr explicit Iterator(pointer ptr) noexcept
                : ptr{ptr} {}

            /**
             * Constructs a new iterator object
             */
            constexpr explicit Iterator(void) noexcept = default;

            /**
             * Increments iterator by one
             *
             * @return reference to this object
             */
            constexpr iter_ref operator++(void) noexcept
                { ++ptr; return *this; }

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr iter operator++(int) noexcept
                { auto temp = *this; ++ptr; return temp; }

            /**
             * Decrements iterator by one
             *
             * @return reference to this object
             */
            constexpr iter_ref operator--(void) noexcept
                { --ptr; return *this; }

            /**
             * Post-decrements iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr iter operator--(int) noexcept
                { auto temp = *this; --ptr; return temp; }

            /**
             * Returns the reference to the matrix's row
             *
             * @return the reference to the matrix's row
             */
            [[nodiscard]] constexpr reference operator*(
                void) const noexcept
                    { return *ptr; }

            /**
             * Returns the pointer to the column's element
             *
             * @return the pointer to the column's element
             */
            [[nodiscard]] constexpr pointer operator->(
                void) const noexcept
                    { return ptr; }

            /**
             * Increments iterator by the given distance
             *
             * @param offset the incremented distance
             * @return reference to this object
             */
            constexpr iter_ref operator+=(
                difference_type offset) noexcept
                    { ptr += offset; return *this; }

            /**
             * Decrements iterator by the given distance
             *
             * @param offset the decremented distance
             * @return reference to this object
             */
            constexpr iter_ref operator-=(
                difference_type offset) noexcept
                    { ptr -= offset; return *this; }

            /**
             * Returns a view shifted by the given offset
             *
             * @param offset the incremented distance
             * @return the view shifted by the given offse
             */
            [[nodiscard]] constexpr reference operator[] (
                difference_type offset) const noexcept
                    { return *(ptr + offset); }

            /**
             * Adds given distance to an iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr iter operator+(
                iter_cref left,
                difference_type right) noexcept
            { auto temp = left; temp.ptr += right; return temp; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the distance
             * @param right the iterator
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr iter operator+(
                difference_type left,
                iter_cref right) noexcept
            { auto temp = right; temp.ptr += left; return temp; }

            /**
             * Subtracts given distance from iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted operator
             */
            [[nodiscard]] friend constexpr iter operator-(
                iter_cref left,
                difference_type right) noexcept
            { auto temp = left; temp.ptr -= right; return temp; }

            /**
             * Returns distance between iterators
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return difference_type
             */
            [[nodiscard]] friend constexpr difference_type operator-(
                iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr - right.ptr; }

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
            [[nodiscard]] friend constexpr bool operator==(
                iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr == right.ptr; }

            /**
             * Compares two iterators to each other
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return the result of compare
             */
            [[nodiscard]] friend constexpr compare operator<=>(
                iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr <=> right.ptr; }
        private:
            pointer                             ptr;
        };

        typedef Iterator<value_type>        iterator;
        typedef Iterator<value_type const>  const_iterator;
        typedef std::reverse_iterator<
            iterator>                       reverse_iterator;
        typedef std::reverse_iterator<
            const_iterator>                 const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the rows
         *
         * @return the iterator to the begining of the rows
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return iterator{ &first() }; }

        /**
         * Returns the iterator to the end of the rows
         *
         * @return the iterator to the end of the rows
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return iterator{ &first() + Rows }; }

        /**
         * Returns the constant iterator to the begining
         * of the rows
         *
         * @return the constant iterator to the begining
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return const_iterator{ &first() }; }

        /**
         * Returns the constant iterator to the end
         * of the rows
         *
         * @return the constant iterator to the end
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return const_iterator{ &first() + Rows }; }

        /**
         * Returns the constant iterator to the begining
         * of the rows
         *
         * @return the constant iterator to the begining
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return const_iterator{ &first() }; }

        /**
         * Returns the constant iterator to the end
         * of the rows
         *
         * @return the constant iterator to the end
         * of the rows
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return const_iterator{ &first() + Rows }; }

        /**
         * Returns the reverse iterator to the end of
         * the rows
         *
         * @return the reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{ end() - 1 }; }

        /**
         * Returns the reverse iterator to the begining of
         * the rows
         *
         * @return the reverse iterator to the begining of
         * the rows
         */
        [[nodiscard]] constexpr reverse_iterator rend(void) noexcept
            { return reverse_iterator{ begin() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept
                { return const_reverse_iterator{ end() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept
                { return const_reverse_iterator{ begin() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept
                { return const_reverse_iterator{ end() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the rows
         *
         * @return the constant reverse iterator to the end of
         * the rows
         */
        [[nodiscard]] constexpr const_reverse_iterator crend(
            void) const noexcept
                { return const_reverse_iterator{ begin() - 1 }; }

        /**
         * Returns the reference to row with the given index
         *
         * @param index the row's index
         * @return the reference to row with the given index
         */
        [[nodiscard]] constexpr value_type& operator[](
            std::size_t index) noexcept
                { return *(&first() + index); }

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
                { return *(&first() + index); }

        typedef Iterator<column_value_type> column_iterator;
        typedef Iterator<
            column_value_type const>        const_column_iterator;
        typedef std::reverse_iterator<
            column_iterator>                column_reverse_iterator;
        typedef std::reverse_iterator<
            const_column_iterator>          const_column_reverse_iterator;

        /**
         * Returns the iterator to the begining of the columns
         *
         * @return the iterator to the begining of the columns
         */
        [[nodiscard]] constexpr column_iterator columnBegin(
            void) noexcept
                { return column_iterator{ &firstCol() }; }

        /**
         * Returns the iterator to the end of the columns
         *
         * @return the iterator to the end of the columns
         */
        [[nodiscard]] constexpr column_iterator columnEnd(
            void) noexcept
                { return column_iterator{ &firstCol() + Cols }; }

        /**
         * Returns the constant iterator to the begining of
         * the columns
         *
         * @return the constant iterator to the begining of
         * the columns
         */
        [[nodiscard]] constexpr const_column_iterator columnBegin(
            void) const noexcept
                { return const_column_iterator{ &firstCol() }; }

        /**
         * Returns the constant iterator to the end of the columns
         *
         * @return the constant iterator to the end of the columns
         */
        [[nodiscard]] constexpr const_column_iterator columnEnd(
            void) const noexcept
                { return const_column_iterator{ &firstCol() + Cols }; }

        /**
         * Returns the constant iterator to the begining of
         * the columns
         *
         * @return the constant iterator to the begining of
         * the columns
         */
        [[nodiscard]] constexpr const_column_iterator constCBegin(
            void) const noexcept
                { return const_column_iterator{ &firstCol() }; }

        /**
         * Returns the constant iterator to the end of the columns
         *
         * @return the constant iterator to the end of the columns
         */
        [[nodiscard]] constexpr const_column_iterator constCEnd(
            void) const noexcept
                { return const_column_iterator{ &firstCol() + Cols }; }

        /**
         * Returns the reverse iterator to the end of
         * the columns
         *
         * @return the reverse iterator to the end of
         * the columns
         */
        [[nodiscard]] constexpr column_reverse_iterator columnRBegin(
            void) noexcept
                { return column_reverse_iterator{ columnEnd() - 1 }; }

        /**
         * Returns the reverse iterator to the begining of
         * the columns
         *
         * @return the reverse iterator to the begining of
         * the columns
         */
        [[nodiscard]] constexpr column_reverse_iterator columnREnd(
            void) noexcept
                { return column_reverse_iterator{ columnBegin() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the columns
         *
         * @return the constant reverse iterator to the end of
         * the columns
         */
        [[nodiscard]] constexpr const_column_reverse_iterator
            columnRBegin(void) const noexcept
        { return const_column_reverse_iterator{ columnEnd() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the columns
         *
         * @return the constant reverse iterator to the end of
         * the columns
         */
        [[nodiscard]] constexpr const_column_reverse_iterator
            columnREnd(void) const noexcept
        { return const_column_reverse_iterator{ columnBegin() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the columns
         *
         * @return the constant reverse iterator to the end of
         * the columns
         */
        [[nodiscard]] constexpr const_column_reverse_iterator
            columnCRBegin(void) const noexcept
        { return const_column_reverse_iterator{ columnEnd() - 1 }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the columns
         *
         * @return the constant reverse iterator to the end of
         * the columns
         */
        [[nodiscard]] constexpr const_column_reverse_iterator
            columnCREnd(void) const noexcept
        { return const_column_reverse_iterator{ columnBegin() - 1 }; }

        /**
         * Returns the reference to column with the given index
         *
         * @param index the column's index
         * @return the reference to column with the given index
         */
        [[nodiscard]] constexpr column_value_type& getColumn(
            std::size_t index) noexcept
                { return *(&firstCol() + index); }

        /**
         * Returns the constant reference to column with
         * the given index
         *
         * @param index the column's index
         * @return the constant reference to column with
         * the given index
         */
        [[nodiscard]] constexpr column_value_type const& getColumn(
            std::size_t index) const noexcept
                { return *(&firstCol() + index); }

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        template <std::integral Ip>
        [[nodiscard]] constexpr value_type& operator[] (
            Vector2<Ip> const& index) noexcept
                { return (*this)[index[0]][index[1]]; }

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
                { return (*this)[index[0]][index[1]]; }

        /**
         * Returns the range with the columns
         *
         * @return the range with the columns
         */
        [[nodiscard]] constexpr auto columnsRange(void) noexcept;

        /**
         * Returns the constant range with the columns
         *
         * @return the constant range with the columns
         */
        [[nodiscard]] constexpr auto columnsRange(
            void) const noexcept;

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
        /**
         * Builds the tuple used by the matrix
         *
         * @tparam Args the arguments types
         * @param args the arguments
         * @return the base tuple
         */
        template <typename... Args>
        constexpr NormalBase tupleBuilder(
            Args&&... args) const noexcept;

        /**
         * Returns the reference to the first element
         *
         * @return the reference to the first element
         */
        constexpr value_type& first(void) noexcept;

        /**
         * Returns the constant reference to the first element
         *
         * @return the constant reference to the first element
         */
        constexpr value_type const& first(void) const noexcept;

        /**
         * Returns the reference to the first element
         *
         * @return the reference to the first element
         */
        constexpr column_value_type& firstCol(void) noexcept;

        /**
         * Returns the constant reference to the first element
         *
         * @return the constant reference to the first element
         */
        constexpr column_value_type const& firstCol(
            void) const noexcept;

        union {
            NormalBase                      normalBase;
            ColumnBase                      columnBase;
        };
    };

    /**
     * Returns the identity matrix of the given size
     *
     * @tparam Tp the type of the matrix element
     * @tparam Rows the number of matrix rows
     * @tparam Cols the number of matrix columns
     * @param diagonal the diagonal values
     * @return the identity matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows == Cols && Rows > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols> identityMatrix(
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

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        Matrix<Tp, Rows, Cols>::operator-(void) const noexcept
    {
        Matrix<Tp, Cols, Rows> result;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                result[i][j] = -(*this)[i][j];
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator=(
            Vector<Tp, Rows> const& vec) noexcept
    {
        std::ranges::copy(vec, begin());
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::Column::operator
        Vector<Tp, Rows>() const noexcept
    {
        Vector<Tp, Rows> vector;
        std::ranges::copy(*this, vector.begin());
        return vector;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <AllAbsolutelySame<typename
        Matrix<Tp, Rows, Cols>::value_type>... Args>
            requires (sizeof...(Args) == Rows)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(Args&&... args) noexcept
        : normalBase{tupleBuilder(std::forward<Args>(args)...)}
    {
        if constexpr (!std::is_constant_evaluated())
            reverse(begin(), end());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <typename... Args>
    constexpr Matrix<Tp, Rows, Cols>::NormalBase
        Matrix<Tp, Rows, Cols>::tupleBuilder(Args&&... args) const noexcept
    {
        if constexpr (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type&
        Matrix<Tp, Rows, Cols>::first(void)  noexcept
    {
        return static_cast<value_type&>(
            std::get<Rows - 1>(
                static_cast<NormalBase&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type const&
        Matrix<Tp, Rows, Cols>::first(void) const noexcept
    {
        return static_cast<value_type const&>(
            std::get<Rows - 1>(
                static_cast<NormalBase const&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type&
        Matrix<Tp, Rows, Cols>::firstCol(void)  noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type const&
        Matrix<Tp, Rows, Cols>::firstCol(void) const noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase const&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic Up>
    [[nodiscard]] constexpr Up Matrix<Tp, Rows, Cols>::Column::length(
        Up init) const noexcept
    {
        return accumulate(*this, std::move(init),
            [](auto const& value){ return value * value; });
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator+=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left + right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator-=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left - right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator*=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left * right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator/=(
            Column const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left / right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator%=(
            Column const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left % right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator^=(
            Column const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left ^ right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
         Matrix<Tp, Rows, Cols>::Column::operator&=(
            Column const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left & right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator|=(
            Column const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::transform(*this, right, begin(),
            [](Tp const& left, Tp const& right) -> Tp
                { return left | right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator+=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator-=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator*=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator/=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator%=(
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator^=(
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator&=(
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator|=(
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](Tp& value) -> void
            { value |= right; });
        return *this;
    }

    /**
     * Adds the column to the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right);

    /**
     * Subtract the column from the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right);

    /**
     * Multiplies the column with the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right);

    /**
     * Divides the column by the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right);

    /**
     * Calculates the modulo of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-xor of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the column
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, |);

    /**
     * Adds the column to the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right);

    /**
     * Subtract the column from the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right);

    /**
     * Multiplies the column with the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right);

    /**
     * Divides the column by the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right);

    /**
     * Calculates the modulo of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-xor of the column and the vector
     *
     * @tparam Tp the value type
     * @tparam Rows the matrix's rows
     * @tparam Cols the matrix's columns
     * @param left the constant reference to the vector
     * @param right the constant reference to the column
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, |);

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
     * value fields with the matrix elements
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

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp
                { return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left / right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            typename Matrix<Tp, Rows, Cols>::Column const& left,
            Vector<Tp, Rows> const& right)
                requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator+(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                 return left + right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator-(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left - right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator*(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left * right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator/(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left / right; });
        return result;
    }


    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator%(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, %)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left % right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator^(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left ^ right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator&(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, &)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left & right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Vector<Tp, Rows>
        operator|(
            Vector<Tp, Rows> const& left,
            typename Matrix<Tp, Rows, Cols>::Column const& right)
                requires mpgl_Operable(Tp, |)
    {
        Vector<Tp, Rows> result;
        std::ranges::transform(left, right, result.begin(),
            [](Tp const& left, Tp const& right)-> Tp {
                return left | right; });
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic Up>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Up, Rows, Cols>() const noexcept
    {
        Matrix<Up, Rows, Cols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <std::size_t URows, std::size_t UCols>
        requires (URows >= Rows && UCols >= Cols &&
            (URows != Rows || UCols != Cols))
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Tp, URows, UCols>() const noexcept
    {
        Matrix<Tp, URows, UCols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator+=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left + right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator-=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left - right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator*=(
            Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left * right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator/=(
             Matrix<Tp, Rows, Cols> const& right)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left / right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator%=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, %)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left % right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator^=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left ^ right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator&=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, &)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left & right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator|=(
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, |)
    {
        std::ranges::transform(*this, right, begin(),
            [](auto const& left, auto const& right)
                { return left | right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator+=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value += right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator-=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value -= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator*=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value *= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator/=(
            Tp const& right)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value /= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator%=(
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value %= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator^=(
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value ^= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator&=(
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value &= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator|=(
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        std::ranges::for_each(*this, [&right](auto& value) -> void
            { value |= right; });
        return *this;
    }

    template <Arithmetic Tp, std::size_t LRows,
        std::size_t LCols, std::size_t RCols>
            requires (LRows > 1 && LCols > 1 && RCols > 1)
    [[nodiscard]] Matrix<Tp, LRows, RCols> operator*(
        Matrix<Tp, LRows, LCols> const& left,
        Matrix<Tp, LCols, RCols> const& right)
    {
        Matrix<Tp, LRows, RCols> result;
        for (std::size_t i = 0; i < LRows; ++i)
            for (std::size_t j = 0; j < RCols; ++j)
                result[i][j] = innerProduct(left[i],
                    right.getColumn(j), Tp{0});
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator+(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result += right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Matrix<Tp, Rows, Cols> operator-(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result -= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] Vector<Tp, Rows> operator*(
        Matrix<Tp, Rows, Cols> const& matrix,
        Vector<Tp, Cols> const& vector)
    {
        Vector<Tp, Cols> result;
        for (std::size_t i = 0;i < Cols; ++i)
            result[i] = innerProduct(matrix[i], vector, Tp{0});
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result += right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result -= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result *= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result /= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, %)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result %= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, ^)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result ^= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator&(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, &)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result &= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Matrix<Tp, Rows, Cols> const& left,
            Tp const& right) requires mpgl_Operable(Tp, |)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result |= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator+(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result += left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator-(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result -= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator*(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result *= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator/(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result /= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator%(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, %)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result %= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator^(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, ^)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result ^= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
         operator&(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, &)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result &= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols>
        operator|(
            Tp const& left,
            Matrix<Tp, Rows, Cols> const& right)
                requires mpgl_Operable(Tp, |)
    {
        Matrix<Tp, Rows, Cols> result{right};
        result |= left;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>&
        Matrix<Tp, Rows, Cols>::operator=(Matrix&& matrix) noexcept
    {
        normalBase = std::move(matrix.normalBase);
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Vector<Tp, Rows>
        Matrix<Tp, Rows, Cols>::Column::operator-(
            void) const noexcept
    {
        return -static_cast<Vector<Tp, Rows>>(*this);
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr auto
        Matrix<Tp, Rows, Cols>::columnsRange(void) noexcept
    {
        return std::ranges::subrange(columnBegin(), columnEnd());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr auto
        Matrix<Tp, Rows, Cols>::columnsRange(void) const noexcept
    {
        return std::ranges::subrange(columnBegin(), columnEnd());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows == Cols && Rows > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Cols> identityMatrix(
        Tp diagonal) noexcept
    {
        Matrix<Tp, Rows, Cols> identity;
        for (std::size_t i = 0; i < Rows; ++i)
            identity[i][i] = diagonal;
        return identity;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] constexpr Matrix<Tp, Cols, Rows>
        transpose(Matrix<Tp, Rows, Cols> const& matrix) noexcept
    {
        Matrix<Tp, Cols, Rows> transposed;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                transposed[j][i] = matrix[i][j];
        return transposed;
    }

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

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Vector<Tp, Rows>>
        LUPDecompositionFn::operator() (
            Matrix<Tp, Rows, Rows>& matrix) const noexcept
    {
        Vector<Tp, Rows> perms;
        std::iota(perms.begin(), perms.end(), Tp{0});
        for (std::size_t k = 0, u; k < Rows; ++k) {
            if (!findPivot(u, k, matrix))
                return {};
            if (u != k) {
                std::swap(perms[k], perms[u]);
                std::swap(matrix[k], matrix[u]);
            }
            permute(k, matrix);
        }
        return { perms };
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<bool>
        LUPDecompositionFn::operator() (
            Matrix<Tp, Rows, Rows>& matrix, CTag) const noexcept
    {
        std::size_t counter = 0;
        for (std::size_t k = 0, u = 0; k < Rows; ++k, u = k) {
            if (!findPivot(u, k, matrix))
                return {};
            if (u != k) {
                ++counter;
                std::swap(matrix[k], matrix[u]);
            }
            permute(k, matrix);
        }
        return { counter % 2 };
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    constexpr bool LUPDecompositionFn::findPivot(
        std::size_t& u,
        std::size_t& k,
        Matrix<Tp, Rows, Rows> const& matrix) const noexcept
    {
        Tp p{0};
        for (std::size_t i = k; i < Rows; ++i) {
            if (Tp v = std::abs(matrix[i][k]); v > p) {
                p = v;
                u = i;
            }
        }
        return p != 0;
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    constexpr void LUPDecompositionFn::permute(
        std::size_t& k,
        Matrix<Tp, Rows, Rows>& matrix) const noexcept
    {
        for (std::size_t i = k + 1; i < Rows; ++i) {
            matrix[i][k] /= matrix[k][k];
            for (std::size_t j = k + 1; j < Rows; ++j)
                 matrix[i][j] -= matrix[i][k] * matrix[k][j];
        }
    }

    template <Arithmetic Tp, std::size_t Rows,
        SizedRange<Tp, Rows> Permutations,
        SizedRange<Tp, Rows> Results>
        requires (Rows > 1)
    [[nodiscard]] constexpr Vector<Tp, Rows>
        lupSolve(
            Matrix<Tp, Rows, Rows> const& luMatrix,
            Permutations const& permutations,
            Results const& results) noexcept
    {
        Vector<Tp, Rows> x, y;
        for (std::size_t i = 0; i < Rows; ++i) {
            Tp sum = std::inner_product(luMatrix[i].begin(),
                luMatrix[i].begin() + i, y.begin(), Tp{0});
            y[i] = results[permutations[i]] - sum;
        }
        for (std::size_t i = Rows - 1; i < Rows; --i) {
            Tp sum = std::inner_product(luMatrix[i].begin() + i + 1,
                luMatrix[i].end(), x.begin() + i + 1, Tp{0});
            x[i] = (y[i] - sum) / luMatrix[i][i];
        }
        return x;
    }

    template <Arithmetic Tp, std::size_t Rows,
        Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Matrix<Up, Rows, Rows>>
        invert(Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto permutations = lupDecomposition(luMatrix)) {
            auto inverseMatrix = identityMatrix<Up, Rows, Rows>();
            for (auto& column : inverseMatrix.columnsRange())
                column = lupSolve(luMatrix, *permutations, column);
            return { inverseMatrix };
        }
        return {};
    }

    template <Arithmetic Tp, std::size_t Rows, Arithmetic Up = Tp>
            requires (Rows > 1)
    [[nodiscard]] constexpr Up det(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto parity = lupDecomposition(luMatrix,
            LUPDecompositionFn::CounterTag))
        {
            Up det{luMatrix[0][0]};
            for (std::size_t i = 1; i < Rows; ++i)
                det *= luMatrix[i][i];
            return *parity ? -det : det;
        }
        return Up{0};
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr Tp trace(
        Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Tp sum{0};
        for (std::size_t i = 0;i < Rows; ++i)
            sum += matrix[i][i];
        return sum;
    }

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

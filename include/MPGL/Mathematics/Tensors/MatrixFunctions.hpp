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

#include <MPGL/Mathematics/Tensors/Views.hpp>
#include <MPGL/Mathematics/Ranges.hpp>

#include <optional>

namespace mpgl {

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

}

#include <MPGL/Mathematics/Tensors/MatrixFunctions.tpp>

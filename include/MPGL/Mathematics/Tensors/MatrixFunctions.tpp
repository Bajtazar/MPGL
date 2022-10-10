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

namespace mpgl {

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
                    right | views::column(j), Tp{0});
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows>
        requires (Rows > 1)
    [[nodiscard]] constexpr Matrix<Tp, Rows, Rows> identityMatrix(
        Tp diagonal) noexcept
    {
        Matrix<Tp, Rows, Rows> identity;
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

    template <Arithmetic Tp, std::size_t Rows, Arithmetic Up>
        requires (Rows > 1)
    [[nodiscard]] constexpr std::optional<Matrix<Up, Rows, Rows>>
        invert(Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto permutations = lupDecomposition(luMatrix)) {
            auto inverseMatrix = identityMatrix<Up, Rows>();
            for (auto column : inverseMatrix | views::columns)
                column = lupSolve(luMatrix, *permutations, column);
            return { inverseMatrix };
        }
        return {};
    }

    template <Arithmetic Tp, std::size_t Rows, Arithmetic Up>
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

}

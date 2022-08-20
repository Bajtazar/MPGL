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

    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    std::ostream& operator<< (
        std::ostream& os,
        Vector<Tp, Size> const& vector)
    {
        os << '[';
        for (std::size_t i = 0;i < vector.size() - 1; ++i)
            os << vector[i] << '\t';
        os << vector[vector.size() - 1] << ']';
        return os;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    std::ostream& operator<< (
        std::ostream& os,
        Matrix<Tp, Rows, Cols> const& matrix)
    {
        os << '[';
        for (std::size_t i = 0;i < matrix.size() - 1; ++i)
            os << matrix[i] << "\n ";
        os << matrix[matrix.size() - 1] << ']';
        return os;
    }

    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    std::istream& operator>> (
        std::istream& is,
        Vector<Tp, Size>& vector)
    {
        for (auto& element : vector)
            is >> element;
        return is;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    std::istream& operator>> (
        std::istream& is,
        Matrix<Tp, Rows, Cols>& matrix)
    {
        for (auto& row : matrix)
            is >> row;
        return is;
    }

    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    [[nodiscard]] std::string toString(
        Vector<Tp, Size> const& vector)
    {
        std::string result = "[";
        for (std::size_t i = 0;i < vector.size() - 1; ++i)
            result += std::to_string(vector[i]) + '\t';
        return result += std::to_string(
            vector[vector.size() - 1]) + ']';
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] std::string toString(
        Matrix<Tp, Rows, Cols> const& matrix)
    {
        std::string result = "[";
        for (std::size_t i = 0;i < matrix.size() - 1; ++i)
            result += toString(matrix[i]) + '\n';
        return result += toString(matrix[matrix.size() - 1]) + ']';
    }

}

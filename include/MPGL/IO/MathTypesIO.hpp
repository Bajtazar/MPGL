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

#include <MPGL/Mathematics/Tensors/Matrix.hpp>

#include <istream>
#include <ostream>

namespace mpgl {

    /**
     * Writes the given vector to the output stream
     *
     * @tparam Tp the type of the vector elements
     * @tparam Size the size of the vector
     * @return a reference to ostream
     */
    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    std::ostream& operator<< (
        std::ostream& os,
        Vector<Tp, Size> const& vector);

    /**
     * Writes the given matrix to the output stream
     *
     * @tparam Tp the type of the matrix elements
     * @tparam Rows the number of matrix's rows
     * @tparam Cols the number of matrix's columns
     * @return a reference to ostream
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    std::ostream& operator<< (
        std::ostream& os,
        Matrix<Tp, Rows, Cols> const& matrix);

    /**
     * Writes data from a input stream to the vector
     *
     * @tparam Tp the type of the vector elements
     * @tparam Size the size of the vector
     * @return a reference to istream
     */
    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    std::istream& operator>> (
        std::istream& is,
        Vector<Tp, Size>& vector);

    /**
     * Writes data from a input stream to the matrix
     *
     * @tparam Tp the type of the matrix elements
     * @tparam Rows the number of matrix's rows
     * @tparam Cols the number of matrix's columns
     * @return a reference to istream
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    std::istream& operator>> (
        std::istream& is,
        Matrix<Tp, Rows, Cols>& matrix);

    /**
     * Writes the given vector to the string
     *
     * @tparam Tp the type of the vector elements
     * @tparam Size the size of the vector
     * @return the string with encoded vector
     */
    template <Arithmetic Tp, std::size_t Size>
        requires (Size > 0)
    [[nodiscard]] std::string toString(
        Vector<Tp, Size> const& vector);

    /**
     * Writes the given matrix to the string
     *
     * @tparam Tp the type of the matrix elements
     * @tparam Rows the number of matrix's rows
     * @tparam Cols the number of matrix's columns
     * @return the string with encoded matrix
     */
    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    [[nodiscard]] std::string toString(
        Matrix<Tp, Rows, Cols> const& matrix);

}

#include <MPGL/IO/MathTypesIO.tpp>

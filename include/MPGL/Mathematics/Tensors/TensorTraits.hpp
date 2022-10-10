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

#include <MPGL/Mathematics/Tensors/Matrix.hpp>

namespace mpgl {

    /**
     * Helper metastruct. Helps to determine whether the given
     * type is a specialization of the matrix template class
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    class MatrixTypeFn {
        /**
         * Method choosen by the compiler when the given type
         * is a specialization of the matrix template class
         *
         * @tparam Up the matrix's element type
         * @tparam R the number of matrix's rows
         * @tparam C the number of matrix's columns
         * @return std::true_type
         */
        template <Arithmetic Up, std::size_t R, std::size_t C>
        static constexpr std::true_type helper(
            Matrix<Up, R, C> const&);

        /**
         * Method choosen by the compiler when the given type
         * is not a specialization of the matrix template class
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr std::false_type helper(...);
    public:
        static constexpr bool value = std::same_as<std::true_type,
            decltype(helper(std::declval<Tp>()))>;
    };

    /**
     * Checks whether the given type is a specialization of
     * the matrix template class
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept MatrixType = MatrixTypeFn<Tp>::value;

    /**
     * Helper metastruct. Helps to determine whether the given
     * type is a specialization of the vector template class
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    class VectorTypeFn {
        /**
         * Method choosen by the compiler when the given type
         * is a specialization of the vector template class
         *
         * @tparam Up the vector's element type
         * @tparam R the number of vector's elements
         * @return std::true_type
         */
        template <Arithmetic Up, std::size_t N>
        static constexpr std::true_type helper(
            Vector<Up, N> const&);

        /**
         * Method choosen by the compiler when the given type
         * is not a specialization of the vector template class
         *
         * @param ... whichever type
         * @return std::false_type
         */
        static constexpr std::false_type helper(...);
    public:
        static constexpr bool value = std::same_as<std::true_type,
            decltype(helper(std::declval<Tp>()))>;
    };

    /**
     * Checks whether the given type is a specialization of
     * the vector template class
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept VectorType = VectorTypeFn<Tp>::value;

}

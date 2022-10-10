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

#include <MPGL/Traits/Concepts.hpp>

namespace mpgl {

    namespace dim {

        /**
         * Dimension tags class
         *
         * @tparam Degree the degree of the orthogonal space
         * represented by this tag
         */
        template <std::size_t Degree>
        struct Dimension {

            constexpr static std::size_t const orthogonal_space_degree
                = Degree;

        };

        /// The Null tag
        typedef Dimension<0>                            NullDim;

        /// The 2D tag
        typedef Dimension<2>                            Dim2;

        /// The 3D tag
        typedef Dimension<3>                            Dim3;

    }

    /**
     * Checks wheter the given type is the dimension tag
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept Dimension = requires {
        Tp::orthogonal_space_degree;
    } && std::same_as<decltype(Tp::orthogonal_space_degree),
            std::size_t const>;

    /**
     * Checks wheter the given type is a two dimensional space's tag
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept TwoDimensional = Dimension<Tp> &&
        Tp::orthogonal_space_degree == 2u;

    /**
     * Checks wheter the given type is a three dimensional space's tag
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept ThreeDimensional = Dimension<Tp> &&
        Tp::orthogonal_space_degree == 3u;

}

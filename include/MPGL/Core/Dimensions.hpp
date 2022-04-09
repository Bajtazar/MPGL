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

#include <concepts>

namespace mpgl {

    namespace dim {

        /**
         * Base class for dimensions tags
         */
        struct Dimension {};

        /**
         * The 2D tag
         */
        struct Dim2 : Dimension {};

        /**
         * The 3D tag
         */
        struct Dim3 : Dimension {};

    }

    /**
     * Checks wheter the given type is the dimension tag
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept Dimension = std::derived_from<Tp, dim::Dimension>;

}

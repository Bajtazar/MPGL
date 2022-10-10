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

    /**
     * Helper metastruct. Provides information about the
     * dimension of the type
     *
     * @tparam Tp the checked type
     * @tparam Up the instance of the Tp template
     */
    template <template <class> class Tp, InstanceOf<Tp> Up>
    class DimensionOf {
        /**
         * Gets the dimension tag from the Tp instance
         *
         * @tparam Dim the instance's dimension
         * @return the dimension tag
         */
        template <Dimension Dim>
        static auto helper(Tp<Dim> const&) noexcept -> Dim;

        /**
         * Returns a null dimension in case when the dimension
         * cannot be checked
         *
         * @return the null dimension
         */
        static dim::NullDim helper(...) noexcept;
    public:
        /**
         * The Tp instance's dimension tag
         */
        using dimension = decltype(helper(std::declval<Up>()));
    };

    /**
     * The convinient shortcut for the DimensionOf dimension
     *
     * @tparam Tp the checked type
     * @tparam Up the instance of the Tp template
     */
    template <template <class> class Tp, InstanceOf<Tp> Up>
    using DimensionOfT = typename DimensionOf<Tp, Up>::dimension;

}

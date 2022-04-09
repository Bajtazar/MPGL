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

#include <MPGL/Core/Drawable.hpp>

namespace mpgl {

    /**
     * Helper metastruct. Provides information about the
     * dimension of the drawable
     *
     * @tparam Tp the type of the drawable instance
     */
    template <InstanceOf<Drawable> Tp>
    class DrawableDimension {
        /**
         * Gets the dimension tag from the drawable instance
         *
         * @tparam Dim the instance's dimension
         * @return the dimension tag
         */
        template <Dimension Dim>
        static auto helper(Drawable<Dim> const&) noexcept
            -> Dim;
    public:
        /**
         * The drawable instance's dimension tag
         */
        using dimension = decltype(helper(std::declval<Tp>()));
    };

    /**
     * The convinient shortcut for the DrawableDimension dimension
     *
     * @tparam Tp the instance of the drawable template
     */
    template <InstanceOf<Drawable> Tp>
    using DrawableDimensionT
        = typename DrawableDimension<Tp>::dimension;

}

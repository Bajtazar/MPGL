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

#include <MPGL/Core/Layouts/Layout.hpp>

namespace mpgl {

    /**
     * Holds the type of the layout and the rest of arguments
     * needed for creation of the layout object
     *
     * @tparam Tp the layout's type
     * @tparam Args the rest of the layout argument types
     */
    template <std::derived_from<Layout> Tp, class... Args>
        requires std::constructible_from<Tp, Vector2u const&, Args...>
    struct LayoutTag {
        using Layout = Tp;

        /**
         * Constructs a new layout tag object
         *
         * @param args a universal reference to the rest
         * of the layout constructor's arguments
         */
        explicit LayoutTag(Args&&... args)
            : args{std::forward(args...)} {}

        std::tuple<Args>                                args;
    };

    /**
     * Specialization of the layout tag class when the layout
     * does not require any aditional constructor arguments
     *
     * @tparam Tp the layout's type
     */
    template <std::derived_from<Layout> Tp>
        requires std::constructible_from<Tp, Vector2u const&>
    struct LayoutTag<Tp> {
        using Layout = Tp;
    };

}

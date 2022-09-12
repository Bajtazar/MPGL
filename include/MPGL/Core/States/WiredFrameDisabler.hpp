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
     * Disables rendering primitives as wireframes upon draw call
     *
     * @tparam Dim the dimension's tag of the drawable
     */
    template <Dimension Dim>
    struct WiredFrameDisabler : public Drawable<Dim> {
        /**
         * Constructs a new wired frame disabler object
         */
        explicit WiredFrameDisabler(void) noexcept = default;

        WiredFrameDisabler(WiredFrameDisabler const&) = default;
        WiredFrameDisabler(WiredFrameDisabler&&) = default;

        WiredFrameDisabler& operator=(
            WiredFrameDisabler const&) = default;
        WiredFrameDisabler& operator=(
            WiredFrameDisabler&&) = default;

        /**
         * Disables rendering primitives as wireframes
         */
        void draw(void) const noexcept final;

        /**
         * Destroys the wired frame disabler object
         */
        ~WiredFrameDisabler(void) noexcept = default;
    };

    template class WiredFrameDisabler<dim::Dim2>;
    template class WiredFrameDisabler<dim::Dim3>;

    using WiredFrameDisabler2D = WiredFrameDisabler<dim::Dim2>;
    using WiredFrameDisabler3D = WiredFrameDisabler<dim::Dim3>;

}

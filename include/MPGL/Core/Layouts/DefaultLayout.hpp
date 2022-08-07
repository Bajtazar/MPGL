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
     * The layout used by default in the MPGL's windows. It ensures
     * that the object will be intact by the screen transformation
     * event
     */
    class DefaultLayout : public Layout {
    public:
        /**
         * Constructs a new Default Layout object
         *
         * @param oldDimensions the dimensions of the window
         * before screen transformation event
         */
        explicit DefaultLayout(
            Vector2u const& oldDimensions) noexcept;

        DefaultLayout(DefaultLayout const&) noexcept = default;
        DefaultLayout(DefaultLayout&&) noexcept = default;

        DefaultLayout& operator=(
            DefaultLayout const&) noexcept = default;

        DefaultLayout& operator=(
            DefaultLayout&&) noexcept = default;

        /**
         * Ensures that the coordinates will be intact by the
         * screen transformation event
         *
         * @param coords the reference to the coordinates object
         */
        void operator() (
            any::InputRange<Adapter2D>& coords) const noexcept final;

        /**
         * Ensures that the coordinate will be intact by the
         * screen transformation event
         *
         * @param coord the reference to the coordinate object
         */
        void operator() (Adapter2D& coord) const noexcept final;

        /**
         * Destroys the Default Layout object
         */
        ~DefaultLayout(void) noexcept = default;
    private:
        using Vector2uR = std::reference_wrapper<Vector2u const>;

        Vector2uR                               dimensions;
    };

}

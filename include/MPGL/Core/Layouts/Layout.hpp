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

#include <MPGL/Core/Transformations/Transformation.hpp>
#include <MPGL/Core/Context/Context.hpp>

namespace mpgl {

    /**
     * An interface for all layouts. Layout allows to determine
     * the behavior of the screen transformation event
     */
    class Layout :
        public Transformation2D,
        protected GraphicalObject
    {
    public:
        /**
         * Constructs a new Layout object
         */
        explicit Layout(void) noexcept = default;

        /**
         * Pure virtual method. Has to be overloaded. Perfoms
         * transformation on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        virtual void operator() (
           any::InputRange<Adapter2D>& coords) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Perfoms
         * transformation on the coodinate
         *
         * @param coord the reference to the coordinate object
         */
        virtual void operator() (Adapter2D& coord) const noexcept = 0;

        /**
         * Virtual destructor. Destroys the Layout object
         */
        virtual ~Layout(void) noexcept = default;
    protected:
        Layout(Layout const&) noexcept = default;
        Layout(Layout&&) noexcept = default;

        Layout& operator=(Layout const&) noexcept = default;
        Layout& operator=(Layout&&) noexcept = default;
    };

}
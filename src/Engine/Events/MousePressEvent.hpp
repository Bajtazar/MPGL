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

#include "../IO/Devices/Mouse.hpp"
#include "Event.hpp"

namespace mpgl {

    /**
     * Base class for classes that are notified when mouse button
     * is pressed
     */
    class MousePressEvent : public EventBase {
    public:
        explicit MousePressEvent(void) noexcept = default;

        MousePressEvent& operator=(MousePressEvent const&) = delete;
        MousePressEvent& operator=(MousePressEvent&&) = delete;

        /**
         * Pure virtual method. Has to be overloaded.
         * Notifies when mousr button is pressed
         *
         * @param button the pressed mouse button
         */
        virtual void onMousePress(
            MouseButton const& button) noexcept = 0;

        /**
         * Virtual destructor. Destroy the Mouse Press Event object
         */
        virtual ~MousePressEvent(void) noexcept = default;
    protected:
        MousePressEvent(MousePressEvent const&) noexcept = default;
        MousePressEvent(MousePressEvent&&) noexcept = default;
    };

}

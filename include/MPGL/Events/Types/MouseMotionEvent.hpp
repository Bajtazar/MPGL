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

#include <MPGL/Mathematics/Vector.hpp>
#include <MPGL/Events/Event.hpp>

namespace mpgl {

    /**
     * Base class for classes that are notified when mouse is moved
     */
    class MouseMotionEvent : public virtual EventBase {
    public:
        explicit MouseMotionEvent(void) noexcept = default;

        MouseMotionEvent& operator=(MouseMotionEvent const&) = delete;
        MouseMotionEvent& operator=(MouseMotionEvent&&) = delete;

        /**
         * Pure virtual method. Has to be overloaded.
         * Notifies when mouse is moved
         *
         * @param position the new mouse position
         */
        virtual void onMouseMotion(
            Vector2f const& position) noexcept = 0;

        /**
         * Virtual destructor. Destroys the Mouse Motion Event object
         */
        virtual ~MouseMotionEvent(void) noexcept = default;
    protected:
        MouseMotionEvent(MouseMotionEvent const&) noexcept = default;
        MouseMotionEvent(MouseMotionEvent&&) noexcept = default;
    };

}

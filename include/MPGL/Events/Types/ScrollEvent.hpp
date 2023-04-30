/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Events/Event.hpp>

namespace mpgl {

    /**
     * Base class for classes that are notified when user uses
     * mouse scroll
     */
    class ScrollEvent : public virtual EventBase {
    public:
        explicit ScrollEvent(void) noexcept = default;

        ScrollEvent& operator=(ScrollEvent const&) = delete;
        ScrollEvent& operator=(ScrollEvent&&) = delete;

        /**
         * Pure virtual method. Has to be overloaded.
         * Notifies when user uses mouse scroll
         *
         * @param scroll the scroll position
         */
        virtual void onScroll(
            Vector2f const& scroll) noexcept = 0;

        /**
         * Virtual destructor. Destroys the Scroll Event object
         */
        virtual ~ScrollEvent(void) noexcept = default;
    protected:
        ScrollEvent(ScrollEvent const&) noexcept = default;
        ScrollEvent(ScrollEvent&&) noexcept = default;
    };

}

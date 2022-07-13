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

#include <MPGL/IO/Devices/Keyboard.hpp>
#include <MPGL/Events/Event.hpp>

namespace mpgl {

    /**
     * Base class for classes that are notified when key is pressed
     */
    class KeyPressEvent : public virtual EventBase {
    public:
        explicit KeyPressEvent(void) noexcept = default;

        KeyPressEvent& operator=(KeyPressEvent const&) = delete;
        KeyPressEvent& operator=(KeyPressEvent&&) = delete;

        /**
         * Pure virtual method. Has to be overloaded.
         * Notifies when the key is pressed
         *
         * @param key the pressed key
         */
        virtual void onKeyPress(
            Key const& key) noexcept = 0;

        /**
         * Virtual destructor. Destroys the Key Press Event object
         */
        virtual ~KeyPressEvent(void) noexcept = default;
    protected:
        KeyPressEvent(KeyPressEvent const&) noexcept = default;
        KeyPressEvent(KeyPressEvent&&) noexcept = default;
    };

}

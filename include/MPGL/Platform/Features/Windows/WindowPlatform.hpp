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

#include <MPGL/Events/Management/EventManager.hpp>
#include <MPGL/Core/Context/Context.hpp>
#include <MPGL/Collections/Image.hpp>

namespace mpgl::platform {

    /**
     * Interface for the window's platfrom dependent implementations
     */
    class WindowPlatform : protected GraphicalObject {
    public:
        /**
         * Constructs a new window platform object
         *
         * @param dimensions the window's dimensions
         * @param title the window's title
         * @param options the window's options
         */
        explicit WindowPlatform(
            Vector2u dimensions,
            std::string title,
            Options const& options);

        /**
         * Returns the window dimensions vector
         *
         * @return the constaint reference to the window dimensions
         * vector
         */
        [[nodiscard]] Vector2u const&
            getWindowDimensions(void) const noexcept
                { return dimensions; }

        /**
         * Returns the window title
         *
         * @return the constant reference to the window title
         * string
         */
        [[nodiscard]] std::string const&
            getWindowTitle(void) const noexcept
                { return title; }

        /**
         * Sets the event manager used by the window
         *
         * @param eventManager a non-handling pointer to the
         * event manager object
         */
        void setEventManager(WindowEventManager* eventManager) noexcept
            { this->eventManager = eventManager; }

        /**
         * Pure virtual method. Has to be overloaded. Closes the window
         */
        virtual void closeWindow(void) noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Opens the window
         */
        virtual void openWindow(void) noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Sets the position
         * of the window on the screen
         *
         * @param position a constant reference to the
         * new position vector
         */
        virtual void setPosition(Vector2u const& position) noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Minimizes
         * the window
         */
        virtual void minimize(void) noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Maximizes
         * the window
         */
        virtual void maximize(void) noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Returns
         * whether the window should be closed
         *
         * @return if the window should be closed
         */
        [[nodiscard]] virtual bool shouldWindowClose(
            void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Clears the framebuffer
         *
         * @param color the background color
         */
        virtual void clear(Color const& color) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Draws the framebuffer in the window
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Saves the
         * current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] virtual Image saveWindowScreen(void) const = 0;

        /**
         * Pure virtual method. Has to be overloaded. Sets
         * the window as the current one
         */
        virtual void setContextWindow(void) noexcept = 0;

        /**
         * Virtual destructor. Destroys the window platform object
         */
        virtual ~WindowPlatform(void) noexcept = default;
    protected:
        WindowPlatform(WindowPlatform const&) = default;
        WindowPlatform(WindowPlatform&&) = default;

        WindowPlatform& operator=(WindowPlatform const&) = default;
        WindowPlatform& operator=(WindowPlatform&&) = default;

        Vector2u                                dimensions;
        Options                                 options;
        std::string                             title;
        WindowEventManager*                     eventManager = nullptr;

        /**
         * Pure virtual method. Has to be overloaded. Sets
         * the Window options
         */
        virtual void setWindowOptions(void) const noexcept = 0;

        /**
         * Sets the window dimensions
         *
         * @param dim the window dimensions
         */
        void setDimensions(Vector2u const& dim) noexcept;
    };

}

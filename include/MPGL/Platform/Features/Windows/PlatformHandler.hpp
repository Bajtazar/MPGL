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

#include <MPGL/Platform/Features/Windows/Window.hpp>

#include <memory>

namespace mpgl::platform {

    /**
     * Handler that allows to derived from the object containing
     * the window implementation thus enforcing the initialization
     * and destruction order
     */
    class PlatfromHandler {
    public:
        using WindowPtr = std::unique_ptr<Window>;

        /**
         * Constructs a new platfrom handler object
         *
         * @param windowPtr a rvalue reference to the window's
         * unique pointer
         */
        explicit PlatfromHandler(WindowPtr&& windowPtr) noexcept
            : handler{std::move(windowPtr)} {}

        /**
         * Returns the window title
         *
         * @return the constant reference to the window title
         * string
         */
        [[nodiscard]] std::string const&
            getWindowTitle(void) const noexcept
                { return handler->getWindowTitle(); }

        /**
         * Sets the event manager used by the window
         *
         * @param eventManager a non-handling pointer to the
         * event manager object
         */
        void setEventManager(
            WindowEventManager* eventManager) noexcept
                { handler->setEventManager(eventManager); }

        /**
         * Closes the window
         */
        void closeWindow(void) noexcept
            { handler->closeWindow(); }

        /**
         * Opens the window
         */
        void openWindow(void) noexcept
            { handler->openWindow(); }

        /**
         * Sets the position of the window on the screen
         *
         * @param position a constant reference to the
         * new position vector
         */
        void setPosition(Vector2u const& position) noexcept
            { handler->setPosition(position); }

        /**
         * Minimizes the window
         */
        void minimize(void) noexcept
            { handler->minimize(); }

        /**
         * Maximizes the window
         */
        void maximize(void) noexcept
            { handler->maximize(); }

        /**
         * Returns whether the window should be closed
         *
         * @return if the window should be closed
         */
        [[nodiscard]] bool shouldWindowClose(
            void) const noexcept
                { handler->shouldWindowClose(); }

        /**
         * Clears the framebuffer
         *
         * @param color the background color
         */
        void clear(Color const& color) const noexcept
            { handler->clear(color); }

        /**
         * Draws the framebuffer in the window
         */
        void draw(void) const noexcept
            { handler->draw(); }

        /**
         * Saves the current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] Image saveWindowScreen(void) const
            { return handler->saveWindowScreen(); }

        /**
         * Sets the window as the current one
         */
        void setContextWindow(void) noexcept
            { handler->setContextWindow(); }

        /**
         * Destroys the platfrom handler object
         */
        ~PlatfromHandler(void) noexcept = default;
    private:
        WindowPtr                       handler;
    };

}

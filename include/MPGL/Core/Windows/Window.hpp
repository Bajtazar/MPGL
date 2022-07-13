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

#include <MPGL/Core/Windows/WindowPlatform.hpp>

namespace mpgl {

    /**
     * Creates window and renders the given content on the screen.
     * Calls the specific event manager method when the designated
     * event occurs
     */
    class Window : public WindowPlatform {
    public:
        /**
         * Creates a new window with given dimensions, title
         * and options
         *
         * @param dimensions the dimensions of the window
         * @param title the title of the window
         * @param options the options which determins different
         * behaviours of window
         * @param eventManager the event manager used by the window
         */
        explicit Window(
            Vector2u const& dimensions,
            std::string const& title,
            Options const& options = Options{},
            EventManagerPtr eventManager
                = std::make_unique<BasicWindowEventManager>());

        Window(Window const& window) noexcept = delete;
        Window(Window&& window) noexcept = delete;

        Window& operator= (Window const& window) noexcept = delete;
        Window& operator= (Window&& window) noexcept = delete;

        /**
         * Sets this window as a context one
         */
        void setContextWindow(void) noexcept;

        /**
         * Restrains the maximum number of frames per second
         * rendered in the window. When 0 is given, then
         * removes the limit
         *
         * @param fpsLimit the fps limit
         */
        void setFPSLimit(std::size_t fpsLimit) noexcept;

        /**
         * Sets the window tickrate
         *
         * @param ticks the number of ticks per second
         */
        void setTickrate(std::size_t ticks) noexcept;

        /**
         * Draw frames until the window has to be closed
         *
         * @param background the background color
         */
        void windowLoop(Color const& background = Color::Black) noexcept;

        /**
         * Returns a reference to the shader library object
         *
         * @return the reference to the shader library object
         */
        [[nodiscard]] ShaderLibrary& getShaderLib(void) noexcept
            { return shaders; }

        /**
         * Saves the current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] Image saveWindowScreen(void) const final;

        /**
         * Destroy the Window object
         */
        ~Window(void) noexcept;
    private:
        typedef std::chrono::microseconds           Duration;
        typedef std::chrono::steady_clock           ThreadClock;
        typedef ThreadClock::time_point             TimePoint;

        /**
         * Draws drawables on the screen
         */
        void drawDrawables(void) const noexcept;

        /**
         * Draws the frame in the window
         */
        void draw(void) noexcept;

        /**
         * Clears the screen in the window
         */
        void clear(Color const&) noexcept;

        ShaderLibrary                               shaders;
        Duration                                    sleepTime;
        TimePoint                                   lastTime;
    };


}

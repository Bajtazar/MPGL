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

#include <MPGL/Core/Windows/WindowBase.hpp>

namespace mpgl {

    /**
     * Implements platform specific operations on window
     */
    class WindowPlatform : public WindowBase {
    public:
        WindowPlatform(void) = delete;

        WindowPlatform(WindowPlatform const&) = delete;
        WindowPlatform(WindowPlatform&&) = delete;

        WindowPlatform& operator=(WindowPlatform const&) = delete;
        WindowPlatform& operator=(WindowPlatform&&) = delete;

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
         * Closes the window
         */
        void closeWindow(void) noexcept;

        /**
         * Opens the window
         */
        void openWindow(void) noexcept;

        /**
         * Virtual destructor. Destroy the Window Platform object
         */
        virtual ~WindowPlatform(void) noexcept;
    protected:
        /**
         * Construct a new Window Platform object
         *
         * @param dimensions the window's dimensions
         * @param title the window's title
         * @param options the window's options
         * @param eventManager the window's event manager
         */
        explicit WindowPlatform(
            Vector2u dimensions,
            std::string title,
            Options const& options,
            EventManagerPtr&& eventManager);

        /**
         * Returns whether the window should be closed
         *
         * @return if the window should be closed
         */
        [[nodiscard]] bool shouldWindowClose(void) const noexcept
            { return glfwWindowShouldClose(window); }

        /**
         * Clears the framebuffer
         *
         * @param color the background color
         */
        void clear(Color const& color) const noexcept;

        /**
         * Draws the framebuffer in the window
         */
        void draw(void) const noexcept;

        /**
         * Pure virtual method. Has to be overloaded. Saves the
         * current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] virtual Image saveWindowScreen(void) const = 0;

        /**
         * Sets the window as the current one
         */
        void setContextWindow(void) noexcept;
    private:
        Vector2u                                dimensions;
        Options                                 options;
        std::string                             title;
        GLFWwindow*                             window;

        /**
         * Sets the Window options
         */
        void setWindowOptions(void) const noexcept;

        /**
         * Sets the window callbacks
         */
        void setCallbacks(void) noexcept;

        /**
         * Sets the window dimensions
         *
         * @param dim the window dimensions
         */
        void setDimensions(Vector2u const& dim) noexcept;

        /**
         * The framebuffer callback
         *
         * @param window the GLFW window pointer
         * @param width the new framebuffer width
         * @param height the new framebuffer height
         */
        static void framebufferCallback(
            GLFWwindow *window,
            int32 width,
            int32 height) noexcept;

        /**
         * The window position callback
         *
         * @param window the GLFW window pointer
         * @param xPos the window's new positon's x-axis coordinate
         * @param yPos the window's new positon's y-axis coordinate
         */
        static void windowPositionCallback(
            GLFWwindow *window,
            int32 xPos,
            int32 yPos) noexcept;

        /**
         * The keyboard callback
         *
         * @param window the GLFW window pointer
         * @param key the key code
         * @param scancode the scan code
         * @param action the performed action
         * @param mods the modifiers
         */
        static void keyCallback(
            GLFWwindow *window,
            int32 key,
            int32 scancode,
            int32 action,
            int32 mods) noexcept;

        /**
         * The text callback
         *
         * @param window the GLFW window pointer
         * @param character the given character code [in unicode]
         */
        static void textCallback(
            GLFWwindow *window,
            uint32 character) noexcept;

        /**
         * The mouse position callback
         *
         * @param window the GLFW window pointer
         * @param xpos the mouse x-axis position
         * @param ypos the mouse y-axis position
         */
        static void mousePosCallback(
            GLFWwindow *window,
            float64 xpos,
            float64 ypos) noexcept;

        /**
         * The mouse button callback
         *
         * @param window the GLFW window pointer
         * @param button the mouse button
         * @param action the performed action
         * @param mods the modifiers
         */
        static void mouseButtonCallback(
            GLFWwindow *window,
            int32 button,
            int32 action,
            int32 mods) noexcept;

        /**
         * The window close callback
         *
         * @param window the GLFW window pointer
         */
        static void windowCloseCallback(
            GLFWwindow *window) noexcept;
    };

}

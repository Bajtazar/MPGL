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

#include <MPGL/Platform/Features/Windows/WindowPlatform.hpp>

#include <GLFW/glfw3.h>

namespace mpgl::platform {

    /**
     * Implements the window using the GLFW3 library
     */
    class GLFWWindow : public WindowPlatform {
    public:
        /**
         * Constructs a new GLFW window object
         *
         * @param dimensions the window's dimensions
         * @param title the window's title
         * @param options the window's options
         */
        explicit GLFWWindow(
            Vector2u dimensions,
            std::string title,
            Options const& options);

        GLFWWindow(GLFWWindow const&) = delete;
        GLFWWindow(GLFWWindow&&) = delete;

        GLFWWindow& operator=(GLFWWindow const&) = delete;
        GLFWWindow& operator=(GLFWWindow&&) = delete;

        /**
         * Closes the window
         */
        void closeWindow(void) noexcept final;

        /**
         * Opens the window
         */
        void openWindow(void) noexcept final;

        /**
         * Sets the position of the window on the screen
         *
         * @param position a constant reference to the
         * new position vector
         */
        void setPosition(Vector2u const& position) noexcept final;

        /**
         * Minimizes the window
         */
        void minimize(void) noexcept final;

        /**
         * Maximizes the window
         */
        void maximize(void) noexcept final;

        /**
         * Returns whether the window should be closed
         *
         * @return if the window should be closed
         */
        [[nodiscard]] bool shouldWindowClose(
            void) const noexcept final;

        /**
         * Clears the framebuffer
         *
         * @param color the background color
         */
        void clear(Color const& color) const noexcept final;

        /**
         * Draws the framebuffer in the window
         */
        void draw(void) const noexcept final;

        /**
         * Saves the current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] Image saveWindowScreen(void) const final;

        /**
         * Sets the window as the current one
         */
        void setContextWindow(void) noexcept final;

        ~GLFWWindow(void) noexcept override;
    private:
        GLFWwindow*                             window;

        /**
         * Sets the window options
         */
        void setWindowOptions(void) const noexcept final;

        /**
         * Sets the window invariant attribibutes
         */
        void setWindowAttributes(void) const noexcept;

        /**
         * Sets the window callbacks
         */
        void setCallbacks(void) noexcept;

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

        /**
         * Actualizes absolut mouse position
         */
        static void actualizeAbsolutePosition(void) noexcept;
    };

}

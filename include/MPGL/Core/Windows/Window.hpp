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

#include <MPGL/Platform/Features/Windows/PlatformHandler.hpp>
#include <MPGL/Core/Camera/StaticCamera.hpp>
#include <MPGL/Core/Windows/WindowBase.hpp>

namespace mpgl {

    /**
     * Creates window and renders the given content on the screen.
     * Calls the specific event manager method when the designated
     * event occurs
     */
    class Window :
        private platform::PlatformHandler,
        public WindowBase {
    public:
        using String = std::string;
        using Paths = std::vector<String>;

        /**
         * Creates a new window with given dimensions, title
         * and options
         *
         * @param dimensions the dimensions of the window
         * @param title the title of the window
         * @param options the options which determins different
         * behaviours of window
         * @param eventManager the event manager used by the window
         * @param shaderDirectories a list of paths or slgz packages
         * checked during shader loading phase
         */
        explicit Window(
            Vector2u const& dimensions,
            String const& title,
            Options const& options = Options{},
            EventManagerPtr eventManager = defaultManager(),
            Paths const& shaderDirectories = defaultShaderDirs());

        Window(Window const& window) = delete;
        Window(Window&& window) noexcept = default;

        Window& operator= (Window const& window) = delete;
        Window& operator= (Window&& window) noexcept = default;

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
         * Returns the window dimensions vector
         *
         * @return the constaint reference to the window dimensions
         * vector
         */
        [[nodiscard]] Vector2u const&
            getWindowDimensions(void) const noexcept;

        /**
         * Returns the window title
         *
         * @return the constant reference to the window title
         * string
         */
        [[nodiscard]] std::string const&
            getWindowTitle(void) const noexcept;

        /**
         * Closes the window
         */
        void closeWindow(void) noexcept;

        /**
         * Opens the window
         */
        void openWindow(void) noexcept;

        /**
         * Sets the position of the window on the screen
         *
         * @param position a constant reference to the
         * new position vector
         */
        void setPosition(Vector2u const& position) noexcept;

        /**
         * Minimizes the window
         */
        void minimize(void) noexcept;

        /**
         * Maximizes the window
         */
        void maximize(void) noexcept;

        /**
         * Draws frames until the window has to be closed. Draws
         * only 2D drawables
         *
         * @param background a background color
         */
        void windowLoop(
            Color const& background = colors::black) noexcept;

        /**
         * Draws frames until the window has to be closed. Draws
         * 3D drawables first and switches to the 2D mode. Adds
         * camera to the event registers if it derives from any
         *
         * @tparam CameraTp the type of the camera
         * @param projection a constant reference to the projection
         * matrix
         * @param background a background color
         * @param cameraPtr a shared pointer to the camera object
         * (StaticCamera by default)
         */
        template <std::derived_from<Camera> CameraTp = StaticCamera>
        void windowLoop(
            Matrix4f const& projection,
            Color const& background = colors::black,
            std::shared_ptr<CameraTp> cameraPtr = defaultCamera()
            ) noexcept;

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
        [[nodiscard]] Image saveWindowScreen(void) const;

        /**
         * Destroy the Window object
         */
        ~Window(void) noexcept;
    private:
        typedef std::chrono::microseconds           Duration;
        typedef std::chrono::steady_clock           ThreadClock;
        typedef ThreadClock::time_point             TimePoint;
        typedef std::shared_ptr<StaticCamera>       StaticCameraPtr;
        typedef std::shared_ptr<Camera>             CameraPtr;
        typedef typename platform::PlatformHandler
            ::WindowPtr                             WindowPtr;

        /**
         * Draws 2D drawables on the screen
         */
        void draw2DDrawables(void) const noexcept;

        /**
         * Draws 3D drawables on the screen
         */
        void draw3DDrawables(void) const noexcept;

        /**
         * Draws the frame in the window
         */
        void draw(void) noexcept;

        /**
         * Clears the screen in the window
         */
        void clear(Color const&) noexcept;

        /**
         * Sets the constext's view-projection matrix
         *
         * @param cameraPtr a constant reference to the camera
         * shared pointer
         */
        void setVPMatrix(CameraPtr const& cameraPtr) noexcept;

        /**
         * Adds camera to the event registers if camera derives
         * from them
         *
         * @tparam CameraTp the type of the camera
         * @param camera a constant reference to the camera shared
         * pointer
         */
        template <std::derived_from<Camera> CameraTp>
        void addCameraEventIfDerived(
            std::shared_ptr<CameraTp> const& camera) noexcept;

        ShaderLibrary                               shaders;
        Duration                                    sleepTime;
        TimePoint                                   lastTime;

        /**
         * Returns the default platform-dependent implementation
         * of the window (GLFWWindow)
         *
         * @param dimensions a constant reference to the window's
         * dimensions
         * @param title a constant reference to the window's
         * dimensions
         * @param options a constant reference to the window's
         * options
         * @return the default window implementation
         */
        static WindowPtr defaultWindowPlatfrom(
            Vector2u const& dimensions,
            String const& title,
            Options const& options);

        /**
         * Returns the default event manager for the window
         * (BasicWindowEventManager)
         *
         * @return the default event manager for the window
         */
        static EventManagerPtr defaultManager(void) noexcept;

        /**
         * Returns the default shader directories
         *
         * @return the default shader directories
         */
        static Paths defaultShaderDirs(void) noexcept;

        /**
         * Returns the default camera (StaticCamera)
         *
         * @return the default camera
         */
        static StaticCameraPtr defaultCamera(void) noexcept;
    };

}

#include <MPGL/Core/Windows/Window.tpp>

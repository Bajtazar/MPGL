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
#include <MPGL/Platform/Features/Windows/GLFWWindow.hpp>
#include <MPGL/Core/Windows/Window.hpp>

#ifndef MPGL_SOURCE_DIR
#define MPGL_SOURCE_DIR "."
#endif

namespace mpgl {

    using std::operator""us;

    Window::EventManagerPtr Window::defaultManager(void) noexcept {
        return std::make_unique<BasicWindowEventManager>();
    }

    Window::Paths Window::defaultShaderDirs(void) noexcept {
        return {
            "shaders",
            "shaders.sl.gz",
            String(MPGL_SOURCE_DIR) + "/shaders.sl.gz"
        };
    }

    Window::StaticCameraPtr Window::defaultCamera(void) noexcept {
        return std::make_shared<StaticCamera>();
    }

    Window::WindowPtr Window::defaultWindowPlatfrom(
        Vector2u const& dimensions,
        String const& title,
        Options const& options)
    {
        return std::make_unique<platform::GLFWWindow>(
            dimensions,
            title,
            options
        );
    }

    Window::Window(
        Vector2u const& dimensions,
        String const& title,
        Options const& options,
        EventManagerPtr eventManager,
        Paths const& shaderDirectories)
            : platform::PlatformHandler{defaultWindowPlatfrom(
                dimensions, title, options)},
            WindowBase{std::move(eventManager)},
            shaders{shaderDirectories}, sleepTime{0us}, lastTime{0us}
    {
        windowImpl->setEventManager(this->eventManager.get());
        context.shaders.setLibrary(shaders);
    }

    [[nodiscard]] Vector2u const&
        Window::getWindowDimensions(void) const noexcept
    {
        return windowImpl->getWindowDimensions();
    }

    [[nodiscard]] std::string const&
        Window::getWindowTitle(void) const noexcept
    {
        return windowImpl->getWindowTitle();
    }

    void Window::closeWindow(void) noexcept {
        windowImpl->closeWindow();
    }

    void Window::openWindow(void) noexcept {
        windowImpl->openWindow();
    }

    void Window::setPosition(Vector2u const& position) noexcept {
        windowImpl->setPosition(position);
    }

    void Window::minimize(void) noexcept {
        windowImpl->minimize();
    }

    void Window::maximize(void) noexcept {
        windowImpl->maximize();
    }

    void Window::setContextWindow(void) noexcept {
        windowImpl->setContextWindow();
        context.shaders.setLibrary(shaders);
    }

    void Window::draw2DDrawables(void) const noexcept {
        std::ranges::for_each(drawables2D, [](auto const& drawable)
            { drawable->draw(); });
    }

    void Window::draw3DDrawables(void) const noexcept {
        glEnable(GL_DEPTH_TEST);
        std::ranges::for_each(drawables3D, [](auto const& drawable)
            { drawable->draw(); });
        glDisable(GL_DEPTH_TEST);
    }

    void Window::clear(const Color& color) noexcept{
        windowImpl->clear(color);
        lastTime = ThreadClock::now();
    }

    void Window::draw(void) noexcept {
        windowImpl->draw();
        std::this_thread::sleep_until(lastTime + sleepTime);
    }

    void Window::setFPSLimit(std::size_t fpsLimit) noexcept {
        sleepTime = fpsLimit ? (1'000'000us / fpsLimit) :
            (0us / std::size_t(1));
        // changes the std::duration type
    }

    void Window::setTickrate(std::size_t ticks) noexcept {
        using std::operator""ms;
        eventManager->setPeriod(ticks ? TickRegister::Duration{1'000ms}
            / ticks : 0ms);
    }

    void Window::windowLoop(const Color& background) noexcept {
        windowImpl->openWindow();
        while (!windowImpl->shouldWindowClose()) {
            windowImpl->clear(background);
            eventManager->onTick();
            draw2DDrawables();
            windowImpl->draw();
        }
    }

    Image Window::saveWindowScreen(void) const {
        return windowImpl->saveWindowScreen();
    }

    void Window::setVPMatrix(CameraPtr const& cameraPtr) noexcept {
        context.setViewProjection(context.projection *
            (*cameraPtr)());
    }

    Window::~Window(void) noexcept {
        context.shaders.removeLibrary();
    }

}

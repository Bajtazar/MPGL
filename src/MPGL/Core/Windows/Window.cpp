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

    Window::Window(
        Vector2u const& dimensions,
        String const& title,
        Options const& options,
        EventManagerPtr eventManager,
        Paths const& shaderDirectories)
            : WindowPlatform{dimensions, title, options,
                std::move(eventManager)},
            shaders{shaderDirectories}, sleepTime{0us}, lastTime{0us}
    {
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        context.shaders.setLibrary(shaders);
    }

    void Window::setContextWindow(void) noexcept {
        WindowPlatform::setContextWindow();
        context.shaders.setLibrary(shaders);
    }

    void Window::drawDrawables(void) const noexcept {
        std::ranges::for_each(drawables2D, [](const auto& drawable)
            { drawable->draw(); });
    }

    void Window::clear(const Color& color) noexcept{
        WindowPlatform::clear(color);
        lastTime = ThreadClock::now();
    }

    void Window::draw(void) noexcept {
        WindowPlatform::draw();
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
        openWindow();
        while (!shouldWindowClose()) {
            clear(background);
            eventManager->onTick();
            drawDrawables();
            draw();
        }
    }

    Image Window::saveWindowScreen(void) const {
        Image image{getWindowDimensions()};
        glReadPixels(0, 0, image.getWidth(), image.getHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        return image;
    }

    Window::~Window(void) noexcept {
        context.shaders.removeLibrary();
    }

}

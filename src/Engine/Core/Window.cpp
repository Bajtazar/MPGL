#include "Window.hpp"

#include <thread>

namespace mpgl {

    using std::operator""us;

    Window::Window(Vector2u const& dimensions,
        std::string const& title, Options const& options)
            : WindowInterface{dimensions, title, options},
            sleepTime{0us}, lastTime{0us}
    {
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        context.shaders.setLibrary(shaders,
            reinterpret_cast<void*>(this));
    }

    void Window::setContextWindow(void) noexcept {
        WindowInterface::setContextWindow();
        context.shaders.setLibrary(shaders,
            reinterpret_cast<void*>(this));
    }

    void Window::copyDrawablesToGPU(void) const noexcept {
        std::ranges::for_each(drawables, [](auto const& drawable)
            { drawable->copyToGPU(); });
    }

    void Window::drawDrawables(void) const noexcept {
        std::ranges::for_each(drawables, [](const auto& drawable)
            { drawable->draw(); });
    }

    void Window::clear(const Color& color) noexcept{
        WindowInterface::clear(color);
        lastTime = ThreadClock::now();
    }

    void Window::draw(void) noexcept {
        WindowInterface::draw();
        std::this_thread::sleep_until(lastTime + sleepTime);
    }

    bool Window::setFPSLimit(std::size_t fpsLimit) noexcept {
        if (fpsLimit) {
            sleepTime = 1'000'000us / fpsLimit;
            return true;
        }
        return false;
    }

    void Window::setTickrate(std::size_t ticks) noexcept {
        using std::operator""ms;
        get<TickRegister>(events).setPeriod(
            ticks ? TickRegister::Duration{1'000ms} / ticks : 0ms);
    }

    int32 Window::windowLoop(const Color& background) noexcept {
        openWindow();
        copyDrawablesToGPU();
        while (!shouldWindowClose()) {
            clear(background);
            get<TickRegister>(events).onEvent();
            drawDrawables();
            draw();
        }
        return 0;
    }

    Window::~Window(void) noexcept {
        context.shaders.removeLibrary(
            reinterpret_cast<void*>(this));
    }

}
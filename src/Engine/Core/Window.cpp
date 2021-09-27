#include "Window.hpp"

#include <thread>

namespace ge {

    using std::operator""us;

    Window::Window(Vector2ui const& dimmensions,
        std::string const& title, Options options, GLFWmonitor* monitor,
        GLFWwindow* share)
        : WindowInterface{dimmensions, title, options, monitor, share},
        sleepTime{0us}, lastTime{0us}
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void Window::setDrawablesShaders(void) noexcept {
        std::ranges::for_each(drawables, [this](auto& drawable)
            { drawable->setShaders(this->shaders); });
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

    int32_t Window::windowLoop(const Color& background) noexcept {
        setDrawablesShaders();
        copyDrawablesToGPU();
        while (!shouldWindowClose()) {
            clear(background);
            get<TickRegister>(events).onEvent();
            drawDrawables();
            draw();
        }
        return 0;
    }

}
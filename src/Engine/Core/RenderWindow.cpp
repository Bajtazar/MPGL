#include "RenderWindow.hpp"

#include <thread>

namespace ge {

    using std::operator""us;

    RenderWindow::RenderWindow(Vector2ui const& dimmensions,
        std::string const& title, Options options, GLFWmonitor* monitor,
        GLFWwindow* share)
        : WindowInterface{dimmensions, title, options, monitor, share},
        sleepTime{0us}, lastTime{0us}
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void RenderWindow::setDrawablesShaders(void) noexcept {
        std::ranges::for_each(drawables, [this](auto& drawable)
            { drawable->setShaders(this->shaders); });
    }

    void RenderWindow::copyDrawablesToGPU(void) const noexcept {
        std::ranges::for_each(drawables, [](auto const& drawable)
            { drawable->copyToGPU(); });
    }

    void RenderWindow::drawDrawables(void) const noexcept {
        std::ranges::for_each(drawables, [](const auto& drawable)
            { drawable->draw(); });
    }

    void RenderWindow::clear(const Color& color) noexcept{
        WindowInterface::clear(color);
        lastTime = ThreadClock::now();
    }

    void RenderWindow::draw(void) noexcept {
        WindowInterface::draw();
        std::this_thread::sleep_until(lastTime + sleepTime);
    }

    bool RenderWindow::setFPSLimit(std::size_t fpsLimit) noexcept {
        if (fpsLimit) {
            sleepTime = 1'000'000us / fpsLimit;
            return true;
        }
        return false;
    }

    void RenderWindow::setTickrate(std::size_t ticks) noexcept {
        using std::operator""ms;
        tickRegister.setPeriod(ticks ? TickRegister::Duration{1'000ms} / ticks : 0ms);
    }

    int32_t RenderWindow::windowLoop(const Color& background) noexcept {
        setDrawablesShaders();
        copyDrawablesToGPU();
        while (!shouldWindowClose()) {
            clear(background);
            tickRegister.onEvent();
            drawDrawables();
            draw();
        }
        return 0;
    }

}
#include "RenderWindow.hpp"

#include <thread>

namespace ge {

    using std::operator""us;

    RenderWindow::RenderWindow(const Vector2i& dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share) noexcept(false) : WindowInterface{dimmensions, title, options, monitor, share}, sleepTime{0us}, lastTime{0us} {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void RenderWindow::pushDrawable(const std::shared_ptr<Drawable>& drawable) noexcept {
        if (auto ptr = std::dynamic_pointer_cast<Transformable>(drawable))
            transformables.push_back(ptr);
        drawables.push_back(drawable);
    }

    void RenderWindow::pushDrawable(std::shared_ptr<Drawable>&& drawable) noexcept {
        if (auto ptr = std::dynamic_pointer_cast<Transformable>(drawable))
            transformables.push_back(ptr);
        drawables.push_back(std::move(drawable));
    }

    void RenderWindow::setDrawablesShaders(void) noexcept {
        std::ranges::for_each(drawables, [this](auto& drawable){ drawable->setShaders(this->shaders); });
    }

    void RenderWindow::copyDrawablesToGPU(void) noexcept {
        std::ranges::for_each(drawables, [](auto& drawable){ drawable->copyToGPU(); });
    }

    void RenderWindow::drawDrawables(void) const noexcept {
        std::ranges::for_each(drawables, [](const auto& drawable){ drawable->draw(); });
    }

    void RenderWindow::clear(const Color& color) noexcept{
        WindowInterface::clear(color);
        lastTime = std::chrono::steady_clock::now();
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

    int32_t RenderWindow::windowLoop(const Color& background) noexcept {
        setDrawablesShaders();
        copyDrawablesToGPU();
        while (!(*this)) {
            clear(background);
            drawDrawables();
            draw();
        }
        return 0;
    }

}
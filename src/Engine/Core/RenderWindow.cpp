#include "RenderWindow.hpp"

namespace ge {

    RenderWindow::RenderWindow(const Vector2i& dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share) noexcept(false) : WindowInterface(dimmensions, title, options, monitor, share){

    }

    void RenderWindow::addDrawable(std::unique_ptr<Drawable>&& drawable) noexcept {
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
    }

    void RenderWindow::draw(void) noexcept {
        WindowInterface::draw();
    }

    int32_t RenderWindow::windowLoop(void) noexcept {
        setDrawablesShaders();
        copyDrawablesToGPU();

        while (!(*this)) {
            clear();
            drawDrawables();
            draw();
        }

        return 0;
    }

}
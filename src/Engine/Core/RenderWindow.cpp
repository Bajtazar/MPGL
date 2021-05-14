#include "RenderWindow.hpp"

namespace ge {

    RenderWindow::RenderWindow(const Vector2i& dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share) noexcept(false) : WindowInterface(dimmensions, title, options, monitor, share){

    }

    void RenderWindow::clear(const Color& color) noexcept{
        WindowInterface::clear(color);
    }

    void RenderWindow::draw(void) noexcept {
        WindowInterface::draw();
    }

}
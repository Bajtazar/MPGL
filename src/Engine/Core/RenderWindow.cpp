#include "RenderWindow.hpp"
#include "../Exceptions/RenderWindowInitException.hpp"
#include "../Exceptions/RenderWindowInvalidArgsException.hpp"

namespace ge {

    RenderWindow::RenderWindow(int32_t width, int32_t height, std::string title, GLFWmonitor* monitor, GLFWwindow* share) noexcept(false) {
        if (!glfwInit())
            throw RenderWindowInitException(title);
        windowHandler = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        if (!windowHandler)
            throw RenderWindowInvalidArgsException(title);
        glfwMakeContextCurrent(windowHandler);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    RenderWindow::RenderWindow(RenderWindow&& window) noexcept : windowHandler(window.windowHandler) {
        window.windowHandler = nullptr;
    }

    RenderWindow& RenderWindow::operator= (RenderWindow&& window) noexcept{
        windowHandler = window.windowHandler;
        window.windowHandler = nullptr;
        return *this;
    }

    void RenderWindow::clear(void) noexcept{
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderWindow::draw(void) noexcept {
        glfwSwapBuffers(windowHandler);
        glfwPollEvents();
    }

}
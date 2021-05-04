#include "RenderWindow.hpp"
#include "../Exceptions/RenderWindowInitException.hpp"
#include "../Exceptions/RenderWindowInvalidArgsException.hpp"
#include "../Exceptions/RenderWindowGladException.hpp"

namespace ge {

    RenderWindow::Options::Options(uint16_t major,uint16_t minor,bool floating,bool maximised,bool resizable) noexcept
    : openGLMajorVersion(major), openGLMinorVersion(minor), floating(floating), maximised(maximised), resizable(resizable) {}

    RenderWindow::RenderWindow(int32_t width, int32_t height, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share) noexcept(false) {
        if (!glfwInit())
            throw RenderWindowInitException(title);
        setWindowOptions(options);
        windowHandler = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        if (!windowHandler)
            throw RenderWindowInvalidArgsException(title);
        glfwMakeContextCurrent(windowHandler);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw RenderWindowGladException(title);
        glViewport(0, 0, width, height);
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

    void RenderWindow::setWindowOptions(const Options& options) const noexcept {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.openGLMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.openGLMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, options.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, options.maximised);
        glfwWindowHint(GLFW_FLOATING, options.floating);
    }

}
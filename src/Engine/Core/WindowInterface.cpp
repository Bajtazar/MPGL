#include "WindowInterface.hpp"
#include "../Exceptions/RenderWindowInitException.hpp"
#include "../Exceptions/RenderWindowInvalidArgsException.hpp"
#include "../Exceptions/RenderWindowGladException.hpp"

namespace ge {

    WindowInterface::Options::Options(uint16_t major,uint16_t minor,bool floating,bool maximised,bool resizable) noexcept
    : openGLMajorVersion(major), openGLMinorVersion(minor), floating(floating), maximised(maximised), resizable(resizable) {}

    WindowInterface::WindowInterface(Vector2i dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share)
    : dimmensions(dimmensions), baseDimmensions(dimmensions) {
        if (!glfwInit())
            throw RenderWindowInitException(title);
        setWindowOptions(options);
        window = glfwCreateWindow(get<0>(dimmensions), get<1>(dimmensions), title.c_str(), monitor, share);
        glfwMakeContextCurrent(window);
        if (!window)
            throw RenderWindowInvalidArgsException(title);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw RenderWindowGladException(title);
        setCallbacks();
    }

    void framebufferCallback(GLFWwindow* window, int32_t width, int32_t height) noexcept {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        render->dimmensions = Vector<int32_t, int32_t>{width, height};
    }

    void WindowInterface::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
    }

    void WindowInterface::setWindowOptions(const Options& options) const noexcept {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.openGLMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.openGLMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, options.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, options.maximised);
        glfwWindowHint(GLFW_FLOATING, options.floating);
    }

    void WindowInterface::draw(void) const noexcept {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void WindowInterface::clear(const Color& color) const noexcept {
        glClearColor(color.red, color.green, color.blue, color.alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}

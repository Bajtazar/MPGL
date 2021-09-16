#include "WindowInterface.hpp"

#include "../Exceptions/RenderWindowInvalidArgsException.hpp"
#include "../Exceptions/RenderWindowInitException.hpp"
#include "../Exceptions/RenderWindowGladException.hpp"

namespace ge {

    WindowInterface::Options::Options(uint16_t major,uint16_t minor,bool floating,bool maximised,bool resizable) noexcept
    : openGLMajorVersion(major), openGLMinorVersion(minor), floating(floating), maximised(maximised), resizable(resizable) {}

    WindowInterface::WindowInterface(Vector2ui dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share)
    : dimmensions{std::move(std::make_shared<Vector2ui>(dimmensions))} {
        if (!glfwInit())
            throw RenderWindowInitException(title);
        setWindowOptions(options);
        window = glfwCreateWindow(dimmensions[0], dimmensions[1], title.c_str(), monitor, share);
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
        Vector2ui oldDimmensions = *(render->dimmensions);
        *(render->dimmensions) = Vector2ui{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        std::ranges::for_each(render->transformables, [&oldDimmensions](auto& ptr){ ptr->onScreenTransformation(oldDimmensions); });
    }

    void keyCallback(GLFWwindow* window, int32_t key,
        [[maybe_unused]] int32_t scancode, int32_t action,
        [[maybe_unused]] int32_t mods)
    {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        auto keyCode = static_cast<Key>(static_cast<int16_t>(key));
        if (action == GLFW_PRESS)
            get<KeyPressRegister>(render->events).onEvent(keyCode);
        else if (action == GLFW_RELEASE)
            get<KeyReleaseRegister>(render->events).onEvent(keyCode);
    }

    void WindowInterface::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
        glfwSetKeyCallback(window, keyCallback);
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

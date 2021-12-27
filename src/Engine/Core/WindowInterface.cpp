#include "WindowInterface.hpp"

#include "../Exceptions/RenderWindowInvalidArgsException.hpp"
#include "../Exceptions/RenderWindowGladException.hpp"
#include "Text/UTF-8.hpp"

namespace ge {

    WindowInterface::Options::Options(uint16_t major,uint16_t minor,bool floating,bool maximised,bool resizable) noexcept
    : openGLMajorVersion(major), openGLMinorVersion(minor), floating(floating), maximised(maximised), resizable(resizable) {}

    WindowInterface::WindowInterface(Vector2ui dimmensions, std::string title, Options options, GLFWmonitor* monitor, GLFWwindow* share)
        : dimmensions{dimmensions}
    {
        context.windowDimmensions = dimmensions;
        setWindowOptions(options);
        window = glfwCreateWindow(dimmensions[0], dimmensions[1], title.c_str(), monitor, share);
        glfwMakeContextCurrent(window);
        if (!window)
            throw RenderWindowInvalidArgsException{title};
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw RenderWindowGladException{title};
        setCallbacks();
    }

    void WindowInterface::setContextWindow(void) noexcept {
        glfwMakeContextCurrent(window);
        context.windowDimmensions = dimmensions;
    }

    void WindowInterface::setDimmensions(Vector2ui const& dimm) noexcept {
        context.windowDimmensions = dimmensions = dimm;
    }

    void framebufferCallback(GLFWwindow* window, int32_t width, int32_t height) noexcept {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        Vector2ui oldDimmensions = render->dimmensions;
        render->setDimmensions(vectorCast<uint32_t>(Vector2i{width, height}));
        get<ScreenTransformationRegister>(render->events).onEvent(oldDimmensions);
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

    void mouseButtonCallback(GLFWwindow* window, int32_t button,
        int32_t action, [[maybe_unused]] int32_t mods)
    {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        auto buttonCode = static_cast<MouseButton>(static_cast<uint8_t>(button));
        if (action == GLFW_PRESS)
            get<MousePressRegister>(render->events).onEvent(buttonCode);
        else if (action == GLFW_RELEASE)
            get<MouseReleaseRegister>(render->events).onEvent(buttonCode);
    }

    void textCallback(GLFWwindow* window, uint32_t character) {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        get<TextWriteRegister>(render->events).onEvent(toUTF8(character));
    }

    void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
        WindowInterface* render = static_cast<WindowInterface*>(glfwGetWindowUserPointer(window));
        get<MouseMotionRegister>(render->events).onEvent(Vector2f{xpos, ypos});
    }

    void WindowInterface::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, textCallback);
        glfwSetCursorPosCallback(window, mousePosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
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
        glClearColor(color.red(), color.green(),
            color.blue(), color.alpha());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    WindowInterface::~WindowInterface(void) noexcept {
        glfwDestroyWindow(window);
    }

}

#include "WindowInterface.hpp"

#include "../Exceptions/RenderWindowInvalidArgsException.hpp"
#include "../Exceptions/RenderWindowGladException.hpp"
#include "Text/UTF-8.hpp"

namespace mpgl {

    WindowInterface::WindowInterface(Vector2u dimensions,
        std::string title, Options const& options)
        : dimensions{dimensions}, options{options}
    {
        setWindowOptions();
        window = glfwCreateWindow(dimensions[0], dimensions[1],
            title.c_str(), nullptr, nullptr);
        setContextWindow();
        if (!window)
            throw RenderWindowInvalidArgsException{title};
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw RenderWindowGladException{title};
        setCallbacks();
    }

    void WindowInterface::setContextWindow(void) noexcept {
        glfwMakeContextCurrent(window);
        setCommunicationThread(dimensions, options);
    }

    void WindowInterface::setDimensions(Vector2u const& dimm) noexcept {
        context.windowDimensions = dimensions = dimm;
    }

    void framebufferCallback(GLFWwindow* window, int32 width,
        int32 height) noexcept
    {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        Vector2u oldDimensions = render->dimensions;
        render->setDimensions(vectorCast<uint32>(
            Vector2i{width, height}));
        get<ScreenTransformationRegister>(render->events).onEvent(
            oldDimensions);
    }

    void keyCallback(GLFWwindow* window, int32 key,
        [[maybe_unused]] int32 scancode, int32 action,
        [[maybe_unused]] int32 mods) noexcept
    {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        auto keyCode = static_cast<Key>(static_cast<int16>(key));
        if (action == GLFW_PRESS)
            get<KeyPressRegister>(render->events).onEvent(keyCode);
        else if (action == GLFW_RELEASE)
            get<KeyReleaseRegister>(render->events).onEvent(keyCode);
    }

    void mouseButtonCallback(GLFWwindow* window, int32 button,
        int32 action, [[maybe_unused]] int32 mods) noexcept
    {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        auto buttonCode = static_cast<MouseButton>(
            static_cast<uint8>(button));
        if (action == GLFW_PRESS)
            get<MousePressRegister>(render->events).onEvent(
                buttonCode);
        else if (action == GLFW_RELEASE)
            get<MouseReleaseRegister>(render->events).onEvent(
                buttonCode);
    }

    void textCallback(GLFWwindow* window, uint32 character) noexcept {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        get<TextWriteRegister>(render->events).onEvent(
            toUTF8(character));
    }

    void windowCloseCallback(GLFWwindow* window) noexcept {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        get<WindowCloseRegister>(render->events).onEvent();
    }

    void mousePosCallback(GLFWwindow* window, float64 xpos,
        float64 ypos) noexcept
    {
        WindowInterface* render = static_cast<WindowInterface*>(
            glfwGetWindowUserPointer(window));
        get<MouseMotionRegister>(render->events).onEvent(
            Vector2f{xpos, ypos});
    }

    void WindowInterface::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, textCallback);
        glfwSetCursorPosCallback(window, mousePosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
    }

    void WindowInterface::setWindowOptions(void) const noexcept {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
            options.openGLMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
            options.openGLMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, options.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, options.maximised);
        glfwWindowHint(GLFW_FLOATING, options.floating);
        glfwWindowHint(GLFW_DECORATED, !options.clean);
        glfwWindowHint(GLFW_SAMPLES, options.antiAliasingSamples);
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

    void WindowInterface::openWindow(void) noexcept {
        glfwSetWindowShouldClose(window, false);
    }

    void WindowInterface::closeWindow(void) noexcept {
        glfwSetWindowShouldClose(window, true);
    }

}

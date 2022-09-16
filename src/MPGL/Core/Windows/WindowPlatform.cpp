/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include <MPGL/Exceptions/Window/WindowInvalidArgsException.hpp>
#include <MPGL/Exceptions/Window/WindowGladException.hpp>
#include <MPGL/Core/Windows/WindowPlatform.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>

namespace mpgl {

    WindowPlatform::WindowPlatform(
        Vector2u dimensions,
        std::string title,
        Options const& options,
        EventManagerPtr&& eventManager)
            : WindowBase{std::move(eventManager)},
            dimensions{dimensions}, options{options},
            title{title}, window{nullptr}
    {
        setWindowOptions();
        window = glfwCreateWindow(dimensions[0], dimensions[1],
            title.c_str(), nullptr, nullptr);
        setContextWindow();
        if (!window)
            throw WindowInvalidArgsException{title};
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw WindowGladException{title};
        setCallbacks();
    }

    void WindowPlatform::setContextWindow(void) noexcept {
        glfwMakeContextCurrent(window);
        const_cast<Vector2u&>(context.windowDimensions) = dimensions;
        const_cast<Options&>(context.windowOptions) = options;
    }

    void WindowPlatform::setDimensions(Vector2u const& dim) noexcept {
        const_cast<Vector2u&>(context.windowDimensions)
            = dimensions = dim;
    }

    void WindowPlatform::framebufferCallback(
        GLFWwindow* window,
        int32 width,
        int32 height) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        Vector2u oldDimensions = render->dimensions;
        render->setDimensions(vectorCast<uint32>(
            Vector2i{width, height}));
        render->eventManager->onScreenTransformation(oldDimensions);
    }

    void WindowPlatform::windowPositionCallback(
        GLFWwindow *window,
        int32 xPos,
        int32 yPos) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        Vector2u oldPosition = render->context.windowPosition;
        const_cast<Vector2u&>(render->context.windowPosition)
            = Vector2u{xPos, yPos};
        const_cast<Vector2u&>(render->context.absoluteMousePosition)
            = render->context.windowPosition + render->context.relativeMousePosition;
        render->eventManager->onWindowMotion(oldPosition);
    }

    void WindowPlatform::keyCallback(
        GLFWwindow* window,
        int32 key,
        [[maybe_unused]] int32 scancode,
        int32 action,
        [[maybe_unused]] int32 mods) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        auto keyCode = static_cast<Key>(static_cast<int16>(key));
        if (action == GLFW_PRESS)
            render->eventManager->onKeyPress(keyCode);
        else if (action == GLFW_RELEASE)
            render->eventManager->onKeyRelease(keyCode);
    }

    void WindowPlatform::mouseButtonCallback(
        GLFWwindow* window,
        int32 button,
        int32 action,
        [[maybe_unused]] int32 mods) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        auto buttonCode = static_cast<MouseButton>(
            static_cast<uint8>(button));
        if (action == GLFW_PRESS)
            render->eventManager->onMousePress(buttonCode);
        else if (action == GLFW_RELEASE)
            render->eventManager->onMouseRelease(buttonCode);
    }

    void WindowPlatform::textCallback(
        GLFWwindow* window,
        uint32 character) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        render->eventManager->onTextWrite(toUTF8(character));
    }

    void WindowPlatform::windowCloseCallback(
        GLFWwindow* window) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        render->eventManager->onWindowClose();
    }

    void WindowPlatform::mousePosCallback(
        GLFWwindow* window,
        float64 xpos,
        float64 ypos) noexcept
    {
        WindowPlatform* render = static_cast<WindowPlatform*>(
            glfwGetWindowUserPointer(window));
        Vector2d position{xpos, ypos};
        render->eventManager->onMouseMotion(position);
        const_cast<Vector2u&>(context.relativeMousePosition)
            = vectorCast<uint32>(position);
        const_cast<Vector2u&>(render->context.absoluteMousePosition)
            = render->context.windowPosition + render->context.relativeMousePosition;
    }

    void WindowPlatform::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, textCallback);
        glfwSetCursorPosCallback(window, mousePosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
        glfwSetWindowPosCallback(window, windowPositionCallback);
    }

    void WindowPlatform::setWindowOptions(void) const noexcept {
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

    void WindowPlatform::draw(void) const noexcept {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void WindowPlatform::clear(const Color& color) const noexcept {
        if (options.cleaningOptions != CleaningOptions::None) {
            glClearColor(color.red(), color.green(),
                color.blue(), color.alpha());
            // change to std::to_underlying in C++23
            glClear(static_cast<uint32>(options.cleaningOptions));
        }
    }

    WindowPlatform::~WindowPlatform(void) noexcept {
        glfwDestroyWindow(window);
    }

    void WindowPlatform::openWindow(void) noexcept {
        glfwSetWindowShouldClose(window, false);
    }

    void WindowPlatform::closeWindow(void) noexcept {
        glfwSetWindowShouldClose(window, true);
    }

    void WindowPlatform::setPosition(
        Vector2u const& position) noexcept
    {
        glfwSetWindowPos(window, position[0], position[1]);
    }

    void WindowPlatform::minimize(void) noexcept {
        glfwIconifyWindow(window);
    }

    void WindowPlatform::maximize(void) noexcept {
        glfwRestoreWindow(window);
    }

}

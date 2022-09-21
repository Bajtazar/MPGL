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
#include <MPGL/Platform/Features/Windows/GLFWWindow.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>

#include <glad/glad.h>

namespace mpgl::platform {

    GLFWWindow::GLFWWindow(
        Vector2u dimensions,
        std::string title,
        Options const& options)
            : WindowPlatform{dimensions, title, options},
            window{nullptr}
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
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void GLFWWindow::closeWindow(void) noexcept {
        glfwSetWindowShouldClose(window, true);
    }

    void GLFWWindow::openWindow(void) noexcept {
        glfwSetWindowShouldClose(window, false);
    }

    void GLFWWindow::setPosition(Vector2u const& position) noexcept {
        glfwSetWindowPos(window, position[0], position[1]);
    }

    void GLFWWindow::minimize(void) noexcept {
        glfwIconifyWindow(window);
    }

    void GLFWWindow::maximize(void) noexcept {
        glfwRestoreWindow(window);
    }

    [[nodiscard]] bool GLFWWindow::shouldWindowClose(
        void) const noexcept
    {
        return glfwWindowShouldClose(window);
    }

    void GLFWWindow::clear(Color const& color) const noexcept {
        if (options.cleaningOptions != CleaningOptions::None) {
            glClearColor(color.red(), color.green(),
                color.blue(), color.alpha());
            // change to std::to_underlying in C++23
            glClear(static_cast<uint32>(options.cleaningOptions));
        }
    }

    void GLFWWindow::draw(void) const noexcept {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    [[nodiscard]] Image GLFWWindow::saveWindowScreen(void) const {
        Image image{getWindowDimensions()};
        glReadPixels(0, 0, image.getWidth(), image.getHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        return image;
    }

    void GLFWWindow::setContextWindow(void) noexcept {
        glfwMakeContextCurrent(window);
        const_cast<Vector2u&>(context.windowDimensions) = dimensions;
        const_cast<Options&>(context.windowOptions) = options;
    }

    void GLFWWindow::setWindowOptions(void) const noexcept {
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
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, options.transparent);
    }

    void GLFWWindow::setCallbacks(void) noexcept {
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, textCallback);
        glfwSetCursorPosCallback(window, mousePosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
        glfwSetWindowPosCallback(window, windowPositionCallback);
    }

    GLFWWindow::~GLFWWindow(void) noexcept {
        glfwDestroyWindow(window);
    }

    void GLFWWindow::framebufferCallback(
        GLFWwindow *window,
        int32 width,
        int32 height) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        Vector2u oldDimensions = render->dimensions;
        render->setDimensions(vectorCast<uint32>(
            Vector2i{width, height}));
        render->eventManager->onScreenTransformation(oldDimensions);
    }

    void GLFWWindow::windowPositionCallback(
        GLFWwindow *window,
        int32 xPos,
        int32 yPos) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        Vector2u oldPosition = render->context.windowPosition;
        const_cast<Vector2u&>(render->context.windowPosition)
            = Vector2u{xPos, yPos};
        actualizeAbsolutePosition();
        render->eventManager->onWindowMotion(oldPosition);
    }

    void GLFWWindow::keyCallback(
        GLFWwindow *window,
        int32 key,
        [[maybe_unused]] int32 scancode,
        int32 action,
        [[maybe_unused]] int32 mods) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        auto keyCode = static_cast<Key>(static_cast<int16>(key));
        if (action == GLFW_PRESS)
            render->eventManager->onKeyPress(keyCode);
        else if (action == GLFW_RELEASE)
            render->eventManager->onKeyRelease(keyCode);
    }

    void GLFWWindow::textCallback(
        GLFWwindow *window,
        uint32 character) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        render->eventManager->onTextWrite(toUTF8(character));
    }

    void GLFWWindow::mousePosCallback(
        GLFWwindow *window,
        float64 xpos,
        float64 ypos) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        Vector2f oldPosition = context.relativeMousePosition;
        const_cast<Vector2u&>(context.relativeMousePosition)
            = Vector2u{xpos, context.windowDimensions[1] - ypos};
        actualizeAbsolutePosition();
        render->eventManager->onMouseMotion(oldPosition);
    }

    void GLFWWindow::mouseButtonCallback(
        GLFWwindow *window,
        int32 button,
        int32 action,
        [[maybe_unused]] int32 mods) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        auto buttonCode = static_cast<MouseButton>(
            static_cast<uint8>(button));
        if (action == GLFW_PRESS)
            render->eventManager->onMousePress(buttonCode);
        else if (action == GLFW_RELEASE)
            render->eventManager->onMouseRelease(buttonCode);
    }

    void GLFWWindow::windowCloseCallback(
        GLFWwindow *window) noexcept
    {
        GLFWWindow* render = static_cast<GLFWWindow*>(
            glfwGetWindowUserPointer(window));
        render->eventManager->onWindowClose();
    }

    void GLFWWindow::actualizeAbsolutePosition(void) noexcept {
        const_cast<Vector2u&>(context.absoluteMousePosition)
            = context.windowPosition +
            Vector2u{context.relativeMousePosition[0],
            context.windowDimensions[1] -
                context.relativeMousePosition[1]};
    }

}

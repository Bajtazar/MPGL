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
#include "FramedWindow.hpp"

#include "../../Exceptions/FramedWindowCompileException.hpp"

namespace mpgl {

    FramedWindow::FramedWindow(Options const& options)
        : windowTexture{options}
    {
        glGenFramebuffers(1, &framebuffer);
        glGenRenderbuffers(1, &renderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        finishTextureSetting(options);
        bindDepthAndStencil();
        bindRenderbuffer();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
            != GL_FRAMEBUFFER_COMPLETE)
                throw FramedWindowCompileException{};
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FramedWindow::finishTextureSetting(
        Options const& options) noexcept
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            context.windowDimensions[0],
            context.windowDimensions[1], 0, GL_RGBA,
            GL_UNSIGNED_BYTE, NULL);
        if (options.mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, windowTexture.getTexture(), 0);
    }

    void FramedWindow::bindDepthAndStencil(void) noexcept {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
            windowTexture.getTexture(), 0);
    }

    void FramedWindow::bindRenderbuffer(void) noexcept {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            context.windowDimensions[0],
            context.windowDimensions[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
            renderbuffer);
    }

    FramedWindow::FramedWindow(FramedWindow&& window) noexcept
        : windowTexture{std::move(window.windowTexture)},
        framebuffer{window.framebuffer},
        renderbuffer{window.renderbuffer}
    {
        events = std::move(window.events);
        window.framebuffer = window.renderbuffer = 0;
    }

    FramedWindow&
        FramedWindow::operator=(FramedWindow&& window) noexcept
    {
        this->~FramedWindow();
        windowTexture = std::move(window.windowTexture);
        framebuffer = window.framebuffer;
        renderbuffer = window.renderbuffer;
        events = std::move(window.events);
        window.framebuffer = window.renderbuffer = 0;
        return *this;
    }

    void FramedWindow::bind(Color const& color) noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(color.red(), color.green(), color.blue(),
            color.alpha());
        glClear(ClearMask);
    }

    void FramedWindow::unbind(void) noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FramedWindow::render(Color const& color) noexcept {
        bind(color);
        std::ranges::for_each(drawables,
            [](auto const& drawable){ drawable->copyToGPU(); });
        std::ranges::for_each(drawables,
            [](auto const& drawable){ drawable->draw(); });
        unbind();
    }

    void FramedWindow::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        get<ScreenTransformationRegister>(events).onEvent(
            oldDimensions);
    }

    void FramedWindow::onMouseRelease(
        MouseButton const& button) noexcept
    {
        get<MouseReleaseRegister>(events).onEvent(button);
    }

    void FramedWindow::onWindowClose(void) noexcept {
        get<WindowCloseRegister>(events).onEvent();
    }

    void FramedWindow::onMouseMotion(
        Vector2f const& position) noexcept
    {
        get<MouseMotionRegister>(events).onEvent(position);
    }

    void FramedWindow::onMousePress(
        MouseButton const& button) noexcept
    {
        get<MousePressRegister>(events).onEvent(button);
    }

    void FramedWindow::onKeyRelease(Key const& key) noexcept {
        get<KeyReleaseRegister>(events).onEvent(key);
    }

    void FramedWindow::onTextWrite(
        std::string const& unicodeString) noexcept
    {
        get<TextWriteRegister>(events).onEvent(unicodeString);
    }

    void FramedWindow::onKeyPress(Key const& key) noexcept {
        get<KeyPressRegister>(events).onEvent(key);
    }

    void FramedWindow::onScroll(Vector2f const& scroll) noexcept {
        get<ScrollRegistry>(events).onEvent(scroll);
    }

    void FramedWindow::onTick(
        std::chrono::milliseconds const& duration) noexcept
    {
        std::ranges::for_each(get<TickRegister>(events),
            [&duration](auto const& drawable)
            { drawable->onTick(duration); });
    }

    FramedWindow::~FramedWindow(void) noexcept {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteRenderbuffers(1, &renderbuffer);
    }

}

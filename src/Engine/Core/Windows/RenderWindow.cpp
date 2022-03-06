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
#include "RenderWindow.hpp"

#include "../../Exceptions/FramedWindowCompileException.hpp"

namespace mpgl {

    RenderWindow::RenderWindow(Options const& options)
        : windowTexture{options}
    {
        glGenFramebuffers(1, &framebuffer);
        glGenRenderbuffers(1, &renderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        auto const& buffer = windowTexture.getTextureBuffer();
        finishTextureSetting(buffer, options);
        buffer.connectToDepthAndStencilBuffer();
        bindRenderbuffer();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
            != GL_FRAMEBUFFER_COMPLETE)
                throw FramedWindowCompileException{};
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderWindow::finishTextureSetting(
        TextureBuffer const& buffer,
        Options const& options) noexcept
    {
        buffer.loadImage(TextureBuffer::PixelFormat::RGBA,
            context.windowDimensions[0],
            context.windowDimensions[1], nullptr);
        if (options.mipmaps)
            buffer.generateMipmaps();
        buffer.connectToFramebuffer();
    }

    void RenderWindow::bindRenderbuffer(void) noexcept {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            context.windowDimensions[0],
            context.windowDimensions[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
            renderbuffer);
    }

    RenderWindow::RenderWindow(RenderWindow&& window) noexcept
        : windowTexture{std::move(window.windowTexture)},
        framebuffer{window.framebuffer},
        renderbuffer{window.renderbuffer}
    {
        events = std::move(window.events);
        window.framebuffer = window.renderbuffer = 0;
    }

    RenderWindow&
        RenderWindow::operator=(RenderWindow&& window) noexcept
    {
        this->~RenderWindow();
        windowTexture = std::move(window.windowTexture);
        framebuffer = window.framebuffer;
        renderbuffer = window.renderbuffer;
        events = std::move(window.events);
        window.framebuffer = window.renderbuffer = 0;
        return *this;
    }

    void RenderWindow::bind(
        CleaningOptions const& cleaning,
        [[maybe_unused]] Color const& color) noexcept
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        if (cleaning != CleaningOptions::None) {
            glClearColor(color.red(), color.green(), color.blue(),
                color.alpha());
            // change to std::to_underlying in C++23
            glClear(static_cast<uint32>(cleaning));
        }
    }

    void RenderWindow::unbind(void) noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderWindow::render(CleaningOptions const& cleaning,
        Color const& color) noexcept
    {
        bind(cleaning, color);
        std::ranges::for_each(drawables,
            [](auto const& drawable){ drawable->draw(); });
        unbind();
    }

    void RenderWindow::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        get<ScreenTransformationRegister>(events).onEvent(
            oldDimensions);
    }

    void RenderWindow::onMouseRelease(
        MouseButton const& button) noexcept
    {
        get<MouseReleaseRegister>(events).onEvent(button);
    }

    void RenderWindow::onWindowClose(void) noexcept {
        get<WindowCloseRegister>(events).onEvent();
    }

    void RenderWindow::onMouseMotion(
        Vector2f const& position) noexcept
    {
        get<MouseMotionRegister>(events).onEvent(position);
    }

    void RenderWindow::onMousePress(
        MouseButton const& button) noexcept
    {
        get<MousePressRegister>(events).onEvent(button);
    }

    void RenderWindow::onKeyRelease(Key const& key) noexcept {
        get<KeyReleaseRegister>(events).onEvent(key);
    }

    void RenderWindow::onTextWrite(
        std::string const& unicodeString) noexcept
    {
        get<TextWriteRegister>(events).onEvent(unicodeString);
    }

    void RenderWindow::onKeyPress(Key const& key) noexcept {
        get<KeyPressRegister>(events).onEvent(key);
    }

    void RenderWindow::onScroll(Vector2f const& scroll) noexcept {
        get<ScrollRegistry>(events).onEvent(scroll);
    }

    void RenderWindow::onTick(
        std::chrono::milliseconds const& duration) noexcept
    {
        std::ranges::for_each(get<TickRegister>(events),
            [&duration](auto const& drawable)
            { drawable->onTick(duration); });
    }

    RenderWindow::~RenderWindow(void) noexcept {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteRenderbuffers(1, &renderbuffer);
    }

}

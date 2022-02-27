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
#include "FrameBuffer.hpp"

namespace mpgl {

    FrameBuffer::FrameBuffer(void) noexcept
        : frameID{0}, renderID{0}
    {
        glGenFramebuffers(1, &frameID);
        glGenRenderbuffers(1, &renderID);
    }

    FrameBuffer::FrameBuffer(uint32 frameID, uint32 renderID) noexcept
        : frameID{frameID}, renderID{renderID} {}

    FrameBuffer::FrameBuffer(FrameBuffer&& buffer) noexcept
        : frameID{buffer.frameID}, renderID{buffer.renderID}
    {
        buffer.frameID = buffer.renderID = 0;
    }

    FrameBuffer& FrameBuffer::operator=(
        FrameBuffer&& buffer) noexcept
    {
        this->~FrameBuffer();
        frameID = buffer.frameID;
        renderID = buffer.renderID;
        buffer.frameID = buffer.renderID = 0;
        return *this;
    }

    void FrameBuffer::bind(void) const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, frameID);
    }

    void FrameBuffer::unbind(void) const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::connectRenderBuffer(void) const noexcept {
        glBindRenderbuffer(GL_RENDERBUFFER, renderID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            context.windowDimensions[0],
            context.windowDimensions[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderID);
    }

    void FrameBuffer::clear(CleaningOptions const& cleaning,
        Color const& color) const noexcept
    {
        if (cleaning == CleaningOptions::None)
            return;
        glClearColor(color.red(), color.green(), color.blue(),
            color.alpha());
        glClear(static_cast<uint32>(cleaning));
    }

    [[nodiscard]] bool FrameBuffer::status(void) const noexcept {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER)
            != GL_FRAMEBUFFER_COMPLETE;
    }

    FrameBuffer::~FrameBuffer(void) noexcept {
        if (frameID)
            glDeleteFramebuffers(1, &frameID);
        if (renderID)
            glDeleteRenderbuffers(1, &renderID);
    }

}

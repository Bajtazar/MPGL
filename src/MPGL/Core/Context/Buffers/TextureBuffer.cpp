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
#include <MPGL/Core/Context/Buffers/TextureBuffer.hpp>

namespace mpgl {

    TextureBuffer::TextureBuffer(void) noexcept
        : textureID{0}
    {
        glGenTextures(1, &textureID);
    }

    TextureBuffer::TextureBuffer(TextureBuffer&& buffer) noexcept
        : textureID{buffer.textureID}
    {
        buffer.textureID = 0;
    }

    TextureBuffer& TextureBuffer::operator=(
        TextureBuffer&& buffer) noexcept
    {
        this->~TextureBuffer();
        textureID = buffer.textureID;
        buffer.textureID = 0;
        return *this;
    }

    void TextureBuffer::loadImage(
        PixelFormat const& format,
        std::size_t width,
        std::size_t height,
        void const* imagePtr) const noexcept
    {
        // Change to std::to_underlying in C++23
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<uint16>(format),
            width, height, 0, static_cast<uint16>(format),
            GL_UNSIGNED_BYTE, imagePtr);
    }

    void TextureBuffer::generateMipmaps(void) const noexcept {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void TextureBuffer::setParameter(
        GLint filter,
        GLint mode) const noexcept
    {
        glTexParameteri(GL_TEXTURE_2D, filter, mode);
    }

    void TextureBuffer::setBorderColor(
        Color const& color) const noexcept
    {
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
            &color.red());
    }

    void TextureBuffer::connectToFramebuffer(void) const noexcept {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, textureID, 0);
    }

    void TextureBuffer::connectToDepthAndStencilBuffer(
        void) const noexcept
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
            textureID, 0);
    }

    void TextureBuffer::activate(void) const noexcept {
        glActiveTexture(GL_TEXTURE0);
    }

    void TextureBuffer::bind(void) const noexcept {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void TextureBuffer::unbind(void) const noexcept {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    TextureBuffer::~TextureBuffer(void) noexcept {
        if (textureID)
            glDeleteTextures(1, &textureID);
    }

}

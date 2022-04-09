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
#include <MPGL/Core/Context/Buffers/VertexBuffer.hpp>

namespace mpgl {

    VertexBuffer::VertexBuffer(void) noexcept
        : bufferID{0}
    {
        glGenBuffers(1, &bufferID);
    }

    VertexBuffer::VertexBuffer(uint32 buffer) noexcept
        : bufferID{buffer} {}

    VertexBuffer::VertexBuffer(VertexBuffer&& buffer) noexcept
        : bufferID{buffer.bufferID}
    {
        buffer.bufferID = 0;
    }

    VertexBuffer&
        VertexBuffer::operator=(VertexBuffer&& buffer) noexcept
    {
        this->~VertexBuffer();
        bufferID = buffer.bufferID;
        buffer.bufferID = 0;
        return *this;
    }

    void* VertexBuffer::bindMap(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void VertexBuffer::unbindMap(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void VertexBuffer::unbind(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::bind(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    }

    VertexBuffer::~VertexBuffer(void) noexcept {
        if (bufferID)
            glDeleteBuffers(1, &bufferID);
    }

}

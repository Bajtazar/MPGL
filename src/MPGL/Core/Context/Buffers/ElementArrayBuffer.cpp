/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>

namespace mpgl {

    ElementArrayBuffer::ElementArrayBuffer(void) noexcept
        : elementID{0}
    {
        glGenBuffers(1, &elementID);
    }

    ElementArrayBuffer::ElementArrayBuffer(uint32 elementID) noexcept
        : elementID{elementID} {}

    ElementArrayBuffer::ElementArrayBuffer(
        ElementArrayBuffer&& buffer) noexcept
            : elementID{std::exchange(buffer.elementID, 0)} {}

    ElementArrayBuffer& ElementArrayBuffer::operator=(
        ElementArrayBuffer&& buffer) noexcept
    {
        destroyBuffer();
        elementID = std::exchange(buffer.elementID, 0);
        return *this;
    }

    void ElementArrayBuffer::bind(void) const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID);
    }

    void ElementArrayBuffer::unbind(void) const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void ElementArrayBuffer::destroyBuffer(void) noexcept {
        if (elementID)
            glDeleteBuffers(1, &elementID);
    }

    ElementArrayBuffer::~ElementArrayBuffer(void) noexcept {
        destroyBuffer();
    }

}

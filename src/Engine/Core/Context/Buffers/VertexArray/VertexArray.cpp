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
#include "VertexArray.hpp"

namespace mpgl {

    VertexArray::VertexArray(void) noexcept : arrayID{0} {
        glGenVertexArrays(1, &arrayID);
    }

    VertexArray::VertexArray(uint32 arrayID) noexcept
        : arrayID{arrayID} {}

    VertexArray::VertexArray(VertexArray&& array) noexcept
        : arrayID{array.arrayID}
    {
        array.arrayID = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& array) noexcept {
        this->~VertexArray();
        arrayID = array.arrayID;
        array.arrayID = 0;
        return *this;
    }

    void VertexArray::bind(void) const noexcept {
        glBindVertexArray(arrayID);
    }

    void VertexArray::unbind(void) const noexcept {
        glBindVertexArray(0);
    }

    void VertexArray::draw(DrawMode mode, uint32 size) const noexcept {
        // Change static cast to std::to_underlying in C++23
        glDrawArrays(static_cast<uint16>(mode), 0, size);
    }

    VertexArray::~VertexArray(void) noexcept {
        if (arrayID)
            glDeleteVertexArrays(1, &arrayID);
    }

}

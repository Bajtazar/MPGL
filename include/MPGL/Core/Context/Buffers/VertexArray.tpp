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
#pragma once

namespace mpgl {

    template <InstanceOf<Vertex> Tp>
    void VertexArray::setArrayData(
        [[maybe_unused]] VertexTag<Tp>) const noexcept
    {
        std::size_t i = 0;
        for (auto const& [size, offset, type] : Tp::memoryLayout)
        {
            // Change static cast to std::to_underlying in C++23
            glVertexAttribPointer(i, size, static_cast<uint16>(type),
                GL_FALSE, sizeof(Tp), reinterpret_cast<void*>(offset));
            glEnableVertexAttribArray(i++);
        }
    }

}

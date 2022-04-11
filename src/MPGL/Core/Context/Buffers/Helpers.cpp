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
#include <MPGL/Core/Context/Buffers/Helpers.hpp>

namespace mpgl {

    namespace details {

        void generateArrays(
            uint32 size,
            uint32* ptr) noexcept
        {
            glGenVertexArrays(size, ptr);
        }

        void generateTextures(
            uint32 size,
            uint32* const ptr) noexcept
        {
            glGenTextures(size, ptr);
        }

        void destroyBuffers(
            uint32 size,
            uint32* const ptr) noexcept
        {
            glDeleteBuffers(size, ptr);
        }

        void destroyArrays(
            uint32 size,
            uint32* const ptr) noexcept
        {
            glDeleteVertexArrays(size, ptr);
        }

        void destroyTextures(
            uint32 size,
            uint32* const ptr) noexcept
        {
            glDeleteTextures(size, ptr);
        }

    }

}

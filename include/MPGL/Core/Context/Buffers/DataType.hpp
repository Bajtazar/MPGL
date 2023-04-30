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
#pragma once

#include <glad/glad.h>

#include <MPGL/Traits/Types.hpp>

namespace mpgl {

    /**
     * Indicates what is the type of the given data. Used mainly to
     * pass the information what data type is stored under an array
     */
    enum class DataType : uint16 {
        /// The 8-bit signed integer
        Int8                                          = GL_BYTE,
        /// the 8-bit unsigned integer
        UInt8                                         = GL_UNSIGNED_BYTE,
        /// the 16-bit signed integer
        Int16                                         = GL_SHORT,
        /// the 16-bit unsigned integer
        UInt16                                        = GL_UNSIGNED_SHORT,
        /// the 32-bit signed integer
        Int32                                         = GL_INT,
        /// the 32-bit unsigned integer
        UInt32                                        = GL_UNSIGNED_INT,
        /// the 16-bit floating point
        Float16                                       = GL_HALF_FLOAT,
        /// the 32-bit floating point (single precision)
        Float32                                       = GL_FLOAT,
        /// the 64-bit floating point (double precision)
        Float64                                       = GL_DOUBLE,
        /// the 32-bit fixed point [16:16]
        Fixed                                         = GL_FIXED
    };

}

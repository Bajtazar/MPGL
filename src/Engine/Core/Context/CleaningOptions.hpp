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

#include <glad/glad.h>

#include "../../Traits/Types.hpp"

namespace mpgl {

    /**
     * Cleaning options for window frame buffers
     */
    enum class CleaningOptions : uint32 {
        /// No cleaning
        None                              = 0,
        /// Only color buffer is cleaned
        Color                             = GL_COLOR_BUFFER_BIT,
        /// Only depth buffer is cleaned
        Depth                             = GL_DEPTH_BUFFER_BIT,
        /// Only stencil buffer is cleaned
        Stencil                           = GL_STENCIL_BUFFER_BIT,
        /// Color buffer and depth buffer are cleaned
        ColorAndDepth                     = Color | Depth,
        /// Color buffer and stencil buffer are cleaned
        ColorAndStencil                   = Color | Stencil,
        /// Depth buffer and stencil buffer are cleaned
        DepthAndStencil                   = Depth | Stencil,
        /// Color buffer, depth buffer and stencil buffer are cleaned
        ColorAndDepthAndStencil           = Color | Depth | Stencil
    };

}

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

#include "../../Traits/Types.hpp"

namespace mpgl {

    /**
     * Available window options. Used by window constructor
     * to determine the renderer and platform settings.
     */
    struct Options {
        /// The major opengl version
        uint16              openGLMajorVersion        = 3;
        /// The minor opengl version
        uint16              openGLMinorVersion        = 3;
        /// The number of samples used by anti-aliaser
        uint8               antiAliasingSamples : 4   = 4;
        /// Specifies whether the window will float on top of
        /// other windows
        bool                floating : 1              = false;
        /// Specifies whether the window should be displayed
        /// in the full screen mode
        bool                maximised : 1             = false;
        /// Specifies whether the window should be resizable
        bool                resizable : 1             = true;
        /// Specifies whether the window should be displayed
        /// without os-related decorations
        bool                clean : 1                 = false;
        // bool                            transparent : 1;
    };

}

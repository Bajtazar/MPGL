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
#include <GLFW/glfw3.h>

#include "../../Traits/Types.hpp"

namespace mpgl {

    /**
     * Describes the mouse buttons
     */
    enum class MouseButton : uint8 {
        Left            = GLFW_MOUSE_BUTTON_LEFT,
        Right           = GLFW_MOUSE_BUTTON_RIGHT,
        Middle          = GLFW_MOUSE_BUTTON_MIDDLE,
        Button4         = GLFW_MOUSE_BUTTON_4,
        Button5         = GLFW_MOUSE_BUTTON_5,
        Button6         = GLFW_MOUSE_BUTTON_6,
        Button7         = GLFW_MOUSE_BUTTON_7,
        Button8         = GLFW_MOUSE_BUTTON_8
    };

}

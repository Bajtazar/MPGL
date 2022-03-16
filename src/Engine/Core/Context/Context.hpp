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

#include "../../Mathematics/Vector.hpp"
#include "ShadersContext.hpp"
#include "Options.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>

namespace mpgl {

    /**
     * Represents an OpenGL context
     */
    struct Context {
        /**
         * Initializes OpenGL context
         */
        explicit Context(void) noexcept;

        /// Current context window shaders
        ShadersContext                              shaders;
        /// Current context window dimensions
        Vector2u                                    windowDimensions;
        /// Current context window options
        Options                                     windowOptions;

        /**
         * Prints message in the console in case of the GLFW
         * error
         *
         * @param error the GLFW error code
         * @param message the GLFW error message
         */
        friend void errorCallback(int32 error,
            char const* message) noexcept;

        /**
         * Destroys an OpenGL context
         */
        ~Context(void) noexcept;
    };

    struct GraphicalObject {
        /// The OpenGL context shared by all graphical objects
        static Context                              context;

        /// Forces context initialization before any other object
        static_assert((context, true));
    };

}

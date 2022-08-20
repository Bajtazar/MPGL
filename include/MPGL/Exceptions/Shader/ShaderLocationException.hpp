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

#include <MPGL/Exceptions/Shader/ShaderException.hpp>

namespace mpgl {

    /**
     * Base exception for all shader location exceptions
     */
    struct ShaderLocationException : public ShaderException {
        /**
         * Constructs a new Shader Location object
         */
        constexpr explicit ShaderLocationException(
            void) noexcept = default;

        /**
         * Pure virtual method that has to be implemented by
         * derived classes
         *
         * @return message associated with an exception
         */
        [[nodiscard]] constexpr virtual const char* what(
            void) const noexcept = 0;

        /**
         * Vrtual destructor. Destroys the Shader Location Exception
         */
        constexpr virtual ~ShaderLocationException(
            void) noexcept = default;
    };

}

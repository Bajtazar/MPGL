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
 *  dist
 */
#pragma once

#include <MPGL/Exceptions/ShaderLocationException.hpp>

#include <string>

namespace mpgl {

    /**
     * Exception indicating that the uniform with the given name
     * does not exist
     */
    class ShaderLocationUniformException
        : public ShaderLocationException
    {
    public:
        /**
         * Constructs a new Shader Location Uniform
         * Exception object
         *
         * @param name the uniform's name
         */
        constexpr explicit ShaderLocationUniformException(
            std::string const& name) noexcept
                : message{"The unifrom with name {"
                    + name + "} does not exist!"} {}

        /**
         * Returns the message informing that the uniform with
         * the given name does not exist
         *
         * @return the exception description
         */
        [[nodiscard]] constexpr const char* what(
            void) const noexcept final
                { return message.c_str(); }

        /**
         * Destroys the Shader Location Uniform
         * Exception object
         */
        constexpr ~ShaderLocationUniformException(
            void) noexcept = default;
    private:
        std::string                                 message;
    };

}

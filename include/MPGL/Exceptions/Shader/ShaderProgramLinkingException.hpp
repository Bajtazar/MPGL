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

#include <vector>
#include <string>

namespace mpgl {

    /**
     * Exception indicating that an error occurs during shader
     * program linking
     */
    class ShaderProgramLinkingException : public ShaderException {
    public:
        /**
         * Constructs a new Shader Program Linking Exception object
         * and attaches a shader program linking log
         *
         * @param infoLog the constant reference to the shader
         * program linking log
         */
        explicit ShaderProgramLinkingException(
            std::string const& infoLog) noexcept
                : message{std::string{"The shader program linker has"}
                    + " occured an unexpected error. Additional data: "
                    + infoLog} {}

        /**
         * Returns the message informing what happened during
         * shader program linking
         *
         * @return the exception description
         */
        [[nodiscard]] const char* what (void) const noexcept
            { return message.c_str(); }

        /**
         * Returns the message informing what happened during
         * shader program linking
         *
         * @return the exception description
         */
        [[nodiscard]] std::string const& getMessage(
            void) const noexcept
                { return message; }

        /**
         * Destroys the Shader Program Linking Exception object
         */
        ~ShaderProgramLinkingException(void) noexcept = default;
    private:
        std::string                                 message;
    };

}

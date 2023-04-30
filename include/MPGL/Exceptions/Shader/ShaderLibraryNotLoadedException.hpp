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

#include <MPGL/Exceptions/Shader/ShaderException.hpp>

#include <vector>
#include <string>

namespace mpgl {


    /**
     * Exception indicating that the shader library is not
     * loaded yet but the shaders are being demanded
     */
    class ShaderLibraryNotLoadedException
        : public ShaderException
    {
    public:
        typedef std::vector<std::string>            Paths;

        /**
         * Construct a new Shader Library Not Loaded
         * Exception object
         *
         * @param demandedShader the shader that is being
         * demanded from the not loaded shader library
         */
        explicit ShaderLibraryNotLoadedException(
            std::string const& demandedShader) noexcept
                : message{"The [" + demandedShader + "] is being "
                + "demanded even though the shader library is not loaded yet."} {}

        /**
         * Returns the message informing which shader is demanding
         * a not loaded shader library
         *
         * @return the exception description
         */
        [[nodiscard]] const char* what (
            void) const noexcept
                { return message.c_str(); }

        /**
         * Returns the message informing which shader is demanding
         * a not loaded shader library
         *
         * @return the exception description
         */
        [[nodiscard]] const std::string& getMessage(
            void) const noexcept
                { return message; }

        /**
         * Destroy the Shader Library Invalid Shaders Exception
         * object
         */
        ~ShaderLibraryNotLoadedException(
            void) noexcept = default;
    private:
        std::string                                 message;
    };

}

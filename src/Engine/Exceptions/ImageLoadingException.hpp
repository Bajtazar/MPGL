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

#include "MPGLException.hpp"
#include <string>

namespace mpgl {

    /**
     * Base exception for all Image Loading related exceptions
     */
    class ImageLoadingException : public MPGLException {
    public:
        /**
         * Constructs a new Image Loading Exception object with
         * given file name and message
         *
         * @param fileName the constant reference to the file name
         * @param message the constant reference to the message
         */
        explicit ImageLoadingException(
            std::string const& fileName,
            std::string const& message) noexcept
                : fileName{fileName}, message{message} {}

        /**
         * Returns the message informing that there was an exception
         * in the image loading process
         *
         * @return the exception description
         */
        virtual const char* what (void) const noexcept final
            { return message.c_str(); }

        /**
         * Virtual destructor. Destroys the Image Loading
         * Exception object
         */
        virtual ~ImageLoadingException(void) noexcept = default;
    private:
        std::string fileName;
        std::string message;
    };

}
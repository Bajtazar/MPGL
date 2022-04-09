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

#include <MPGL/Exceptions/ImageLoadingException.hpp>

namespace mpgl {

    /**
     * Exception indicating that a loaded image file type is not
     * supported
     */
    struct ImageLoadingUnsuportedFileType
        : public ImageLoadingException
    {
        /**
         * Constructs a new Image Loading Unsuported File Type object
         * with a given file name
         *
         * @param fileName the constant reference to the file name
         */
        explicit ImageLoadingUnsuportedFileType(
            std::string const& fileName) noexcept
                : ImageLoadingException{fileName, "File " + fileName
                    + " cannot be oppened - unsuported type"} {}

        /**
         * Destroys the Image Loading Unsuported File Type object
         */
        ~ImageLoadingUnsuportedFileType(void) noexcept = default;
    };

}
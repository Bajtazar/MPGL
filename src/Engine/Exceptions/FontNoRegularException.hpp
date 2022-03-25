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
     * Exception indicating that a font does not contain
     * the regular version
     */
    class FontNoRegularException : public MPGLException {
    public:
        /**
         * Constructs a new Font No Regular Exception object with
         * given font name
         *
         * @param fontName the font name
         */
        explicit FontNoRegularException(
            std::string const& fontName) noexcept
                : message{fontName +
                    " - font has not contain regular version"} {}

        /**
         * Returns the message informing that the given font does
         * not contain regular version
         *
         * @return the exception description
         */
        virtual const char* what (void) const noexcept final
            { return message.c_str(); }

        ~FontNoRegularException(void) noexcept = default;
    private:
        std::string message;
    };

}

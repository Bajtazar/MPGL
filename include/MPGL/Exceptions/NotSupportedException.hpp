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

#include <MPGL/Exceptions/MPGLException.hpp>
#include <string>

namespace mpgl {

    /**
     * Exception indicating that the given feature is not supported
     */
    class NotSupportedException : public MPGLException {
    public:
        /**
         * Constructs a new Not Supported Exception object with
         * given information string
         *
         * @param info the constant reference to the information
         * string
         */
        explicit NotSupportedException(
            std::string const& info) noexcept
                : message{std::string("An exception related to the") +
                    "lack of feature support has occured - " + info
                    + "\n"} {}

        /**
         * Returns the message informing that the given feature is
         * not supported
         *
         * @return the exception description
         */
        [[nodiscard]] const char* what (void) const noexcept final
            { return message.c_str(); }

        /**
         * Returns the message informing that the given feature is
         * not supported
         *
         * @return the exception description
         */
        [[nodiscard]] std::string const& getMessage(
            void) const noexcept
                { return message; }

        /**
         *  Destroys the Not Supported Exception object
         */
        ~NotSupportedException(void) noexcept = default;
    protected:
        std::string                             message;
    };

}

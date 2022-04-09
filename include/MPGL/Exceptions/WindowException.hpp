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

#include <MPGL/Exceptions/MPGLException.hpp>
#include <string>

namespace mpgl{

    /**
     * Base exception for all Window related exceptions
     */
    class WindowException : public MPGLException {
    public:
        /**
         * Construct a new Window Exception object with
         * a window name and a message
         *
         * @param windowName the name of the window
         * @param messahe the exception message
         */
        explicit WindowException(
            std::string windowName,
            std::string message) noexcept
                : windowName{std::move(windowName)},
                  message{std::move(message)} {}

        /**
         * Returns the message informing that in the given
         * window an exception has occured
         *
         * @return the exception description
         */
        [[nodiscard]] const char* what (void) const noexcept final
            { return message.c_str(); }

        /**
         * Returns the message informing that in the given
         * window an exception has occured
         *
         * @return the exception description
         */
        [[nodiscard]] std::string const& getMessage(
            void) const noexcept
                { return message; }

        /**
         * Returns the name of the window
         *
         * @return the name of the window
         */
        [[nodiscard]] std::string const& getWindowName(
            void) const noexcept
                { return windowName; }

        /**
         * Virtual destructor. Destroys the Render Window Exception
         * object
         */
        virtual ~WindowException(void) noexcept = default;
    protected:
        std::string windowName;
        std::string message;
    };

}

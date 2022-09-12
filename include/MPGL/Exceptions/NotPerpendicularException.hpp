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

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Exceptions/MPGLException.hpp>

#include <string>

namespace mpgl {

    /**
     * Exception indicating that the given vectors are not
     * perpendicular to each other
     */
    class NotPerpendicularException : public MPGLException {
    public:
        /**
         * Constructs a new not perpendicular exception object
         *
         * @tparam Size the vector's size
         * @tparam Tp the vector's element type
         * @param left a constant reference to the left vector
         * @param right a constant reference to the right vector
         */
        template <std::size_t Size, Arithmetic Tp>
        explicit NotPerpendicularException(
            Vector<Tp, Size> const& left,
            Vector<Tp, Size> const& right);

        /**
         * Returns the message informing that the two vectors
         * are not perpendicular
         *
         * @return the exception description
         */
        [[nodiscard]] const char* what (void) const noexcept final
            { return message.c_str(); }

        /**
         * Destroys the not perpendicular exception object
         */
        ~NotPerpendicularException(
            void) noexcept = default;
    private:
        std::string                             message;
    };

}

#include <MPGL/Exceptions/NotPerpendicularException.tpp>

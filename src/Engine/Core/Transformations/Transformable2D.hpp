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

#include "../../Mathematics/Matrix.hpp"
#include "Transformable.hpp"

namespace mpgl {

    /**
     * Definies the set of transformations for the
     * 2D entity
     */
    template<>
    struct Transformable<dim::Dim2>
        : public virtual ScreenTransformationEvent
    {
        /**
         *  Construct a new Transformable object
         */
        explicit Transformable(void) noexcept = default;

        /**
         * Pure virtual function. Has to be overloaded.
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        virtual void translate(Vector2f const& shift) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Scales the figure around given center by
         * a given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        virtual void scale(Vector2f const& center,
            float32 factor) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Rotates the figure around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        virtual void rotate(Vector2f const& center, float32 angle) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        virtual void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept = 0;

        /**
         * Virtual destrouctor. Destroy the Transformable object
         */
        virtual ~Transformable(void) noexcept = default;
    };

    typedef Transformable<dim::Dim2>      Transformable2D;

}

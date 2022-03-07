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

#include "../Shape.hpp"
#include "../Shaders/Shadeable.hpp"
#include "../Transformations/Transformable2D.hpp"

namespace mpgl {

    /**
     * Base class for the two-dimensional figures
     */
    class Figure : public Shape, public Shadeable,
        public Transformable2D
    {
    public:
        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        virtual void translate(
            Vector2f const& shift) noexcept = 0;

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
        virtual void rotate(Vector2f const& center,
            float32 angle) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        virtual void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Transforms the figure during the screen
         * transformation event
         *
         * @param oldDimensions the old screen dimensions
         */
        virtual void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept = 0;

        /**
         * Destroy the Figure object
         */
        virtual ~Figure(void) noexcept = default;
    protected:
        /**
         * Construct a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         */
        explicit Figure(std::string const& programName);

        /**
         * Construct a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         * @param exec the shader's executable
         */
        explicit Figure(std::string const& programName,
            Executable exec);

        /**
         *  Construct a new Figure object
         */
        explicit Figure(void) noexcept = default;

        /**
         * Construct a new Figure object from a given
         * constant reference
         *
         * @param shape the given shape constant reference
         */
        explicit Figure(Figure const& shape);

        explicit Figure(Figure&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given figure constant reference
         * @return the reference to this object
         */
        Figure& operator=(Figure const& shape);

        Figure& operator=(Figure&& shape) noexcept = default;

        mutable bool                                isModified = false;
    };

}

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

#include <MPGL/Core/Transformations/Transformation.hpp>
#include <MPGL/Mathematics/Tensors/Matrix.hpp>

namespace mpgl {

    /**
     * Rotates the given coordinates
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Rotation;

    /**
     * Rotates the given coordiantes in the 2D space using rotation
     * matrix and rotation center
     */
    template <>
    class Rotation<dim::Dim2> : public Transformation<dim::Dim2> {
    public:
        using TransformedType = Adapter2D;

        /**
         * Constructs a new rotation object
         *
         * @param rotationMatrix the constant reference to the
         * rotation matrix
         * @param rotationCenter the constant reference to the
         * rotation center [the point around which the rotation
         * is performed]
         */
        Rotation(
            Matrix2f const& rotationMatrix,
            Vector2f const& rotationCenter) noexcept;

        /**
         * Constructs a new rotation object
         *
         * @param rotationMatrix the rvalue reference to the
         * rotation matrix
         * @param rotationCenter the rvalue reference to the
         * rotation center [the point around which the rotation
         * is performed]
         */
        Rotation(
            Matrix2f&& rotationMatrix,
            Vector2f&& rotationCenter) noexcept;

        /**
         * Constructs a new rotation object
         *
         * @param angle the radial angle which is used to rotate
         * coordinates around the rotation center [counterclockwisely]
         * @param rotationCenter the constant reference to the
         * rotation center [the point around which the rotation
         * is performed]
         */
        Rotation(
            float32 angle,
            Vector2f const& rotationCenter) noexcept;

        /**
         * Constructs a new rotation object
         *
         * @param angle the radial angle which is used to rotate
         * coordinates around the rotation center [counterclockwisely]
         * @param rotationCenter the rvalue reference to the
         * rotation center [the point around which the rotation
         * is performed]
         */
        Rotation(
            float32 angle,
            Vector2f&& rotationCenter) noexcept;

        Rotation(Rotation const&) noexcept = default;
        Rotation(Rotation&&) noexcept = default;

        Rotation& operator=(
            Rotation const&) noexcept = default;
        Rotation& operator=(
            Rotation&&) noexcept = default;

        /**
         * Perfoms rotation with the inner rotation matrix and
         * rotation center on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        void operator() (
            any::InputRange<TransformedType>& coords
            ) const noexcept final;

        /**
         * Perfoms rotation with the inner rotation matrix and
         * rotation center on the coordinate
         *
         * @param coord the reference to the coordinate object
         */
        void operator() (
            TransformedType& coord) const noexcept final;

        /**
         * Returns a reference to the rotation center
         *
         * @return the reference to the rotation center
         */
        [[nodiscard]] Vector2f& getCenter(void) noexcept
            { return rotationCenter; }

        /**
         * Returns a constant reference to the rotation center
         *
         * @return the constant reference to the rotation center
         */
        [[nodiscard]] Vector2f const& getCenter(
            void) const noexcept
                { return rotationCenter; }

        /**
         * Returns a reference to the rotation matrix
         *
         * @return the reference to the rotation matrix
         */
        [[nodiscard]] Matrix2f& getMatrix(void) noexcept
            { return rotationMatrix; }

        /**
         * Returns a constant reference to the rotation matrix
         *
         * @return the constant reference to the rotation matrix
         */
        [[nodiscard]] Matrix2f const& getMatrix(
            void) const noexcept
                { return rotationMatrix; }

        ~Rotation(void) noexcept = default;
    private:
        Matrix2f                                rotationMatrix;
        Vector2f                                rotationCenter;
    };

    typedef Rotation<dim::Dim2>                 Rotation2D;

}

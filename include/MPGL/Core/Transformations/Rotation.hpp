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

#include <MPGL/Core/Transformations/Transformation.hpp>
#include <MPGL/Mathematics/Tensors/Matrix.hpp>

namespace mpgl {

    /**
     * Rotates the given coordiantes in the given space using
     * rotation matrix and rotation center
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Rotation : public Transformation<Dim> {
    public:
        using TransformedType = Transformation<Dim>::TransformedType;
        using VectorDf = Vector<float32, Dim::orthogonal_space_degree>;
        using MatrixDf = SquareMatrix<float32, VectorDf::size()>;

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
            MatrixDf const& rotationMatrix,
            VectorDf const& rotationCenter) noexcept;

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
            MatrixDf&& rotationMatrix,
            VectorDf&& rotationCenter) noexcept;

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
        [[nodiscard]] VectorDf& getCenter(void) noexcept
            { return rotationCenter; }

        /**
         * Returns a constant reference to the rotation center
         *
         * @return the constant reference to the rotation center
         */
        [[nodiscard]] VectorDf const& getCenter(
            void) const noexcept
                { return rotationCenter; }

        /**
         * Returns a reference to the rotation matrix
         *
         * @return the reference to the rotation matrix
         */
        [[nodiscard]] MatrixDf& getMatrix(void) noexcept
            { return rotationMatrix; }

        /**
         * Returns a constant reference to the rotation matrix
         *
         * @return the constant reference to the rotation matrix
         */
        [[nodiscard]] MatrixDf const& getMatrix(
            void) const noexcept
                { return rotationMatrix; }

        ~Rotation(void) noexcept = default;
    private:
        MatrixDf                                rotationMatrix;
        VectorDf                                rotationCenter;
    };

    template class Rotation<dim::Dim2>;
    template class Rotation<dim::Dim3>;

    typedef Rotation<dim::Dim2>                 Rotation2D;
    typedef Rotation<dim::Dim3>                 Rotation3D;

    /**
     * Constructs a new rotation object
     *
     * @param angle the radial angle which is used to rotate
     * coordinates around the rotation center [counterclockwisely]
     * @param rotationCenter the constant reference to the
     * rotation center [the point around which the rotation
     * is performed]
     * @return the 2D rotation transformation
     */
    [[nodiscard]] Rotation2D makeRotation(
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
     * @return the 2D rotation transformation
     */
    [[nodiscard]] Rotation2D makeRotation(
        float32 angle,
        Vector2f&& rotationCenter) noexcept;

    /**
     * Constructs a new rotation object
     *
     * @param yaw the angle of rotation around the x axis [in Rads]
     * @param pitch the angle of rotation around the y axis [in Rads]
     * @param roll the angle of rotation around the z axis [in Rads]
     * @param rotationCenter the constant reference to the
     * rotation center [the point around which the rotation
     * is performed]
     * @return the 3D rotation transformation
     */
    [[nodiscard]] Rotation3D makeRotation(
        float32 yaw,
        float32 pitch,
        float32 roll,
        Vector3f const& rotationCenter) noexcept;

    /**
     * Constructs a new rotation object
     *
     * @param yaw the angle of rotation around the x axis [in Rads]
     * @param pitch the angle of rotation around the y axis [in Rads]
     * @param roll the angle of rotation around the z axis [in Rads]
     * @param rotationCenter the rvalue reference to the
     * rotation center [the point around which the rotation
     * is performed]
     * @return the 3D rotation transformation
     */
    [[nodiscard]] Rotation3D makeRotation(
        float32 yaw,
        float32 pitch,
        float32 roll,
        Vector3f&& rotationCenter) noexcept;

    /**
     * Constructs a new rotation object
     *
     * @param angles a constant reference to the vector object
     * containing angles
     * @param rotationCenter the constant reference to the
     * rotation center [the point around which the rotation
     * is performed]
     * @return the 3D rotation transformation
     */
    [[nodiscard]] Rotation3D makeRotation(
        Vector3f const& angles,
        Vector3f const& rotationCenter) noexcept;

    /**
     * Constructs a new rotation object
     *
     * @param angles a constant reference to the vector object
     * containing angles
     * @param rotationCenter the rvalue reference to the
     * rotation center [the point around which the rotation
     * is performed]
     * @return the 3D rotation transformation
     */
    [[nodiscard]] Rotation3D makeRotation(
        Vector3f const& angles,
        Vector3f&& rotationCenter) noexcept;

}

#include <MPGL/Core/Transformations/Rotation.tpp>

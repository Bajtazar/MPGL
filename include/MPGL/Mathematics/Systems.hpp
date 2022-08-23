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

#include <MPGL/Mathematics/Tensors/Matrix.hpp>
#include <MPGL/Mathematics/Ranges.hpp>

#include <complex>

namespace mpgl {

    /**
     * Returns the value of the signum function on the given
     * number
     *
     * @tparam Tp the integer type
     * @param value the number
     * @return the value of the signum function
     */
    template <std::signed_integral Tp>
    [[nodiscard]] inline constexpr Tp signum(
        Tp const& value) noexcept;

    /**
     * Returns whether the given value is inside the given
     * boundries. The edge cases are classified as being inside
     * too
     *
     * @tparam Tp the value type
     * @param first the first boundry
     * @param second the second bondry
     * @param checked the checked value
     * @return if the value is inside the boundries
     */
    template <std::totally_ordered Tp>
    [[nodiscard]] inline constexpr bool between(
        Tp const& first,
        Tp const& second,
        Tp const& checked) noexcept;


    /**
     * Returns whether the given vector is inside the given
     * boundries. Checks if all dimensions are between their
     * boundries
     *
     * @tparam Tp the vector's value type
     * @tparam Size the vector's size
     * @param first the first boundry
     * @param second the second bondry
     * @param checked the checked vector
     * @return if the vector is inside the boundries
     */
    template <std::totally_ordered Tp, std::size_t Size>
    [[nodiscard]] inline constexpr bool between(
        Vector<Tp, Size> const& first,
        Vector<Tp, Size> const& second,
        Vector<Tp, Size> const& checked) noexcept;

    /**
     * Algorithms that computes the intersection of lines
     */
    class IntersectionOfFn {
    public:
        /**
         * Returns an intersection of two lines. Returns an empty
         * optional if lines are parallel
         *
         * @tparam Tp the vector's value type
         * @param firstPoint the point on the first line
         * @param firstVersor the versor of the first line
         * @param secondPoint the point on the second line
         * @param secondVersor the versor of the second line
         * @return the intersection point of the two lines
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr std::optional<Vector2<Tp>>
            operator() (
                Vector2<Tp> const& firstPoint,
                Vector2<Tp> const& firstVersor,
                Vector2<Tp> const& secondPoint,
                Vector2<Tp> const& secondVersor) const noexcept;

        /**
         * Returns an intersection of two lines. Returns an empty optional
         * if lines are not intesecting
         *
         * @tparam Tp the vector's value type
         * @param firstPoint the point on the first line
         * @param firstVersor the versor of the first line
         * @param secondPoint the point on the second line
         * @param secondVersor the versor of the second line
         * @return the intersection point of the two lines
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr std::optional<Vector3<Tp>>
            operator() (
                Vector3<Tp> const& firstPoint,
                Vector3<Tp> const& firstVersor,
                Vector3<Tp> const& secondPoint,
                Vector3<Tp> const& secondVersor) const noexcept;
    private:
        /// Tuple used to return the valid system params
        template <FloatConvertible Tp>
        using SystemTuple = std::tuple<Matrix2<Tp>, uint8, uint8, uint8>;

        /**
         * Constructs a Matrix2 consisting of vector elements lying
         * under the indicated dimensions
         *
         * @tparam Tp the vector's value type
         * @param firstVersor the versor of the first line
         * @param secondVersor the versor of the second line
         * @param firstDim the first dimension used to build the matrix
         * @param secondDim the second dimension used to build
         * the matrix
         * @return an optional containing the space's matrix. If
         * matrix is singular then returns an empty optional
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr std::optional<Matrix2<Tp>>
            getMatrix(
                Vector3<Tp> const& firstVersor,
                Vector3<Tp> const& secondVersor,
                uint8 firstDim,
                uint8 secondDim) const noexcept;

        /**
         * Finds a system of coordinates that can be used to determine
         * the intersection of lines. If this system does not exist
         * then returns an empty optional
         *
         * @tparam Tp the vector's value type
         * @param firstVersor the versor of the first line
         * @param secondVersor the versor of the second line
         * @return an optional containing informations about valid
         * system of coordinates. If this system does not exist then
         * returns an empty optional
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr std::optional<SystemTuple<Tp>>
            findValidSystem(
                Vector3<Tp> const& firstVersor,
                Vector3<Tp> const& secondVersor) const noexcept;

        /**
         * Builds the result vector used by the linear system solver
         * consisting of vector elements lying under the indicated
         * dimensions
         *
         * @tparam Tp the vector's value type
         * @param firstPoint the point on the first line
         * @param secondPoint the point on the second line
         * @param firstDim the first dimension used to build the matrix
         * @param secondDim the second dimension used to build
         * the matrix
         * @return the result vector
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr Vector2<Tp>
            buildResultVector(
                Vector3<Tp> const& firstPoint,
                Vector3<Tp> const& secondPoint,
                uint8 firstDim,
                uint8 secondDim) const noexcept;

        /**
         * Checks whether solution of the linear system satisfies
         * the last dimension requirements
         *
         * @tparam Tp the vector's value type
         * @param solution the solution of the linear system
         * @param firstPoint the point on the first line
         * @param firstVersor the versor of the first line
         * @param secondPoint the point on the second line
         * @param secondVersor the versor of the second line
         * @param testDim the third dimension used to test the
         * solution
         * @return if solution satisfies the last dimension
         * requirements
         */
        template <FloatConvertible Tp>
        [[nodiscard]] constexpr bool
            validSolution(
                Vector2<Tp> const& solution,
                Vector3<Tp> const& firstPoint,
                Vector3<Tp> const& firstVersor,
                Vector3<Tp> const& secondPoint,
                Vector3<Tp> const& secondVersor,
                uint8 testDim) const noexcept;
    };

    inline constexpr IntersectionOfFn               intersectionOf;

    /**
     * Checks whether the given point is inside a triangle
     *
     * @tparam Tp the vector's value type
     * @param position the point's position
     * @param firstVertex the first triangle vertex's position
     * @param secondVertex the second triangle vertex's position
     * @param thirdVertex the third triangle vertex's position
     * @return if the given point is inside the triangle
     */
    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr bool
        isInsideTriangle(
            Vector2<Tp> const& position,
            Vector2<Tp> const& firstVertex,
            Vector2<Tp> const& secondVertex,
            Vector2<Tp> const& thirdVertex) noexcept;

    /**
     * Checks whether the given point is on a line
     *
     * @tparam Tp the vector's value type
     * @param position the point's position
     * @param firstVertex the first line vertex's position
     * @param secondVertex the second line vertex's position
     * @return if the given point is on the line
     */
    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr bool
        isOnLine(
            Vector2<Tp> const& position,
            Vector2<Tp> const& firstVertex,
            Vector2<Tp> const& secondVertex) noexcept;

    /**
     * Calculates a normal vector of the given plane
     *
     * @tparam Tp the vector's value type
     * @param firstPoint the plane's first point
     * @param secondPoint the plane's second point
     * @param thirdPoint the plane's third point
     * @return the normal vector of the given plane
     */
    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr Vector3<Tp>
        planeNormalVector(
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& thirdPoint) noexcept;

    /**
     * Calculates a coefficients of the given plane [A, B, C, D]
     *
     * @tparam Tp the vector's value type
     * @param firstPoint the plane's first point
     * @param secondPoint the plane's second point
     * @param thirdPoint the plane's third point
     * @return the coefficients of the given plane
     */
    template <FloatConvertible Tp>
    [[nodiscard]] constexpr Vector4<Tp>
        planeCoefficients(
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& thirdPoint) noexcept;

    /**
     * Checks whether the given point is on the plane
     *
     * @tparam Tp the vector's value type
     * @param coefficients the plane's coefficients
     * @param point the point's position
     * @return if point is lying on the plane
     */
    template <FloatConvertible Tp>
    [[nodiscard]] constexpr bool isOnPlane(
        Vector4<Tp> const& coefficients,
        Vector3<Tp> const& point) noexcept;

    /**
     * Calculates distance between plane and point
     *
     * @tparam Tp the vector's value type
     * @param coefficients the plane's coefficients
     * @param point the point's position
     * @return the distance between plane and point
     */
    template <FloatConvertible Tp>
    [[nodiscard]] constexpr Tp distance(
        Vector4<Tp> const& coefficients,
        Vector3<Tp> const& point) noexcept;

    /**
     * Transform the given vector from the cartesian coordinate
     * system to the polar coordinate system. The x-axis represents
     * the radius and the y-axis represents the angle in the
     * output vector
     *
     * @tparam Tp the vector's type
     * @param vector the vector in cartesian coordinate system
     * @return the vector in polar coordinate system
     */
    template <FloatConvertible Tp>
    [[nodiscard]] Vector2<Tp> cartesianToPolar(
        Vector2<Tp> const& vector) noexcept;

    /**
     * Transforms the given vector from the polar coordinate
     * system to the cartesian coordinate system
     *
     * @tparam Tp the vector's type
     * @param vector the vector in polar coordinate system
     * @return the vector in cartesian coordinate system
     */
    template <FloatConvertible Tp>
    [[nodiscard]] Vector2<Tp> polarToCartesian(
        Vector2<Tp> const& vector) noexcept;

    /**
     * Transform the given vector from the cartesian coordinate
     * system to the spherical coordinate system. The x-axis represents
     * the radius, the y-axis represents the phi angle, the z-axis
     * represents the theta angle in the output vector
     *
     * @tparam Tp the vector's type
     * @param vector the vector in cartesian coordinate system
     * @return the vector in polar spherical system
     */
    template <FloatConvertible Tp>
    [[nodiscard]] Vector3<Tp> cartesianToSpherical(
        Vector3<Tp> const& vector) noexcept;

    /**
     * Transforms the given vector from the spherical coordinate
     * system to the cartesian coordinate system
     *
     * @tparam Tp the vector's type
     * @param vector the vector in spherical coordinate system
     * @return the vector in cartesian coordinate system
     */
    template <FloatConvertible Tp>
    [[nodiscard]] Vector3<Tp> sphericalToCartesian(
        Vector3<Tp> const& vector);

    /**
     * Calculates the rotation matrix for the given angle.
     * This matrix rotates the two dimensional vector
     * counter-clock-wise
     *
     * @tparam Tp the matrix's element type
     * @param angle the rotation angle [rads]
     * @return the rotation matrix
     */
    template <FloatConvertible Tp>
    [[nodiscard]] Matrix2<Tp> rotationMatrix(Tp angle) noexcept;

    /**
     * Calculates the hermitian transpose of the given matrix
     *
     * @tparam Rows the number of matrix's rows
     * @tparam Cols the number of matrix's colums
     * @param matrix the matrix to be hermitian transposed
     * @return the hermitian transposed matrix
     */
    template <std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<std::complex<float64>, Cols, Rows>
        hermitianTranspose(
            Matrix<std::complex<float64>, Rows, Cols> const& matrix
            ) noexcept;

    /**
     * Calculates the perspective projection matrix
     *
     * @param fov the fov
     * @param aspect the view port's aspect ratio
     * @param zAxis the z-axis depth [near, far]
     * @return the perspective projection matrix
     */
    [[nodiscard]] Matrix4f perspectiveMatrix(
        float32 fov,
        float32 aspect,
        Vector2f zAxis) noexcept;

    /**
     * Calculates the frustum projection matrix
     *
     * @param xAxis the x-axis depth [left, right]
     * @param yAxis the y-axis depth [bottom, top]
     * @param zAxis the z-axis depth [near, far]
     * @return the frustum projection matrix
     */
    [[nodiscard]] constexpr Matrix4f frustumMatrix(
        Vector2f xAxis,
        Vector2f yAxis,
        Vector2f zAxis) noexcept;

    /**
     * Calculates the orthographic projection matrix
     *
     * @param xAxis the x-axis depth [left, right]
     * @param yAxis the y-axis depth [bottom, top]
     * @param zAxis the z-axis depth [near, far]
     * @return the frustum orthographic matrix
     */
    [[nodiscard]] constexpr Matrix4f orthographicMatrix(
        Vector2f xAxis,
        Vector2f yAxis,
        Vector2f zAxis) noexcept;

    /**
     * Extends the 3D matrix to the 4-th dimension by
     * adding 1 at the 4-th row and 4-th column and
     * zeros at the rest of added fields
     *
     * @tparam Tp the matrix element's type
     * @param matrix the constant reference to the matrix
     * @return the extended matrix
     */
    template <class Tp>
        requires std::convertible_to<int32, Tp>
    [[nodiscard]] constexpr Matrix4<Tp> extend(
        Matrix3<Tp> const& matrix) noexcept;

    /**
     * Extends the 3D vector by adding 1 at the newly created
     * cell
     *
     * @tparam Tp the vector element's type
     * @param vector the constant reference to the vector
     * @return the extended vector
     */
    template <class Tp>
        requires std::convertible_to<int32, Tp>
    [[nodiscard]] constexpr Vector4<Tp> extend(
        Vector3<Tp> const& vector) noexcept;

    /**
     * Calculates the rotation matrix representing an extrinsic
     * rotation whose Euler angles are yaw, pitch, and roll about
     * axes x, y and z
     *
     * @tparam Tp the matrix element's type
     * @param yaw the angle of rotation around the x axis [in Rads]
     * @param pitch the angle of rotation around the y axis [in Rads]
     * @param roll the angle of rotation around the z axis [in Rads]
     * @return the rotation matrix
     */
    template <std::floating_point Tp>
    [[nodiscard]] Matrix3<Tp> rotationMatrix(
        Tp yaw,
        Tp pitch,
        Tp roll) noexcept;

    /**
     * Calculates the rotation matrix representing an extrinsic
     * rotation whose Euler angles are yaw, pitch, and roll about
     * axes x, y and z. The first element of vector is concidered to
     * be the yaw angle, the second element is concidered to be the
     * pitch angle and the last element is concidered to be the roll
     * angle
     *
     * @tparam Tp the matrix element's type
     * @param angles a constant reference to the vector object
     * containing angles
     * @return the rotation matrix
     */
    template <std::floating_point Tp>
    [[nodiscard]] Matrix3<Tp> rotationMatrix(
        Vector3<Tp> const& angles) noexcept;

    /**
     * Converts angle from the [0, 360] range to radians
     *
     * @tparam Tp the angle's type
     * @param angle an angle in the [0, 360] range
     * @return angle in radians
     */
    template <std::floating_point Tp>
    [[nodiscard]] Tp toRadians(Tp angle) noexcept;

    /**
     * Converts angle from the radians to the [0, 360] range
     *
     * @tparam Tp the angle's type
     * @param angle the angle in the radians system
     * @return angle in degrees
     */
    template <std::floating_point Tp>
    [[nodiscard]] Tp fromRadians(Tp angle) noexcept;

    /**
     * Calculates the view matrix of a camera looking at the given
     * target
     *
     * @param position the position of the camera
     * @param target the position of the target
     * @param up the y axis versor
     * @return the 'look at' matrix
     */
    [[nodiscard]] constexpr Matrix4f lookAt(
        Vector3f const& position,
        Vector3f const& target,
        Vector3f const& up = {0.f, 1.f, 0.f}) noexcept;

}

#include <MPGL/Mathematics/Systems.ipp>
#include <MPGL/Mathematics/Systems.tpp>

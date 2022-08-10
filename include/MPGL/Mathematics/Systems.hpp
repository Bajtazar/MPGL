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
     * Returns an intersection of two lines. REturns an empty optional
     * if lines are parallel
     *
     * @param firstPoint the point on the first line
     * @param firstVersor the versor of the first line
     * @param secondPoint the point on the second line
     * @param secondVersor the versor of the second line
     * @return the intersection point of the two lines
     */
    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr std::optional<Vector2<Tp>>
        intersectionOf(
            Vector2<Tp> const& firstPoint,
            Vector2<Tp> const& firstVersor,
            Vector2<Tp> const& secondPoint,
            Vector2<Tp> const& secondVersor) noexcept;

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
    [[nodiscard]] inline constexpr Vector2<Tp> cartesianToPolar(
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
    [[nodiscard]] inline constexpr Vector2<Tp> polarToCartesian(
        Vector2<Tp> const& vector) noexcept;

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
    [[nodiscard]] inline constexpr Matrix2<Tp> rotationMatrix(
        float32 angle) noexcept;

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
    [[nodiscard]] constexpr Matrix4f perspectiveMatrix(
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

}

#include <MPGL/Mathematics/Systems.ipp>
#include <MPGL/Mathematics/Systems.tpp>

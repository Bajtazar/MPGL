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

#include <numbers>
#include <limits>

namespace mpgl {

    template <std::signed_integral Tp>
    [[nodiscard]] inline constexpr Tp signum(
        Tp const& value) noexcept
    {
        return value < Tp{0} ? Tp{-1} : !value ? Tp{0} : Tp{1};
    }

    template <std::totally_ordered Tp>
    [[nodiscard]] inline constexpr bool between(
        Tp const& first,
        Tp const& second,
        Tp const& checked) noexcept
    {
        if (first < second)
            return (first <= checked) && (checked <= second);
        return (second <= checked) && (checked <= first);
    }

    template <std::totally_ordered Tp, std::size_t Size>
    [[nodiscard]] inline constexpr bool between(
        Vector<Tp, Size> const& first,
        Vector<Tp, Size> const& second,
        Vector<Tp, Size> const& checked) noexcept
    {
        for (std::size_t i = 0; i < Size; ++i)
            if (!between(first[i], second[i], checked[i]))
                return false;
        return true;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr std::optional<Vector2<Tp>>
        IntersectionOfFn::operator() (
            Vector2<Tp> const& firstPoint,
            Vector2<Tp> const& firstVersor,
            Vector2<Tp> const& secondPoint,
            Vector2<Tp> const& secondVersor) const noexcept
    {
        if (auto matrix = invert(Matrix2f{tags::transposedTag,
            firstVersor, -secondVersor}))
        {
            Vector2<Tp> pos = *matrix * (secondPoint - firstPoint);
            return firstPoint + firstVersor * pos[0];
        }
        return std::nullopt;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr std::optional<Vector3<Tp>>
        IntersectionOfFn::operator() (
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& firstVersor,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& secondVersor) const noexcept
    {
        if (auto system = findValidSystem(firstVersor, secondVersor)) {
            auto [matrix, d1, d2, dt] = *system;
            auto const result = buildResultVector(firstPoint, secondPoint, d1, d2);
            if (auto perms = lupDecomposition(matrix)) {
                auto const solution = lupSolve(matrix, *perms, result);
                if (validSolution(solution, firstPoint, firstVersor,
                    secondPoint, secondVersor, dt))
                        return {firstVersor * solution[0] + firstPoint};
            }
        }
        return std::nullopt;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr bool
        IntersectionOfFn::validSolution(
            Vector2<Tp> const& solution,
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& firstVersor,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& secondVersor,
            uint8 testDim) const noexcept
    {
        return (firstVersor[testDim] * solution[0]
                - secondVersor[testDim] * solution[1])
            == (secondPoint[testDim] - firstPoint[testDim]);
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr Vector2<Tp>
        IntersectionOfFn::buildResultVector(
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& secondPoint,
            uint8 firstDim,
            uint8 secondDim) const noexcept
    {
        return {
            secondPoint[firstDim] - firstPoint[firstDim],
            secondPoint[secondDim] - firstPoint[secondDim]
        };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr std::optional<Matrix2<Tp>>
        IntersectionOfFn::getMatrix(
            Vector3<Tp> const& firstVersor,
            Vector3<Tp> const& secondVersor,
            uint8 firstDim,
            uint8 secondDim) const noexcept
    {
        Vector2<Tp> const v1{firstVersor[firstDim], -secondVersor[firstDim]};
        Vector2<Tp> const v2{firstVersor[secondDim], -secondVersor[secondDim]};
        if (cross(v1, v2))
            return {{v1, v2}};
        return std::nullopt;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr std::optional<
        IntersectionOfFn::SystemTuple<Tp>>
    IntersectionOfFn::findValidSystem(
        Vector3<Tp> const& firstVersor,
        Vector3<Tp> const& secondVersor) const noexcept
    {
        if (auto matrix = getMatrix(firstVersor, secondVersor, 0, 1))
            return SystemTuple<Tp>{*matrix, 0, 1, 2};
        if (auto matrix = getMatrix(firstVersor, secondVersor, 0, 2))
            return SystemTuple<Tp>{*matrix, 0, 2, 1};
        if (auto matrix = getMatrix(firstVersor, secondVersor, 1, 2))
            return SystemTuple<Tp>{*matrix, 1, 2, 0};
        return std::nullopt;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr bool
        isInsideTriangle(
            Vector2<Tp> const& position,
            Vector2<Tp> const& firstVertex,
            Vector2<Tp> const& secondVertex,
            Vector2<Tp> const& thirdVertex) noexcept
    {
        Vector2<Tp> v1 = secondVertex - firstVertex;
        Vector2<Tp> v2 = thirdVertex - firstVertex;
        Tp base = cross(v1, v2);
        Tp a = (cross(position, v2) - cross(firstVertex, v2)) / base;
        Tp b = (cross(firstVertex, v1) - cross(position, v1)) / base;
        return (a >= 0.f) && (b >= 0.f) && (a + b <= 1.f);
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr bool
        isOnLine(
            Vector2<Tp> const& position,
            Vector2<Tp> const& firstVertex,
            Vector2<Tp> const& secondVertex) noexcept
    {
        if (!between(firstVertex, secondVertex, position))
            return false;
        return std::fabs(cross(position - firstVertex,
            secondVertex - firstVertex))
                < std::numeric_limits<float>::epsilon();
    }

    template <FloatConvertible Tp>
    [[nodiscard]] inline constexpr Vector3<Tp>
        planeNormalVector(
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& thirdPoint) noexcept
    {
        return cross(secondPoint - firstPoint,
            thirdPoint - secondPoint);
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr Vector4<Tp>
        planeCoefficients(
            Vector3<Tp> const& firstPoint,
            Vector3<Tp> const& secondPoint,
            Vector3<Tp> const& thirdPoint) noexcept
    {
        Vector3<Tp> normal = planeNormalVector(firstPoint,
            secondPoint, thirdPoint);
        Vector4<Tp> result = normal;
        result[3] = -dot(normal, firstPoint);
        return result;
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr bool isOnPlane(
        Vector4<Tp> const& coefficients,
        Vector3<Tp> const& point) noexcept
    {
        Vector3<Tp> normal{coefficients[0], coefficients[1],
            coefficients[2]};
        return dot(normal, point) == -coefficients[3];
    }

    template <FloatConvertible Tp>
    [[nodiscard]] constexpr Tp distance(
        Vector4<Tp> const& coefficients,
        Vector3<Tp> const& point) noexcept
    {
        Vector3<Tp> normal{coefficients[0], coefficients[1],
            coefficients[2]};
        return std::abs(dot(normal, point) + coefficients[3])
            / normal.length();
    }

    template <FloatConvertible Tp>
    [[nodiscard]] Vector2<Tp> cartesianToPolar(
        Vector2<Tp> const& vector) noexcept
    {
        return { norm(vector), std::atan2(vector[1], vector[0]) };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] Vector2<Tp> polarToCartesian(
        Vector2<Tp> const& vector) noexcept
    {
        return { vector[0] * std::cos(vector[1]),
            vector[0] * std::sin(vector[1]) };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] Vector3<Tp> sphericalToCartesian(
        Vector3<Tp> const& vector)
    {
        auto const helper = vector[0] * std::cos(vector[1]);
        return {
            helper * std::cos(vector[2]),
            helper * std::sin(vector[2]),
            vector[0] * std::sin(vector[1])
        };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] Vector3<Tp> cartesianToSpherical(
        Vector3<Tp> const& vector) noexcept
    {
        auto const radius = vector.length();
        return {
            radius,
            std::atan(vector[1] / vector[0]),
            std::asin(vector[2] / radius)
        };
    }

    template <FloatConvertible Tp>
    [[nodiscard]] Matrix2<Tp> rotationMatrix(
        Tp angle) noexcept
    {
        return {Vector2<Tp>{std::cos(angle), -std::sin(angle)},
            Vector2<Tp>{std::sin(angle), std::cos(angle)}};
    }

    template <std::size_t Rows, std::size_t Cols>
    [[nodiscard]] constexpr Matrix<std::complex<float64>, Cols, Rows>
        hermitianTranspose(
            Matrix<std::complex<float64>, Rows, Cols> const& matrix
            ) noexcept
    {
        Matrix<std::complex<float64>, Cols, Rows> output;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < Cols; ++j)
                output[j][i] = std::conj(matrix[i][j]);
        return output;
    }

    template <class Tp>
        requires std::convertible_to<int32, Tp>
    [[nodiscard]] constexpr Matrix4<Tp> extend(
        Matrix3<Tp> const& matrix) noexcept
    {
        Matrix4<Tp> result = static_cast<Matrix4<Tp>>(matrix);
        result[3][3] = static_cast<Tp>(1);
        return result;
    }

    template <class Tp>
        requires std::convertible_to<int32, Tp>
    [[nodiscard]] constexpr Vector4<Tp> extend(
        Vector3<Tp> const& vector) noexcept
    {
        Vector4<Tp> result = static_cast<Vector4<Tp>>(vector);
        result[3] = static_cast<Tp>(1);
        return result;
    }

    template <std::floating_point Tp>
    [[nodiscard]] Matrix3<Tp> rotationMatrix(
        Tp yaw,
        Tp pitch,
        Tp roll) noexcept
    {
        auto cy = std::cos(yaw), cp = std::cos(pitch), cr = std::cos(roll);
        auto sy = std::sin(yaw), sp = std::sin(pitch), sr = std::sin(roll);
        return {
            Vector3<Tp>{ cp*cr, sy*sp*cr - cy*sr, cy*sp*cr + sy*sr },
            Vector3<Tp>{ cp*sr, sy*sp*sr + cy*cr, cy*sp*sr - sy*cr },
            Vector3<Tp>{ -sp, sy*cp, cy*cp }
        };
    }

    template <std::floating_point Tp>
    [[nodiscard]] Matrix3<Tp> rotationMatrix(
        Vector3<Tp> const& angles) noexcept
    {
        return rotationMatrix(angles[0], angles[1], angles[2]);
    }

    template <std::floating_point Tp>
    [[nodiscard]] Tp toRadians(Tp angle) noexcept {
        return std::numbers::pi_v<Tp> * angle /
            static_cast<Tp>(180.);
    }

    template <std::floating_point Tp>
    [[nodiscard]] Tp fromRadians(Tp angle) noexcept {
        return static_cast<Tp>(180.) * angle /
            std::numbers::pi_v<Tp>;
    }

}

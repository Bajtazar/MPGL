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

#include <math.h>

namespace mpgl {

    [[nodiscard]] constexpr Matrix4f frustumMatrix(
        Vector2f xAxis,
        Vector2f yAxis,
        Vector2f zAxis) noexcept
    {
        Matrix4f matrix;
        float32 const ndepth = zAxis[0] - zAxis[1];
        float32 const nheight = yAxis[0] - yAxis[1];
        float32 const nwidth = xAxis[0] - xAxis[1];
        matrix[0][0] = -2.f * zAxis[0] / nwidth;
        matrix[1][1] = -2.f * zAxis[0] / nheight;
        matrix[2][2] = (zAxis[0] + zAxis[1]) / ndepth;
        matrix[3][2] = -1.f;
        matrix[0][3] = zAxis[0] * (xAxis[0] + xAxis[1]) / nwidth;
        matrix[1][3] = zAxis[0] * (yAxis[0] + yAxis[1]) / nheight;
        matrix[2][3] = 2.f * zAxis[0] * zAxis[1] / ndepth;
        return matrix;
    }

    [[nodiscard]] constexpr Matrix4f orthographicMatrix(
        Vector2f xAxis,
        Vector2f yAxis,
        Vector2f zAxis) noexcept
    {
        Matrix4f matrix;
        float32 const ndepth = zAxis[0] - zAxis[1];
        float32 const nheight = yAxis[0] - yAxis[1];
        float32 const nwidth = xAxis[0] - xAxis[1];
        matrix[0][0] = -2.f / nwidth;
        matrix[1][1] = -2.f / nheight;
        matrix[2][2] = 2.f / ndepth;
        matrix[0][3] = (xAxis[0] + xAxis[1]) / nwidth;
        matrix[1][3] = (yAxis[0] + yAxis[1]) / nheight;
        matrix[2][3] = (zAxis[0] + zAxis[1]) / ndepth;
        matrix[3][3] = 1.f;
        return matrix;
    }

    [[nodiscard]] constexpr Matrix4f lookAt(
        Vector3f const& position,
        Vector3f const& target,
        Vector3f const& up) noexcept
    {
        Vector3f const forward = (target - position).normalize();
        Vector3f const right = (cross(forward, up)).normalize();
        Vector3f const top = (cross(right, forward)).normalize();
        Matrix4f lookAt = extend(Matrix3f{right, top, -forward});
        lookAt[0][3] = -dot(right, position);
        lookAt[1][3] = -dot(top, position);
        lookAt[2][3] = -dot(forward, position);
        return lookAt;
    }

}

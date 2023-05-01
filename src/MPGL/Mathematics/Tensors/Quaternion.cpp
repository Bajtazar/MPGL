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
#include <MPGL/Mathematics/Tensors/Quaternion.hpp>

namespace mpgl {

    template class Quaternion<float32>;
    template class Quaternion<float64>;
    template class Quaternion<uint32>;

    template Quaternion4d quaternionCast(
        Quaternion4f const&) noexcept;

    template Quaternion4f quaternionCast(
        Quaternion4u const&) noexcept;

    template Vector3f rotate(
        Vector3f const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f rotationQuaternion(
        float32,
        Vector3f const&) noexcept;

    template Vector3f rotateWithQuaternion(
        Vector3f const&,
        float32,
        Vector3f const&) noexcept;

    template Quaternion4f floor(Quaternion4f const&) noexcept;

    template Quaternion4f ceil(Quaternion4f const&) noexcept;

    template Quaternion4f round(Quaternion4f const&) noexcept;

    template Quaternion4f normalize(Quaternion4f const&);

    template Quaternion4f conjugate(Quaternion4f const&);

    template Quaternion4f invert(Quaternion4f const&);

    template Quaternion4f operator+(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator-(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator*(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator/(
        Quaternion4f const&,
        Quaternion4f const&);

    template Quaternion4f operator+(
        Quaternion4f const&,
        float32 const&) noexcept;

    template Quaternion4f operator-(
        Quaternion4f const&,
        float32 const&) noexcept;

    template Quaternion4f operator*(
        Quaternion4f const&,
        float32 const&) noexcept;

    template Quaternion4f operator/(
        Quaternion4f const&,
        float32 const&);

    template Quaternion4f operator+(
        float32 const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator-(
        float32 const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator*(
        float32 const&,
        Quaternion4f const&) noexcept;

    template Quaternion4f operator/(
        float32 const&,
        Quaternion4f const&);

    template std::partial_ordering totalCompare(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

    template bool operator==(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

    template std::partial_ordering operator<=>(
        Quaternion4f const&,
        Quaternion4f const&) noexcept;

}

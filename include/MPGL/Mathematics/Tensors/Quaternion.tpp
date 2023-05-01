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

#include <cmath>

namespace mpgl {

template <Arithmetic Tp>
constexpr Quaternion<Tp>::Quaternion(Vector4<Tp> vector) noexcept
    : vector{std::move(vector)} {}

template <Arithmetic Tp>
constexpr Quaternion<Tp>::Quaternion(
    const Vector3<Tp>& imaginary,
    const Tp& real) noexcept
        : vector{real, imaginary[0], imaginary[1], imaginary[2]} {}

template <Arithmetic Tp>
constexpr Quaternion<Tp>::Quaternion(
    const Tp& real,
    const Tp& iPart,
    const Tp& jPart,
    const Tp& kPart) noexcept : vector{real, iPart, jPart, kPart} {}

template <Arithmetic Tp>
[[nodiscard]] constexpr std::size_t Quaternion<Tp>::size(
    void) noexcept
{
    return 4;
}

template <Arithmetic Tp>
template <Arithmetic Up>
[[nodiscard]] constexpr Up Quaternion<Tp>::length(void) const noexcept {
    return vector.length();
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::normalize(void) noexcept {
    vector.normalize();
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::conjugate(void) noexcept {
    for (uint8 i = 1;i < 4; ++i)
        vector[i] = -vector[i];
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::invert(void) noexcept {
    return conjugate() /= dot(vector, vector);
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator+=(
    Quaternion const& right)
{
    vector += right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator-=(
    Quaternion const& right)
{
    vector -= right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator*=(
    Quaternion const& right)
{
    auto const& [a1, b1, c1, d1] = vector;
    auto const& [a2, b2, c2, d2] = right.vector;

    return *this = Quaternion{
        a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2,
        a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2,
        a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2,
        a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2
    };
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator/=(
    Quaternion const& right)
{
    return *this *= mpgl::invert(right);
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator%=(
    Quaternion const& right) requires mpgl_Operable(Tp, %)
{
    vector %= right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator^=(
    Quaternion const& right) requires mpgl_Operable(Tp, ^)
{
    vector ^= right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator&=(
    Quaternion const& right) requires mpgl_Operable(Tp, &)
{
    vector &= right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator|=(
    Quaternion const& right) requires mpgl_Operable(Tp, |)
{
    vector |= right.vector;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator+=(Tp const& right) {
    vector += right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator-=(Tp const& right) {
    vector -= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator*=(Tp const& right) {
    vector *= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator/=(Tp const& right) {
    vector /= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator%=(
    Tp const& right) requires mpgl_Operable(Tp, %)
{
    vector %= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator^=(
    Tp const& right) requires mpgl_Operable(Tp, ^)
{
    vector ^= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator&=(
    Tp const& right) requires mpgl_Operable(Tp, &)
{
    vector &= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator|=(
    Tp const& right) requires mpgl_Operable(Tp, |)
{
    vector |= right;
    return *this;
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::operator std::span<Tp, 4>(
    ) noexcept
{
    return std::span<Tp, 4>{vector};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::operator std::span<Tp const, 4>(
    ) const noexcept
{
    return std::span<Tp const, 4>{vector};
}

template <Arithmetic Tp>
template <Arithmetic Up>
    requires std::convertible_to<Tp, Up>
[[nodiscard]] constexpr Quaternion<Tp>::operator Quaternion<Up>(
    ) const noexcept
{
    return Quaternion<Up>{vectorCast<Up>(vector)};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Vector4<Tp> Quaternion<Tp>::asVector(
    void) const noexcept
{
    return vector;
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Tp Quaternion<Tp>::real(void) const noexcept {
    return vector[0];
}

template <Arithmetic Tp>
constexpr void Quaternion<Tp>::real(
    Tp value) noexcept
{
    vector[0] = std::move(value);
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Vector3<Tp> Quaternion<Tp>::imaginary(
    void) const noexcept
{
    return Vector3<Tp>{vector[1], vector[2], vector[3]};
}

template <Arithmetic Tp>
constexpr void Quaternion<Tp>::imaginary(const Vector3<Tp>& value) noexcept
{
    for (uint8 i = 0;i < 3; ++i)
        vector[i + 1] = value[i];
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> Quaternion<Tp>::operator-(
    void) const noexcept
{
    return Quaternion<Tp>{-vector};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::iterator Quaternion<Tp>::begin(
    void) noexcept
{
    return vector.begin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::iterator Quaternion<Tp>::end(
    void) noexcept
{
    return vector.end();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_iterator
    Quaternion<Tp>::begin(void) const noexcept
{
    return vector.begin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_iterator
    Quaternion<Tp>::end(void) const noexcept
{
    return vector.end();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_iterator
    Quaternion<Tp>::cbegin(void) const noexcept
{
    return vector.cbegin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_iterator
    Quaternion<Tp>::cend(void) const noexcept
{
    return vector.cend();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::reverse_iterator
    Quaternion<Tp>::rbegin(void) noexcept
{
    return vector.rbegin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::reverse_iterator
    Quaternion<Tp>::rend(void) noexcept
{
    return vector.rend();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reverse_iterator
    Quaternion<Tp>::rbegin(void) const noexcept
{
    return vector.rbegin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reverse_iterator
    Quaternion<Tp>::rend(void) const noexcept
{
    return vector.rend();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reverse_iterator
    Quaternion<Tp>::crbegin(void) const noexcept
{
    return vector.crbegin();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reverse_iterator
    Quaternion<Tp>::crend(void) const noexcept
{
    return vector.crend();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::reference
    Quaternion<Tp>::operator[] (std::size_t index) noexcept
{
    return vector[index];
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reference
    Quaternion<Tp>::operator[] (std::size_t index) const noexcept
{
    return vector[index];
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::reference
    Quaternion<Tp>::at(std::size_t index)
{
    return vector.at(index);
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::const_reference
    Quaternion<Tp>::at(std::size_t index) const
{
    return vector.at(index);
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Tp* Quaternion<Tp>::data(void) noexcept {
    return vector.data();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Tp const* Quaternion<Tp>::data(
    void) const noexcept
{
    return vector.data();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator+(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept
{
    return {left.value + right.value};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator-(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept
{
    return {left.value - right.value};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator*(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept
{
    auto const& [a1, b1, c1, d1] = left.vector;
    auto const& [a2, b2, c2, d2] = right.vector;

    return {
        a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2,
        a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2,
        a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2,
        a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2
    };
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator/(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept
{
    return {left.value * invert(right)};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator%(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, %)
{
    return {left.value % right.value};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator^(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, ^)
{
    return {left.value ^ right.value};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator&(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, &)
{
    return {left.value & right.value};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> operator|(
    Quaternion<Tp> const& left,
    Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, |)
{
    return {left.value | right.value};
}

template <Arithmetic Up, Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Up>
    quaternionCast(Quaternion<Tp> const& quaternion) noexcept
{
    return static_cast<Quaternion<Up>>(quaternion);
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Vector3<Tp> rotate(
    Vector3<Tp> const& vector,
    Quaternion<Tp> const& quaternion) noexcept
{
    return (quaternion * Quaternion<Tp>{vector} * conjugate(quaternion)
        ).imaginary();
}

template <Arithmetic Tp>
[[nodiscard]] Quaternion<Tp> rotationQuaternion(
    Tp angle,
    Vector3<Tp> const& axis) noexcept
{
    return {axis, std::cos(angle) / static_cast<Tp>(2)};
}

template <Arithmetic Tp>
[[nodiscard]] Vector3<Tp> rotateWithQuaternion(
    Vector3<Tp> const& vector,
    Tp angle,
    Vector3<Tp> const& axis) noexcept
{
    auto const temp = static_cast<Tp>(2) * cross(axis, vector);
    auto const real = std::cos(angle) / static_cast<Tp>(2);
    return vector + real * temp + cross(axis, temp);
}

template <std::floating_point Tp>
[[nodiscard]] constexpr Quaternion<Tp> floor(
    Quaternion<Tp> quaternion)
{
    return {
        std::floor(quaternion[0]),
        std::floor(quaternion[1]),
        std::floor(quaternion[2]),
        std::floor(quaternion[3])
    };
}

template <std::floating_point Tp>
[[nodiscard]] constexpr Quaternion<Tp> ceil(
    Quaternion<Tp> quaternion)
{
    return {
        std::ceil(quaternion[0]),
        std::ceil(quaternion[1]),
        std::ceil(quaternion[2]),
        std::ceil(quaternion[3])
    };
}

template <std::floating_point Tp>
[[nodiscard]] constexpr Quaternion<Tp> round(
    Quaternion<Tp> quaternion)
{
    return {
        std::round(quaternion[0]),
        std::round(quaternion[1]),
        std::round(quaternion[2]),
        std::round(quaternion[3])
    };
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> normalize(
    Quaternion<Tp> const& quaternion)
{
    auto temp = quaternion;
    return temp.nomalize();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> conjugate(
    Quaternion<Tp> const& quaternion)
{
    auto temp = quaternion;
    return temp.conjugate();
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> invert(
    Quaternion<Tp> const& quaternion)
{
    auto temp = quaternion;
    return temp.invert();
}

}

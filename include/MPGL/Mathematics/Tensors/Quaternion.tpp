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

namespace mpgl {

template <Arithmetic Tp>
template <Arithmetic Up>
[[nodiscard]] constexpr Up Quaternion<Tp>::length(void) const noexcept {
    return _M_data.length();
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::normalize(void) noexcept {
    _M_data.normalize();
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::conjugate(void) noexcept {
    for (uint8 i = 1;i < 4; ++i)
        _M_data[i] = -_M_data[i];
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::invert(void) const noexcept {
    return conjugate() /= dot(_M_data, _M_data);
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator+=(
    Quaternion const& right)
{
    _M_data += right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator-=(
    Quaternion const& right)
{
    _M_data -= right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator*=(
    Quaternion const& right)
{
    auto const& [a1, b1, c1, d1] = _M_data;
    auto const& [a2, b2, c2, d2] = right._M_data;

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
    return *this *= ::invert(right);
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator%=(
    Quaternion const& right) requires mpgl_Operable(Tp, %)
{
    _M_data %= right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator^=(
    Quaternion const& right) requires mpgl_Operable(Tp, ^)
{
    _M_data ^= right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator&=(
    Quaternion const& right) requires mpgl_Operable(Tp, &)
{
    _M_data &= right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator|=(
    Quaternion const& right) requires mpgl_Operable(Tp, |)
{
    _M_data |= right._M_data;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator+=(Tp const& right) {
    _M_data += right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator-=(Tp const& right) {
    _M_data -= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator*=(Tp const& right) {
    _M_data *= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator/=(Tp const& right) {
    _M_data /= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator%=(
    Tp const& right) requires mpgl_Operable(Tp, %)
{
    _M_data %= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator^=(
    Tp const& right) requires mpgl_Operable(Tp, ^)
{
    _M_data ^= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator&=(
    Tp const& right) requires mpgl_Operable(Tp, &)
{
    _M_data &= right;
    return *this;
}

template <Arithmetic Tp>
constexpr Quaternion<Tp>& Quaternion<Tp>::operator|=(
    Tp const& right) requires mpgl_Operable(Tp, |)
{
    _M_data |= right;
    return *this;
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::operator std::span<Tp, 4>(
    ) noexcept
{
    return std::span<Tp, 4>{_M_data};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp>::operator std::span<Tp const, 4>(
    ) const noexcept
{
    return std::span<Tp const, 4>{_M_data};
}

template <Arithmetic Tp>
template <Arithmetic Up>
    requires std::convertible_to<Tp, Up>
[[nodiscard]] constexpr Quaternion<Tp>::operator Quaternion<Up>(
    ) const noexcept
{
    return Quaternion<Up>{vectorCast<Up>(_M_data)};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Vector4<Tp> Quaternion<Tp>::asVector(
    void) const noexcept
{
    return _M_data;
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Tp Quaternion<Tp>::real(void) const noexcept {
    return _M_data[0];
}

template <Arithmetic Tp>
[[nodiscard]] constexpr void Quaternion<Tp>::real(
    Tp value) noexcept
{
    return _M_data[0] = std::move(value);
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Vector3<Tp> Quaternion<Tp>::imaginary(
    void) const noexcept
{
    return Vector3f{_M_data[1], _M_data[2], _M_data[3]};
}

template <Arithmetic Tp>
[[nodiscard]] constexpr void Quaternion<Tp>::imaginary(
    const Vector3<Tp>& value) noexcept
{
    for (uint8 i = 0;i < 3; ++i)
        _M_data[i + 1] = value[i];
}

template <Arithmetic Tp>
[[nodiscard]] constexpr Quaternion<Tp> Quaternion<Tp>::operator-(
    void) const noexcept
{
    return Quaternion<Tp>{-_M_data};
}

}

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

#include <MPGL/Mathematics/Tensors/Vector.hpp>

namespace mpgl {

    /**
     * Represents the mathematical quaterion in the memory
     *
     * @tparam Tp the quaterion's value type
     */
    template <Arithmetic Tp>
    struct Quaternion {
        using value_type = typename Vector4<Tp>::value_type;
        using reference = typename Vector4<Tp>::reference;
        using const_reference = typename Vector4<Tp>::const_reference;

        /**
         * Constructs a new quaternion object
        */
        constexpr Quaternion(void) noexcept = default;

        /**
         * Constructs a new quaternion object
         *
         * @param vector a vector from which the quaternion
         * will be created
        */
        constexpr Quaternion(Vector4<Tp> vector) noexcept;

        /**
         * Constructs a new quaternion object
         *
         * @param imaginary a constant reference to the vector
         * representing an imaginary part of the quaternion
         * @param real a constant reference to the scalar
         * representing a real part of the quaternion
        */
        constexpr Quaternion(
            const Vector3<Tp>& imaginary,
            const Tp& real = Tp{}) noexcept;

        /**
         * Constructs a new quaternion object
         *
         * @param real the real part
         * @param iPart the i imaginary part
         * @param jPart the j imaginary part
         * @param kPart the k imaginary part
        */
        constexpr Quaternion(
            const Tp& real,
            const Tp& iPart,
            const Tp& jPart,
            const Tp& kPart) noexcept;

        /**
         * Returns the size of the quaterion [always 4]
         *
         * @return the size of the quaterion
         */
        [[nodiscard]] static constexpr std::size_t size(
            void) noexcept;

        /**
         * Returns the length of the quaterion [equivalent
         * to the square norm in the euclidean space]
         *
         * @tparam Up the result type
         * @return the length of the quaterion
         */
        template <Arithmetic Up = Tp>
        [[nodiscard]] constexpr Up length(void) const noexcept;

        /**
         * Normalizes quaternion
         *
         * @return a reference to this object
         */
        constexpr Quaternion& normalize(void) noexcept;

        /**
         * Conjugates quaternion
         *
         * @return a reference to this object
        */
        constexpr Quaternion& conjugate(void) noexcept;

        /**
         * Calculates a multiplicative inversions of this quaterion
         *
         * @return a reference to this object
        */
        constexpr Quaternion& invert(void) noexcept;

        /**
         * Adds the elements of this quaternion with
         * the given quaternion elements
         *
         * @param right the added quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator+=(Quaternion const& right) noexcept;

        /**
         * Subtracts the elements of this quaternion with
         * the given quaternion elements
         *
         * @param right the subtracted quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator-=(Quaternion const& right) noexcept;

        /**
         * Multiplies two quaternions together
         *
         * @param right the multiplicand quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator*=(Quaternion const& right) noexcept;

        /**
         * Multiplies this quaternion with the reciprocal of the
         * second one
         *
         * @param right the divider quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator/=(Quaternion const& right);

        /**
         * Calculates the modulo of elements of this quaternion
         * and the given quaternion elements
         *
         * @param right the quaternion with modulos
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator%=(Quaternion const& right)
            requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * quaternion and the given quaternion elements
         *
         * @param right the quaternion with values
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator^=(Quaternion const& right)
            noexcept requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this quaternion
         * and the given quaternion elements
         *
         * @param right the quaternion with values
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator&=(Quaternion const& right)
            noexcept requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this quaternion
         * and the given quaternion elements
         *
         * @param right the quaternion with values
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator|=(Quaternion const& right)
            noexcept requires mpgl_Operable(Tp, |);

        /**
         * Adds the given scalar value to the elements of
         * this quaternion
         *
         * @param right the added scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator+=(Tp const& right) noexcept;

        /**
         * Subtracts the given scalar value to the elements of
         * this quaternion
         *
         * @param right the subtracted scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator-=(Tp const& right) noexcept;

        /**
         * Multiplies the given scalar value to the elements of
         * this quaternion
         *
         * @param right the multiplied scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator*=(Tp const& right) noexcept;

        /**
         * Divides the elements of this quaternion by the given
         * scalar value
         *
         * @param right the divisor scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator/=(Tp const& right);

        /**
         * Calculates the modulo of elements of this quaternion
         * with the given scalar
         *
         * @param right the modulo scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator%=(Tp const& right)
            requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * quaternion and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator^=(Tp const& right)
            noexcept requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this
         * quaternion and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator&=(Tp const& right)
            noexcept requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this
         * quaternion and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator|=(Tp const& right)
            noexcept requires mpgl_Operable(Tp, |);

        /**
         * Returns a std::span that holds view to this quaternion
         *
         * @return the std::span that holds view to this quaternion
         */
        [[nodiscard]] constexpr operator std::span<Tp, 4>(
            ) noexcept;

        /**
         * Returns a std::span that holds view to this quaternion
         *
         * @return the std::span that holds view to this quaternion
         */
        [[nodiscard]] constexpr operator std::span<Tp const, 4>(
            ) const noexcept;

        /**
         * Casts the quaternion to the given type
         *
         * @tparam Up the new quaternion's element type
         * @return the casted quaternion
         */
        template <Arithmetic Up>
            requires std::convertible_to<Tp, Up>
        [[nodiscard]] constexpr operator Quaternion<Up>(
            ) const noexcept;

        /**
         * Returns a quaternion value as a Vector4
         *
         * @return a Vector4 containing the current quaternion's value
        */
        [[nodiscard]] constexpr Vector4<Tp> asVector(void) const noexcept;

        /**
         * Returns a real part of the quaternion
         *
         * @return a reference to the real part of the quaternion
        */
        [[nodiscard]] constexpr Tp real(void) const noexcept;

        /**
         * Sets a real part of the quaternion
         *
         * @param value the real part of the quaternion
        */
        constexpr void real(Tp value) noexcept;

        /**
         * Returns an imaginary part of the quaternion
         *
         * @return the imaginary part of the quaternion
        */
        [[nodiscard]] constexpr Vector3<Tp> imaginary(void) const noexcept;

        /**
         * Sets an imaginary part of the quaternion
         *
         * @param value the imaginary part of the quaternion
        */
        constexpr void imaginary(const Vector3<Tp>& value) noexcept;

        /**
         * Inverses the sign of the quaternion's elements
         *
         * @return the quaternion with sign-inversed elements
         */
        [[nodiscard]] constexpr Quaternion operator-(void) const noexcept;

        using iterator = typename Vector4<Tp>::iterator;
        using const_iterator = typename Vector4<Tp>::const_iterator;
        using reverse_iterator
            = typename Vector4<Tp>::reverse_iterator;
        using const_reverse_iterator
            = typename Vector4<Tp>::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the quaternion
         *
         * @return the iterator to the begining of the quaternion
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept;

        /**
         * Returns the iterator to the end of the quaternion
         *
         * @return the iterator to the end of the quaternion
         */
        [[nodiscard]] constexpr iterator end(void) noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the quaternion
         *
         * @return the constant iterator to the begining
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the quaternion
         *
         * @return the constant iterator to the end
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the quaternion
         *
         * @return the constant iterator to the begining
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the quaternion
         *
         * @return the constant iterator to the end
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept;

        /**
         * Returns the reverse iterator to the end of
         * the quaternion
         *
         * @return the reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(
            void) noexcept;

        /**
         * Returns the reverse iterator to the begining of
         * the quaternion
         *
         * @return the reverse iterator to the begining of
         * the quaternion
         */
        [[nodiscard]] constexpr reverse_iterator rend(
            void) noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the quaternion
         *
         * @return the constant reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the
         * begining of the quaternion
         *
         * @return the constant reverse iterator to the
         * begining of the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the quaternion
         *
         * @return the constant reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the
         * begining of the quaternion
         *
         * @return the constant reverse iterator to the
         * begining of the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator crend(
            void) const noexcept;

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference operator[] (
            std::size_t index) noexcept;

        /**
         * Returns the constant reference to element with
         * the given index
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        [[nodiscard]] constexpr const_reference operator[] (
            std::size_t index) const noexcept;

        /**
         * Returns the reference to element with the given index
         * @throws std::out_of_range when the index is out of the
         * quaternion's scope
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference at(
            std::size_t index);

        /**
         * Returns the constant reference to element with
         * the given index
         * @throws std::out_of_range when the index is out of the
         * quaternion's scope
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        [[nodiscard]] constexpr const_reference at(
            std::size_t index) const;

        /**
         * Returns a reference to the quaternion's element with
         * the given index
         *
         * @tparam N the element's index
         * @return the reference to the element with
         * the given index
         */
        template<std::size_t N>
            requires (N < 4)
        [[nodiscard]] constexpr std::tuple_element_t<N, Quaternion>&
            get(void) & noexcept
                { return _M_data[N]; }

        /**
         * Returns a constant reference to the quaternion's element
         * with the given index
         *
         * @tparam N the element's index
         * @return the constant reference to the element with
         * the given index
         */
        template<std::size_t N>
            requires (N < 4)
        [[nodiscard]] constexpr
            std::tuple_element_t<N, Quaternion> const& get(
                void) const& noexcept
                    { return _M_data[N]; }

        /**
         * Returns a rvalue reference to the quaternion's element
         * with the given index
         *
         * @tparam N the element's index
         * @return the rvalue reference to the element with
         * the given index
         */
        template<std::size_t N>
            requires (N < 4)
        [[nodiscard]] constexpr std::tuple_element_t<N, Quaternion>&&
            get(void) && noexcept
                { return std::move(_M_data[N]); }

        /**
         * Returns a const rvalue reference to the quaternion's element
         * with the given index
         *
         * @tparam N the element's index
         * @return the const rvalue reference to the element with
         * the given index
         */
        template<std::size_t N>
            requires (N < 4)
        [[nodiscard]] constexpr std::tuple_element_t<N, Quaternion> const&&
            get(void) const && noexcept
                { return std::move(_M_data[N]); }

        /**
         * Returns a poiner to the handled memory
         *
         * @return the poiner to the handled memory
         */
        [[nodiscard]] constexpr Tp* data(void) noexcept;

        /**
         * Returns a constant poiner to the handled memory
         *
         * @return the constant poiner to the handled memory
         */
        [[nodiscard]] constexpr Tp const* data(void) const noexcept;

        Vector4<Tp> _M_data;
    };

    /**
     * Casts the quaternion elements to the given type
     *
     * @tparam Up the new quaternion's value type
     * @tparam Tp the old quaternion's value type
     * @param quaternion the constant reference to the quaternion
     * @return the casted quaternion
     */
    template <Arithmetic Up, Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Up>
        quaternionCast(Quaternion<Tp> const& quaternion) noexcept;

    /**
     * Rotates a 3D vector using a quaternion
     *
     * @tparam Tp the element's value type
     * @param vector a constant reference to the rotated vector
     * @param quaternion a constant reference to the rotation
     * quaternion
     * @return the rotated vector
    */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Vector3<Tp> rotate(
        Vector3<Tp> const& vector,
        Quaternion<Tp> const& quaternion) noexcept;

    /**
     * Creates a rotation quaternion
     *
     * @tparam Tp the element's value type
     * @param angle a rotation angle
     * @param axis a rotation axis
     * @return the rotation quaternion
    */
    template <Arithmetic Tp>
    [[nodiscard]] Quaternion<Tp> rotationQuaternion(
        Tp angle,
        Vector3<Tp> const& axis) noexcept;

    /**
     * Rotates a 3D vector by a given angle around the given axis
     * using a quaternion arithmetic
     *
     * @tparam Tp the element's type
     * @param vector a constant reference to the rotated vector
     * @param angle a rotation angle
     * @param axis a rotation axis
     * @return the rotated vector
    */
    template <Arithmetic Tp>
    [[nodiscard]] Vector3<Tp> rotateWithQuaternion(
        Vector3<Tp> const& vector,
        Tp angle,
        Vector3<Tp> const& axis) noexcept;

    /**
     * Returns the quaternion with the floored values
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the floored quaternion
     */
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> floor(
        Quaternion<Tp> quaternion);

    /**
     * Returns the quaternion with the ceiled values
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the ceiled quaternion
     */
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> ceil(
        Quaternion<Tp> quaternion);

    /**
     * Returns the quaternion with the rounded values
     *
     * @tparam Tp the quaternion's type
     * @tparam Size the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the rounded quaternion
     */
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> round(
        Quaternion<Tp> quaternion);

    /**
     * Normalizes quaternion
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the normalized quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> normalize(
        Quaternion<Tp> const& quaternion);

    /**
     * Conjugates a quaternion
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the normalized quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> conjugate(
        Quaternion<Tp> const& quaternion);

    /**
     * Inverts a quaternion
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the normalized quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> invert(
        Quaternion<Tp> const& quaternion);

    /**
     * Adds two quaternions together
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Subtracts quaternion from other quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Multiplies two quaternions together
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Divides the elements of one quaternion by the elements
     * of another
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator/(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right);

    /**
     * Calculates the modulo of the quaternion and the other quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator%(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the quaternion and the other
     * quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the quaternion and the other
     * quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion and the other
     * quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept;

    /**
     * Subtracts the scalar value from the quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept;

    /**
     * Multiplies the quaternion with the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept;

    /**
     * Divides the quaternion consisting of scalar value fields
     * by the quaternion elements
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator/(
        Quaternion<Tp> const& left,
        Tp const& right);

    /**
     * Calculates the modulo of the quaternion consisting of scalar
     * values with the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator%(
        Quaternion<Tp> const& left,
        Tp const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Quaternion<Tp> const& left,
        Tp const& right) noexcept requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Subtracts the scalar value from the quaternion
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Multiplies the quaternion with the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Divides the quaternion consisting of scalar value fields
     * by the quaternion elements
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator/(
        Tp const& left,
        Quaternion<Tp> const& right);

    /**
     * Calculates the modulo of the quaternion consisting of scalar
     * values with the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator%(
        Tp const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Tp const& left,
        Quaternion<Tp> const& right) noexcept requires mpgl_Operable(Tp, |);

    /**
     * Compares two quaternions in the lexicographical order
     *
     * @tparam Tp the quaternion's type
     * @tparam Size the quaternion's size
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the ordering result
     */
    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::weak_ordering>
    [[nodiscard]] constexpr std::weak_ordering lexicographicalCompare(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Compares two quaternions in the total order
     *
     * @tparam Tp the quaternion's typee
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the ordering result
     */
    template <Arithmetic Tp>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] constexpr std::partial_ordering totalCompare(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Checks whether two quaternions are equivalent in the total
     * order
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return if the quaternions are equals
     */
    template <Arithmetic Tp>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] bool constexpr operator== (
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    /**
     * Compares two quaternions in the total order
     *
     * @tparam Tp the quaternion's type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the ordering result
     */
    template <Arithmetic Tp>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] std::partial_ordering constexpr operator<=> (
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) noexcept;

    extern template class Quaternion<float32>;
    extern template class Quaternion<float64>;
    extern template class Quaternion<uint32>;

    typedef Quaternion<float32>                     Quaternion4f;
    typedef Quaternion<float64>                     Quaternion4d;
    typedef Quaternion<uint32>                      Quaternion4u;

    template Quaternion4f operator+(
            Quaternion4f const& left,
            float32 const& right) noexcept;

}

#include <MPGL/Mathematics/Tensors/Quaternion.tpp>

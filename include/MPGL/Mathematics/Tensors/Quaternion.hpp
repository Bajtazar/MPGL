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
         * Returns the size of the quaterion [always 4]
         *
         * @return the size of the quaterion
         */
        [[nodiscard]] static consteval std::size_t size(
            void) noexcept
                { return 4; }

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
        constexpr Quaternion& invert(void) const noexcept;

        /**
         * Adds the elements of this quaternion with
         * the given quaternion elements
         *
         * @param right the added quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator+=(Quaternion const& right);

        /**
         * Subtracts the elements of this quaternion with
         * the given quaternion elements
         *
         * @param right the subtracted quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator-=(Quaternion const& right);

        /**
         * Multiplies two quaternions together
         *
         * @param right the multiplicand quaternion
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator*=(Quaternion const& right);

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
            requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this quaternion
         * and the given quaternion elements
         *
         * @param right the quaternion with values
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator&=(Quaternion const& right)
            requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this quaternion
         * and the given quaternion elements
         *
         * @param right the quaternion with values
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator|=(Quaternion const& right)
            requires mpgl_Operable(Tp, |);

        /**
         * Adds the given scalar value to the elements of
         * this quaternion
         *
         * @param right the added scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator+=(Tp const& right);

        /**
         * Subtracts the given scalar value to the elements of
         * this quaternion
         *
         * @param right the subtracted scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator-=(Tp const& right);

        /**
         * Multiplies the given scalar value to the elements of
         * this quaternion
         *
         * @param right the multiplied scalar
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator*=(Tp const& right);

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
            requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this
         * quaternion and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator&=(Tp const& right)
            requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this
         * quaternion and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this quaternion
         */
        constexpr Quaternion& operator|=(Tp const& right)
            requires mpgl_Operable(Tp, |);

        /**
         * Returns a std::span that holds view to this quaternion
         *
         * @return the std::span that holds view to this quaternion
         */
        [[nodiscard]] constexpr operator std::span<Tp const, Size>(
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
         * @return the real part of the quaternion
        */
        [[nodiscard]] constexpr Tp real(void) const noexcept;

        /**
         * Returns an imaginary part of the quaternion
         *
         * @return the imaginary part of the quaternion
        */
        [[nodiscard]] constexpr Vector3<Tp> imaginary(void) const noexcept;

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
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return _M_data.begin(); }

        /**
         * Returns the iterator to the end of the quaternion
         *
         * @return the iterator to the end of the quaternion
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return _M_data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the quaternion
         *
         * @return the constant iterator to the begining
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return _M_data.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the quaternion
         *
         * @return the constant iterator to the end
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return _M_data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the quaternion
         *
         * @return the constant iterator to the begining
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return _M_data.cbegin(); }

        /**
         * Returns the constant iterator to the end
         * of the quaternion
         *
         * @return the constant iterator to the end
         * of the quaternion
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return _M_data.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the quaternion
         *
         * @return the reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(
            void) noexcept
                { return _M_data.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the quaternion
         *
         * @return the reverse iterator to the begining of
         * the quaternion
         */
        [[nodiscard]] constexpr reverse_iterator rend(
            void) noexcept
                { return _M_data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the quaternion
         *
         * @return the constant reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept
                { return _M_data.rbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the quaternion
         *
         * @return the constant reverse iterator to the
         * begining of the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept
                { return _M_data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the quaternion
         *
         * @return the constant reverse iterator to the end of
         * the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept
                { return _M_data.crbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the quaternion
         *
         * @return the constant reverse iterator to the
         * begining of the quaternion
         */
        [[nodiscard]] constexpr const_reverse_iterator crend(
            void) const noexcept
                { return _M_data.crend(); }

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference operator[] (
            std::size_t index) noexcept
                { return _M_data[index]; }

        /**
         * Returns the constant reference to element with
         * the given index
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        [[nodiscard]] constexpr const_reference operator[] (
            std::size_t index) const noexcept
                { return _M_data[index]; }

        /**
         * Returns the reference to element with the given index
         * @throws std::out_of_range when the index is out of the
         * quaternion's scope
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference at(
            std::size_t index)
                { return _M_data.at(index); }

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
            std::size_t index) const
                { return _M_data.at(index); }

        /**
         * Returns a reference to the quaternion's element with
         * the given index
         *
         * @tparam N the element's index
         * @return the reference to the element with
         * the given index
         */
        template<std::size_t N>
            requires (N < Size)
        [[nodiscard]] constexpr std::tuple_element_t<N, Vector>&
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
            requires (N < Size)
        [[nodiscard]] constexpr
            std::tuple_element_t<N, Vector> const& get(
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
            requires (N < Size)
        [[nodiscard]] constexpr std::tuple_element_t<N, Vector>&&
            get(void) && noexcept
                { return std::move(_M_data[N]); }

        /**
         * Returns a poiner to the handled memory
         *
         * @return the poiner to the handled memory
         */
        [[nodiscard]] Tp* data(void) noexcept
            { return _M_data.data(); }

        /**
         * Returns a constant poiner to the handled memory
         *
         * @return the constant poiner to the handled memory
         */
        [[nodiscard]] Tp const* data(void) const noexcept
            { return _M_data.data(); }

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
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> normalize(
        Quaternion<Tp> const& quaternion);

    /**
     * Conjugates a quaternion
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the normalized quaternion
     */
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> conjugate(
        Quaternion<Tp> const& quaternion);

    /**
     * Inverts a quaternion
     *
     * @tparam Tp the quaternion's type
     * @param quaternion the constant reference to the quaternion
     * @return the normalized quaternion
     */
    template <std::floating_point Tp>
    [[nodiscard]] constexpr Quaternion<Tp> invert(
        Quaternion<Tp> const& quaternion);

    /**
     * Adds two vectors together
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right);

    /**
     * Subtracts quaternion from other quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right);

    /**
     * Multiplies two vectors together
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right);

    /**
     * Divides the elements of one quaternion by the elements
     * of another
     *
     * @tparam Tp the quaternion's value type
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
     * @tparam Tp the quaternion's value type
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
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the quaternion and the other
     * quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion and the other
     * quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the left quaternion
     * @param right the constant reference to the right quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Quaternion<Tp> const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Quaternion<Tp> const& left,
        Tp const& right);

    /**
     * Subtracts the scalar value from the quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Quaternion<Tp> const& left,
        Tp const& right);

    /**
     * Multiplies the quaternion with the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Quaternion<Tp> const& left,
        Tp const& right);

    /**
     * Divides the quaternion consisting of scalar value fields
     * by the quaternion elements
     *
     * @tparam Tp the quaternion's value type
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
     * @tparam Tp the quaternion's value type
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
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Quaternion<Tp> const& left,
        Tp const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Quaternion<Tp> const& left,
        Tp const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the quaternion
     * @param right the constant reference to the scalar value
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Quaternion<Tp> const& left,
        Tp const& right) requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator+(
        Tp const& left,
        Quaternion<Tp> const& right);

    /**
     * Subtracts the scalar value from the quaternion
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator-(
        Tp const& left,
        Quaternion<Tp> const& right);

    /**
     * Multiplies the quaternion with the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator*(
        Tp const& left,
        Quaternion<Tp> const& right);

    /**
     * Divides the quaternion consisting of scalar value fields
     * by the quaternion elements
     *
     * @tparam Tp the quaternion's value type
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
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator%(
        Tp const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator^(
        Tp const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator&(
        Tp const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the quaternion elements and
     * the scalar value
     *
     * @tparam Tp the quaternion's value type
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the quaternion
     * @return the result quaternion
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Quaternion<Tp> operator|(
        Tp const& left,
        Quaternion<Tp> const& right) requires mpgl_Operable(Tp, |);

}

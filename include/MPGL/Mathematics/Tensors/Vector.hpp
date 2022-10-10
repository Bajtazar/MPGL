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

#include <algorithm>
#include <iterator>
#include <numeric>
#include <complex>
#include <math.h>
#include <array>
#include <span>

#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Utility/Ranges.hpp>

namespace mpgl {

    /**
     * Represents the mathematical vector in the memory
     *
     * @tparam Tp the vector's value type
     * @tparam Size the number of vector's elements
     */
    template <Arithmetic Tp, std::size_t Size>
    class Vector {
        using carrier = std::array<Tp, Size>;
    public:
        using value_type = Tp;
        using reference = Tp&;
        using const_reference = Tp const&;

        /**
         * Constructs a new vector object from the given
         * elements
         *
         * @tparam Args the universal references to elements
         */
        template <AbsolutelyArithmetic... Args>
            requires (sizeof...(Args) == Size
                && AllConvertible<value_type,
                    std::remove_cvref_t<Args>...>)
        constexpr Vector(Args&&... args) noexcept
            : data{{ static_cast<Tp>(std::forward<Args>(args))... }} {}

        /**
         * Constructs a new vector object
         */
        constexpr Vector(void) noexcept = default;

        /**
         * Returns the size of the vector [the number of its
         * elements]
         *
         * @return the size of the vector
         */
        [[nodiscard]] static constexpr std::size_t size(
            void) noexcept
                { return Size; }

        /**
         * Returns the length of the vector [equivalent
         * to the square norm in the euclidean space]
         *
         * @tparam Up the result type
         * @param init the initial value
         * @return the length of the vector
         */
        template <Arithmetic Up = Tp>
        [[nodiscard]] constexpr Up length(void) const noexcept;

        /**
         * Normalizes vector
         */
        constexpr Vector& normalize(void) noexcept;

        /**
         * Adds the elements of this vector with
         * the given vector elements
         *
         * @param right the added vector
         * @return the reference to this vector
         */
        constexpr Vector& operator+=(Vector const& right);

        /**
         * Subtracts the elements of this vector with
         * the given vector elements
         *
         * @param right the subtracted vector
         * @return the reference to this vector
         */
        constexpr Vector& operator-=(Vector const& right);

        /**
         * Multiplies the elements of this vector with
         * the given vector elements
         *
         * @param right the multiplicand vector
         * @return the reference to this vector
         */
        constexpr Vector& operator*=(Vector const& right);

        /**
         * Divides the elements of this vector by the
         * given vector elements
         *
         * @param right the divider vector
         * @return the reference to this vector
         */
        constexpr Vector& operator/=(Vector const& right);

        /**
         * Calculates the modulo of elements of this vector
         * and the given vector elements
         *
         * @param right the vector with modulos
         * @return the reference to this vector
         */
        constexpr Vector& operator%=(Vector const& right)
            requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * vector and the given vector elements
         *
         * @param right the vector with values
         * @return the reference to this vector
         */
        constexpr Vector& operator^=(Vector const& right)
            requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this vector
         * and the given vector elements
         *
         * @param right the vector with values
         * @return the reference to this vector
         */
        constexpr Vector& operator&=(Vector const& right)
            requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this vector
         * and the given vector elements
         *
         * @param right the vector with values
         * @return the reference to this vector
         */
        constexpr Vector& operator|=(Vector const& right)
            requires mpgl_Operable(Tp, |);

        /**
         * Adds the given scalar value to the elements of
         * this vector
         *
         * @param right the added scalar
         * @return the reference to this vector
         */
        constexpr Vector& operator+=(Tp const& right);

        /**
         * Subtracts the given scalar value to the elements of
         * this vector
         *
         * @param right the subtracted scalar
         * @return the reference to this vector
         */
        constexpr Vector& operator-=(Tp const& right);

        /**
         * Multiplies the given scalar value to the elements of
         * this vector
         *
         * @param right the multiplied scalar
         * @return the reference to this vector
         */
        constexpr Vector& operator*=(Tp const& right);

        /**
         * Divides the elements of this vector by the given
         * scalar value
         *
         * @param right the divisor scalar
         * @return the reference to this vector
         */
        constexpr Vector& operator/=(Tp const& right);

        /**
         * Calculates the modulo of elements of this vector
         * with the given scalar
         *
         * @param right the modulo scalar
         * @return the reference to this vector
         */
        constexpr Vector& operator%=(Tp const& right)
            requires mpgl_Operable(Tp, %);

        /**
         * Calculates the bitwise-xor of elements of this
         * vector and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this vector
         */
        constexpr Vector& operator^=(Tp const& right)
            requires mpgl_Operable(Tp, ^);

        /**
         * Calculates the bitwise-and of elements of this
         * vector and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this vector
         */
        constexpr Vector& operator&=(Tp const& right)
            requires mpgl_Operable(Tp, &);

        /**
         * Calculates the bitwise-or of elements of this
         * vector and the given scalar value
         *
         * @param right the scalar value
         * @return the reference to this vector
         */
        constexpr Vector& operator|=(Tp const& right)
            requires mpgl_Operable(Tp, |);

        /**
         * Returns a std::span that holds view to this vector
         *
         * @return the std::span that holds view to this vector
         */
        [[nodiscard]] constexpr operator std::span<Tp, Size>(
            ) const noexcept;

        /**
         * Casts the vector to the given type
         *
         * @tparam Up the new vector's element type
         * @return the casted vector
         */
        template <Arithmetic Up>
            requires std::convertible_to<Tp, Up>
        [[nodiscard]] constexpr operator Vector<Up, Size>(
            ) const noexcept;

        /**
         * Casts the vector to the given size
         *
         * @tparam USize the new vector's size
         * @return the casted vector
         */
        template <std::size_t USize>
            requires (USize > Size)
        [[nodiscard]] constexpr operator Vector<Tp, USize>(
            ) const noexcept;

        /**
         * Inverses the sign of the vector's elements
         *
         * @return the vector with sign-inversed elements
         */
        [[nodiscard]] constexpr Vector operator-(void) const noexcept;

        using iterator = typename carrier::iterator;
        using const_iterator = typename carrier::const_iterator;
        using reverse_iterator
            = typename carrier::reverse_iterator;
        using const_reverse_iterator
            = typename carrier::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the vector
         *
         * @return the iterator to the begining of the vector
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return data.begin(); }

        /**
         * Returns the iterator to the end of the vector
         *
         * @return the iterator to the end of the vector
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vector
         *
         * @return the constant iterator to the begining
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return data.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vector
         *
         * @return the constant iterator to the end
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vector
         *
         * @return the constant iterator to the begining
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return data.cbegin(); }

        /**
         * Returns the constant iterator to the end
         * of the vector
         *
         * @return the constant iterator to the end
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return data.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the vector
         *
         * @return the reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(
            void) noexcept
                { return data.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vector
         *
         * @return the reverse iterator to the begining of
         * the vector
         */
        [[nodiscard]] constexpr reverse_iterator rend(
            void) noexcept
                { return data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vector
         *
         * @return the constant reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept
                { return data.rbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the vector
         *
         * @return the constant reverse iterator to the
         * begining of the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept
                { return data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vector
         *
         * @return the constant reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept
                { return data.crbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the vector
         *
         * @return the constant reverse iterator to the
         * begining of the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator crend(
            void) const noexcept
                { return data.crend(); }

        /**
         * Returns the reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference operator[] (
            std::size_t index) noexcept
                { return data[index]; }

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
                { return data[index]; }

        /**
         * Returns the reference to element with the given index
         * @throws std::out_of_range when the index is out of the
         * vector's scope
         *
         * @param index the element's index
         * @return the reference to element with the given index
         */
        [[nodiscard]] constexpr reference at(
            std::size_t index)
                { return data.at(index); }

        /**
         * Returns the constant reference to element with
         * the given index
         * @throws std::out_of_range when the index is out of the
         * vector's scope
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        [[nodiscard]] constexpr const_reference at(
            std::size_t index) const
                { return data.at(index); }
    private:
        std::array<Tp, Size>                    data;
    };

    /**
     * Returns the dot product of two vectors
     *
     * @tparam Tp the vector's value type
     * @tparam Size the vector' size
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the dot product
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Tp dot(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    /**
     * Returns the cross product of two two-dimensional vectors
     *
     * @tparam Tp the vector's value type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the cross product
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Tp cross(
        Vector<Tp, 2> const& left,
        Vector<Tp, 2> const& right) noexcept;

    /**
     * Returns the cross product of two three-dimensional vectors
     *
     * @tparam Tp the vector's value type
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the cross product
     */
    template <Arithmetic Tp>
    [[nodiscard]] constexpr Vector<Tp, 3> cross(
        Vector<Tp, 3> const& left,
        Vector<Tp, 3> const& right) noexcept;

    /**
     * Casts the vector elements to the given type
     *
     * @tparam Up the new vector's value type
     * @tparam Tp the old vector's value type
     * @tparam Size the vector' size
     * @param vector the constant reference to the vector
     * @return the casted vector
     */
    template <Arithmetic Up, Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Up, Size>
        vectorCast(Vector<Tp, Size> const& vector) noexcept;

    /**
     * Returns the vector with the floored values
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's type
     * @param vector the constant reference to the vector
     * @return the floored vector
     */
    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> floor(
        Vector<Tp, Size> vector);

    /**
     * Returns the vector with the ceiled values
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's type
     * @param vector the constant reference to the vector
     * @return the ceiled vector
     */
    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> ceil(
        Vector<Tp, Size> vector);

    /**
     * Returns the vector with the rounded values
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's type
     * @param vector the constant reference to the vector
     * @return the rounded vector
     */
    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> round(
        Vector<Tp, Size> vector);

    /**
     * Normalizes vector
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's type
     * @param vector the constant reference to the vector
     * @return the normalized vector
     */
    template <std::floating_point Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> normalize(
        Vector<Tp, Size> const& vector);

    /**
     * Adds two vectors together
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right);

    /**
     * Subtracts vector from other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right);

    /**
     * Multiplies two vectors together
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right);

    /**
     * Divides the elements of one vector by the elements
     * of another
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right);

    /**
     * Calculates the modulo of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-and of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the vector and the other vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Vector<Tp, Size> const& left,
        Tp const& right);

    /**
     * Subtracts the scalar value from the vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Vector<Tp, Size> const& left,
        Tp const& right);

    /**
     * Multiplies the vector with the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Vector<Tp, Size> const& left,
        Tp const& right);

    /**
     * Divides the vector consisting of scalar value fields
     * by the vector elements
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Vector<Tp, Size> const& left,
        Tp const& right);

    /**
     * Calculates the modulo of the vector consisting of scalar
     * values with the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, %);

    /**
     * Calculates the bitwise-xor of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the vector
     * @param right the constant reference to the scalar value
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Vector<Tp, Size> const& left,
        Tp const& right) requires mpgl_Operable(Tp, |);

    /**
     * Adds the scalar value to the vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator+(
        Tp const& left,
        Vector<Tp, Size> const& right);

    /**
     * Subtracts the scalar value from the vector
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator-(
        Tp const& left,
        Vector<Tp, Size> const& right);

    /**
     * Multiplies the vector with the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator*(
        Tp const& left,
        Vector<Tp, Size> const& right);

    /**
     * Divides the vector consisting of scalar value fields
     * by the vector elements
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator/(
        Tp const& left,
        Vector<Tp, Size> const& right);

    /**
     * Calculates the modulo of the vector consisting of scalar
     * values with the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator%(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator^(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, ^);

    /**
     * Calculates the bitwise-xor of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator&(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, &);

    /**
     * Calculates the bitwise-or of the vector elements and
     * the scalar value
     *
     * @tparam Tp the vector's value type
     * @tparam Rows the vector's rows
     * @param left the constant reference to the scalar value
     * @param right the constant reference to the vector
     * @return the result vector
     */
    template <Arithmetic Tp, std::size_t Size>
    [[nodiscard]] constexpr Vector<Tp, Size> operator|(
        Tp const& left,
        Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, |);

    /**
     * Compares two vectors in the lexicographical order
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's size
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the ordering result
     */
    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::weak_ordering>
    [[nodiscard]] constexpr std::weak_ordering lexicographicalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    /**
     * Compares two vectors in the total order
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's size
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the ordering result
     */
    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] constexpr std::partial_ordering totalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    /**
     * Checks whether two vectors are equivalent in the total
     * order
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's size
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return if the vectors are equals
     */
    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] bool constexpr operator== (
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    /**
     * Compares two vectors in the total order
     *
     * @tparam Tp the vector's type
     * @tparam Size the vector's size
     * @param left the constant reference to the left vector
     * @param right the constant reference to the right vector
     * @return the ordering result
     */
    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    [[nodiscard]] std::partial_ordering constexpr operator<=> (
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    template class Vector<float32, 2>;
    template class Vector<float32, 3>;
    template class Vector<float32, 4>;
    template class Vector<uint32, 2>;

    template <std::size_t Size>
    using VectorC                     =             Vector<
        std::complex<float64>, Size>;

    typedef Vector<int16, 2>                        Vector2si;
    typedef Vector<float32, 2>                      Vector2f;
    typedef Vector<uint32, 2>                       Vector2u;
    typedef Vector<int32, 2>                        Vector2i;
    typedef Vector<float64, 2>                      Vector2d;
    typedef VectorC<2>                              Vector2c;

    typedef Vector<float32, 3>                      Vector3f;
    typedef Vector<uint32, 3>                       Vector3u;
    typedef Vector<int32, 3>                        Vector3i;
    typedef Vector<float64, 3>                      Vector3d;
    typedef VectorC<3>                              Vector3c;

    typedef Vector<float32, 4>                      Vector4f;
    typedef Vector<uint32, 4>                       Vector4u;
    typedef Vector<int32, 4>                        Vector4i;
    typedef Vector<float64, 4>                      Vector4d;
    typedef VectorC<4>                              Vector4c;

    template <Arithmetic Tp>
    using Vector2 =                                 Vector<Tp, 2>;

    template <Arithmetic Tp>
    using Vector3 =                                 Vector<Tp, 3>;

    template <Arithmetic Tp>
    using Vector4 =                                 Vector<Tp, 4>;

    /**
     * Convinient literal for creating 2D floating point vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the x-axis
     */
    [[nodiscard]] constexpr Vector2f operator"" _x(
        float128 value) noexcept;

    /**
     * Convinient literal for creating 2D unsigned integer vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the x-axis
     */
    [[nodiscard]] constexpr Vector2u operator"" _x(
        uint64 value) noexcept;

    /**
     * Convinient literal for creating 2D floating point vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the y-axis
     */
    [[nodiscard]] constexpr Vector2f operator"" _y(
        float128 value) noexcept;

    /**
     * Convinient literal for creating 2D unsigned integer vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the y-axis
     */
    [[nodiscard]] constexpr Vector2u operator"" _y(
        uint64 value) noexcept;

    /**
     * Convinient literal for creating 3D floating point vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the z-axis
     */
    [[nodiscard]] constexpr Vector3f operator"" _z(
        float128 value) noexcept;

    /**
     * Convinient literal for creating 3D unsigned integer vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the z-axis
     */
    [[nodiscard]] constexpr Vector3u operator"" _z(
        uint64 value) noexcept;

    /**
     * Convinient literal for creating 4D floating point vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the w-axis [4th]
     */
    [[nodiscard]] constexpr Vector4f operator"" _w(
        float128 value) noexcept;

    /**
     * Convinient literal for creating 4D unsigned integer vectors
     *
     * @param value the operator's value
     * @return the vector with the given value in the w-axis [4th]
     */
    [[nodiscard]] constexpr Vector4u operator"" _w(
        uint64 value) noexcept;

}

#include <MPGL/Mathematics/Tensors/Vector.tpp>

namespace std {

    template <mpgl::Arithmetic Tp, std::size_t Size>
    struct tuple_size<mpgl::Vector<Tp, Size>>
        : integral_constant<size_t, Size> {};

    template <mpgl::Arithmetic Tp, std::size_t Size,
        std::size_t Index>
            requires (Index < Size)
    struct tuple_element<Index, mpgl::Vector<Tp, Size>> {
        using type = Tp;
    };

}

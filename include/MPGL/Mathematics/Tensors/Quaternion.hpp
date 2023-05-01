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

    template <Arithmetic Tp>
    struct Quaternion {
        using value_type = typename Vector4<Tp>::value_type;
        using reference = typename Vector4<Tp>::reference;
        using const_reference = typename Vector4<Tp>::const_reference;

        [[nodiscard]] static consteval std::size_t size(
            void) noexcept
                { return 4; }

        template <Arithmetic Up = Tp>
        [[nodiscard]] constexpr Up length(void) const noexcept;

        constexpr Quaternion& normalize(void) noexcept;

        constexpr Quaternion& conjugate(void) noexcept;

        constexpr Quaternion& operator+=(Quaternion const& right);

        constexpr Quaternion& operator-=(Quaternion const& right);

        constexpr Quaternion& operator*=(Quaternion const& right);

        constexpr Quaternion& operator/=(Quaternion const& right);

        constexpr Quaternion& operator%=(Quaternion const& right)
            requires mpgl_Operable(Tp, %);

        constexpr Quaternion& operator^=(Quaternion const& right)
            requires mpgl_Operable(Tp, ^);

        constexpr Quaternion& operator&=(Quaternion const& right)
            requires mpgl_Operable(Tp, &);

        constexpr Quaternion& operator|=(Quaternion const& right)
            requires mpgl_Operable(Tp, |);

        constexpr Quaternion& operator+=(Tp const& right);

        constexpr Quaternion& operator-=(Tp const& right);

        constexpr Quaternion& operator*=(Tp const& right);

        constexpr Quaternion& operator/=(Tp const& right);

        constexpr Quaternion& operator%=(Tp const& right)
            requires mpgl_Operable(Tp, %);

        constexpr Quaternion& operator^=(Tp const& right)
            requires mpgl_Operable(Tp, ^);

        constexpr Quaternion& operator&=(Tp const& right)
            requires mpgl_Operable(Tp, &);

        constexpr Quaternion& operator|=(Tp const& right)
            requires mpgl_Operable(Tp, |);

        [[nodiscard]] constexpr operator std::span<Tp const, Size>(
            ) const noexcept;

        template <Arithmetic Up>
            requires std::convertible_to<Tp, Up>
        [[nodiscard]] constexpr operator Quaternion<Up>(
            ) const noexcept;

        [[nodiscard]] constexpr Vector4<Tp> asVector(void) const noexcept;

        [[nodiscard]] constexpr Tp real(void) const noexcept;

        [[nodiscard]] constexpr Vector3<Tp> imaginary(void) const noexcept;

        [[nodiscard]] constexpr Quaternion operator-(void) const noexcept;

        using iterator = typename Vector4<Tp>::iterator;
        using const_iterator = typename Vector4<Tp>::const_iterator;
        using reverse_iterator
            = typename Vector4<Tp>::reverse_iterator;
        using const_reverse_iterator
            = typename Vector4<Tp>::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the vector
         *
         * @return the iterator to the begining of the vector
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return _M_data.begin(); }

        /**
         * Returns the iterator to the end of the vector
         *
         * @return the iterator to the end of the vector
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return _M_data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vector
         *
         * @return the constant iterator to the begining
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return _M_data.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vector
         *
         * @return the constant iterator to the end
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return _M_data.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vector
         *
         * @return the constant iterator to the begining
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return _M_data.cbegin(); }

        /**
         * Returns the constant iterator to the end
         * of the vector
         *
         * @return the constant iterator to the end
         * of the vector
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return _M_data.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the vector
         *
         * @return the reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(
            void) noexcept
                { return _M_data.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vector
         *
         * @return the reverse iterator to the begining of
         * the vector
         */
        [[nodiscard]] constexpr reverse_iterator rend(
            void) noexcept
                { return _M_data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vector
         *
         * @return the constant reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin(
            void) const noexcept
                { return _M_data.rbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the vector
         *
         * @return the constant reverse iterator to the
         * begining of the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator rend(
            void) const noexcept
                { return _M_data.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vector
         *
         * @return the constant reverse iterator to the end of
         * the vector
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin(
            void) const noexcept
                { return _M_data.crbegin(); }

        /**
         * Returns the constant reverse iterator to the
         * begining of the vector
         *
         * @return the constant reverse iterator to the
         * begining of the vector
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
         * vector's scope
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
         * vector's scope
         *
         * @param index the element's index
         * @return the constant reference to element with
         * the given index
         */
        [[nodiscard]] constexpr const_reference at(
            std::size_t index) const
                { return _M_data.at(index); }

        /**
         * Returns a reference to the vector's element with
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
         * Returns a constant reference to the vector's element
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
         * Returns a rvalue reference to the vector's element
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

}

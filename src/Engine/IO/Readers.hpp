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

#include "../Iterators/BitIterator.hpp"
#include "../Traits/Concepts.hpp"

#include <algorithm>
#include <iterator>

namespace mpgl {

    /**
     * Reads the given non-byte-size type from the given iterator.
     * Advances iterator by the size of the given type
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the reference to the iterator
     * @return the readed type
     */
    template <NotSameSize<std::byte> Tp, bool BigEndian = false,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] Tp
        readType(Iter& iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Peeks the given non-byte-size type from the given iterator
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the iterator object
     * @return the peaked type
     */
    template <NotSameSize<std::byte> Tp, bool BigEndian = false,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] Tp
        peekType(Iter iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Reads the given byte-size type from the given iterator.
     * Advances iterator by the size of the given type
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the reference to the iterator
     * @return the readed type
     */
    template <SameSize<std::byte> Tp, bool BigEndian = false,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] inline Tp
        readType(Iter& iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Peeks the given byte-size type from the given iterator
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the iterator object
     * @return the peaked type
     */
    template <SameSize<std::byte> Tp, bool BigEndian = false,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] inline Tp
        peekType(Iter iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Reads the fixed point type from the given iterator.
     * Advances iterator by the size of the given type
     *
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Up the 'base' type
     * @tparam Tp the floating point representing the fixed type
     * @tparam Shift the number of bits in the mantissa
     * @tparam Iter the iterator type
     * @param iterator the reference to the iterator
     * @return the readed fixed type
     */
    template <bool BigEndian = false, std::integral Up = int32,
        std::floating_point Tp = float32, std::size_t Shift = 16,
        std::input_iterator Iter>
    [[nodiscard]] inline Tp readFixed(
        Iter& iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Peeks the fixed point type from the given iterator
     *
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Up the 'base' type
     * @tparam Tp the floating point representing the fixed type
     * @tparam Shift the number of bits in the mantissa
     * @tparam Iter the iterator type
     * @param iterator the iterator object
     * @return the peaked fixed type
     */
    template <bool BigEndian = false, std::integral Up = int32,
        std::floating_point Tp = float32, std::size_t Shift = 16,
        std::input_iterator Iter>
    [[nodiscard]] inline Tp peekFixed(
        Iter iterator) noexcept(NothrowReadable<Iter>);

    /**
     * Reads n-chars from the iterator. Advances iterator by n
     *
     * @tparam Iter the iterator type
     * @param length the length of the char sequence
     * @param iter the reference to the iterator
     * @return the string with readed char sequence
     */
    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] std::string readNChars(
        std::size_t length,
        Iter& iter) noexcept(NothrowReadable<Iter>);

    /**
     * Peeks n-chars from the iterator
     *
     * @tparam Iter the iterator type
     * @param length the length of the char sequence
     * @param iter the reference to the iterator
     * @return the string with peaked char sequence
     */
    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] std::string peekNChars(
        std::size_t length,
        Iter iter) noexcept(NothrowReadable<Iter>);

    /**
     * Reads the given type from the given bit iterator.
     * Advances iterator by the size of the given type
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the reference to the iterator
     * @return the readed type
     */
    template <std::integral Tp, bool BigEndian, BitIterator Iter>
    [[nodiscard]] constexpr Tp readType(Iter& iter) noexcept;

    /**
     * Peeks the given type from the given bit iterator
     *
     * @tparam Tp the given type
     * @tparam BigEndian indicates if the type is saved
     * in the big-endian manner
     * @tparam Iter the iterator type
     * @param iterator the iterator object
     * @return the readed type
     */
    template <std::integral Tp, bool BigEndian, BitIterator Iter>
    [[nodiscard]] constexpr Tp peekType(Iter iter) noexcept;

    /**
     * Reads n-chars from the given bit iterator to the given
     * type. Advances iterator by n
     *
     * @tparam Tp the target type
     * @tparam Iter the iterator type
     * @param length the length of the sequence
     * @param iter the reference to the iterator object
     * @return the readed data
     */
    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp readNBits(
        std::size_t length,
        Iter& iter) noexcept;

    /**
     * Peeks n-chars from the given bit iterator to the given
     * type
     *
     * @tparam Tp the target type
     * @tparam Iter the iterator type
     * @param length the length of the sequence
     * @param iter the iterator object
     * @return the readed data
     */
    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp peekNBits(
        std::size_t length,
        Iter iter) noexcept;

    /**
     * Reads n-chars from the given bit iterator to the given
     * type in reverse order. Advances iterator by n
     *
     * @tparam Tp the target type
     * @tparam Iter the iterator type
     * @param length the length of the sequence
     * @param iter the reference to the iterator object
     * @return the readed data
     */
    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp readRNBits(
        std::size_t length,
        Iter& iter) noexcept;

    /**
     * Peeks n-chars from the given bit iterator to the given
     * type in reverse order
     *
     * @tparam Tp the target type
     * @tparam Iter the iterator type
     * @param length the length of the sequence
     * @param iter the iterator object
     * @return the readed data
     */
    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp peekRNBits(
        std::size_t length,
        Iter iter) noexcept;

    template <NotSameSize<std::byte> Tp, bool BigEndian,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] Tp readType(
        Iter& iterator) noexcept(NothrowReadable<Iter>)
    {
        Tp data;
        if constexpr (BigEndian) {
            char* raw = reinterpret_cast<char*>(&data) + sizeof(Tp) - 1;
            for (uint8 i = sizeof(Tp);i != 0; --i, --raw, ++iterator)
                *raw = *iterator;
        } else {
            char* raw = reinterpret_cast<char*>(&data);
            for (uint8 i = 0;i != sizeof(Tp); ++i, ++raw, ++iterator)
                *raw = *iterator;
        }
        return data;
    }

    template <NotSameSize<std::byte> Tp, bool BigEndian,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] Tp peekType(
        Iter iterator) noexcept(NothrowReadable<Iter>)
    {
        Tp data;
        if constexpr (BigEndian) {
            char* raw = reinterpret_cast<char*>(&data) + sizeof(Tp) - 1;
            for (uint8 i = sizeof(Tp);i != 0; --i, --raw, ++iterator)
                *raw = *iterator;
        } else {
            char* raw = reinterpret_cast<char*>(&data);
            for (uint8 i = 0;i != sizeof(Tp); ++i, ++raw, ++iterator)
                *raw = *iterator;
        }
        return data;
    }

    template <SameSize<std::byte> Tp, bool BigEndian,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] inline Tp readType(
        Iter& iterator) noexcept(NothrowReadable<Iter>)
    {
        return static_cast<Tp>(*iterator++);
    }

    template <SameSize<std::byte> Tp, bool BigEndian,
        std::input_iterator Iter>
            requires (std::is_trivially_constructible_v<Tp>
                && std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] inline Tp peekType(
        Iter iterator) noexcept(NothrowReadable<Iter>)
    {
        return static_cast<Tp>(*iterator++);
    }

    template <bool BigEndian, std::integral Up,
        std::floating_point Tp, std::size_t Shift,
        std::input_iterator Iter>
    [[nodiscard]] inline Tp readFixed(
        Iter& iterator) noexcept(NothrowReadable<Iter>)
    {
        return static_cast<Tp>(readType<Up, BigEndian>(iterator))
            / (1 << Shift);
    }

    template <bool BigEndian, std::integral Up,
        std::floating_point Tp, std::size_t Shift,
        std::input_iterator Iter>
    [[nodiscard]] inline Tp peekFixed(
        Iter iterator) noexcept(NothrowReadable<Iter>)
    {
        return static_cast<Tp>(peekType<Up, BigEndian>(iterator))
            / (1 << Shift);
    }

    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] std::string readNChars(
        std::size_t length,
        Iter& iter) noexcept(NothrowReadable<Iter>)
    {
        std::string data(length, ' ');
        std::ranges::for_each(data, [&iter](auto& c){ c = *iter++; });
        return data;
    }

    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    [[nodiscard]] std::string peekNChars(
        std::size_t length,
        Iter iter) noexcept(NothrowReadable<Iter>)
    {
        std::string data(length, ' ');
        std::ranges::for_each(data, [&iter](auto& c){ c = *iter++; });
        return data;
    }

    template <std::integral Tp, bool BigEndian, BitIterator Iter>
    [[nodiscard]] constexpr Tp readType(Iter& iter) noexcept {
        Tp data;
        if constexpr (BigEndian) {
            std::byte* raw = reinterpret_cast<std::byte*>(&data) +
                sizeof(Tp) - 1;
            for (uint8 i = sizeof(Tp); i != 0; --i, --raw)
                *raw = iter.readByte();
        } else {
            std::byte* raw = reinterpret_cast<std::byte*>(&data);
            for (uint8 i = 0; i != sizeof(Tp); ++i, ++raw)
                *raw = iter.readByte();
        }
        return data;
    }

    template <std::integral Tp, bool BigEndian, BitIterator Iter>
    [[nodiscard]] constexpr Tp peekType(Iter iter) noexcept {
        Tp data;
        if constexpr (BigEndian) {
            std::byte* raw = reinterpret_cast<std::byte*>(&data) +
                sizeof(Tp) - 1;
            for (uint8 i = sizeof(Tp); i != 0; --i, --raw)
                *raw = iter.readByte();
        } else {
            std::byte* raw = reinterpret_cast<std::byte*>(&data);
            for (uint8 i = 0; i != sizeof(Tp); ++i, ++raw)
                *raw = iter.readByte();
        }
        return data;
    }

    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp
        readNBits(std::size_t length, Iter& iter) noexcept
    {
        Tp answer = 0;
        for (std::size_t i = 0; i < length; ++i)
            answer += (*iter++) << i;
        return answer;
    }

    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp
        peekNBits(std::size_t length, Iter iter) noexcept
    {
        Tp answer = 0;
        for (std::size_t i = 0; i < length; ++i)
            answer += (*iter++) << i;
        return answer;
    }

    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp
        readRNBits(std::size_t length, Iter& iter) noexcept
    {
        Tp answer = 0;
        for (std::size_t i = 1; i <= length; ++i)
            answer += (*iter++) << (length - i);
        return answer;
    }

    template <std::integral Tp, BitIterator Iter>
    [[nodiscard]] constexpr Tp
        peekRNBits(std::size_t length, Iter iter) noexcept
    {
        Tp answer = 0;
        for (std::size_t i = 1; i <= length; ++i)
            answer += (*iter++) << (length - i);
        return answer;
    }

}

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

    template <ByteInputIterator Iter>
    constexpr LittleEndianInputBitIter<Iter>&
        LittleEndianInputBitIter<Iter>::operator++ (void) noexcept
    {
        if (++bitIter == byteLength()) {
            ++iter;
            bitIter = 0;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        LittleEndianInputBitIter<Iter>::readByte(void) noexcept
    {
        bitIter = 0;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr bool
        LittleEndianInputBitIter<Iter>::equal(
            LittleEndianInputBitIter const& other) const noexcept
    {
        return iter == other.iter && bitIter == other.bitIter;
    }

    template <ByteOutputIterator Iter>
    constexpr LittleEndianOutputBitIter<Iter>&
        LittleEndianOutputBitIter<Iter>::operator=(bit value) noexcept
    {
        uint8 mask = 1 << bitIter;
        temporary = (~mask & temporary) | (value << bitIter);
        if (++bitIter == byteLength()) {
            *iter++ = temporary;
            bitIter = temporary = 0;
        }
        return *this;
    }

    template <ByteOutputIterator Iter>
    constexpr void LittleEndianOutputBitIter<Iter>::skipToNextByte(
        void) noexcept
    {
        *iter++ = temporary;
        bitIter = temporary = 0;
    }

    template <ByteOutputIterator Iter>
    constexpr void LittleEndianOutputBitIter<Iter>::saveByte(
        std::byte byte) noexcept
    {
        temporary = bitIter = 0;
        *iter++ = byte;
    }

    template <ByteOutputIterator Iter>
    [[nodiscard]] constexpr bool
        LittleEndianOutputBitIter<Iter>::equal(
            LittleEndianOutputBitIter const& other) const noexcept
    {
        return iter == other.iter && bitIter == other.bitIter;
    }

    template <ByteInputIterator Iter>
    constexpr BigEndianInputBitIter<Iter>&
        BigEndianInputBitIter<Iter>::operator++ (void) noexcept
    {
        if (!(bitIter--)) {
            ++iter;
            bitIter = 7;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        BigEndianInputBitIter<Iter>::readByte(void) noexcept
    {
        bitIter = 7;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr bool
        BigEndianInputBitIter<Iter>::equal(
            BigEndianInputBitIter const& other) const noexcept
    {
        return iter == other.iter && bitIter == other.bitIter;
    }

    template <ByteOutputIterator Iter>
    constexpr BigEndianOutputBitIter<Iter>&
        BigEndianOutputBitIter<Iter>::operator=(bit value) noexcept
    {
        uint8 mask = 1 << bitIter;
        temporary = (~mask & temporary) | (value << bitIter);
        if (!(bitIter--)) {
            *iter++ = temporary;
            bitIter = 7;
            temporary = 0;
        }
        return *this;
    }

    template <ByteOutputIterator Iter>
    constexpr void BigEndianOutputBitIter<Iter>::skipToNextByte(
        void) noexcept
    {
        *iter++ = temporary;
        bitIter = 7;
        temporary = 0;
    }

    template <ByteOutputIterator Iter>
    constexpr void BigEndianOutputBitIter<Iter>::saveByte(
        std::byte byte) noexcept
    {
        temporary = 0;
        bitIter = 7;
        *iter++ = byte;
    }

    template <ByteOutputIterator Iter>
    [[nodiscard]] constexpr bool
        BigEndianOutputBitIter<Iter>::equal(
            BigEndianOutputBitIter const& other) const noexcept
    {
        return iter == other.iter && bitIter == other.bitIter;
    }

}

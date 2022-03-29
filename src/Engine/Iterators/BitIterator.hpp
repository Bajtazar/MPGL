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

#include <iterator>
#include <climits>

#include "../Traits/Concepts.hpp"

namespace mpgl {

    /**
     * Defines the category of the bit iterators
     */
    struct BitIteratorTag : public std::input_iterator_tag {};

    template <class Iter>
    concept BitIterator = std::input_iterator<Iter> &&
        requires {typename Iter::iterator_category;} &&
        std::derived_from< typename Iter::iterator_category,
            BitIteratorTag>;

    /**
     * Checks whether the given range uses a bit iterator
     *
     * @tparam Range the checked range
     */
    template <class Range>
    concept BitRange = std::ranges::input_range<Range> &&
        BitIterator<std::ranges::iterator_t<Range>>;

    /**
     * Iterator returning the values of the individual bits in the
     * little endian manner. Allows to override the bit value
     * with the special method
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteInputIterator Iter>
    class LittleEndianBitIter : public std::iterator<
        BitIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitIteratorTag;

        /**
         * Constructs a new little endian bit iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr LittleEndianBitIter(
            Iter const& iter) noexcept
                : iter{iter}, bitIter{0} {}

        /**
         * Constructs a new little endian bit iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr LittleEndianBitIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, bitIter{0} {}

        /**
         * Constructs a new little endian bit iterator
         */
        explicit constexpr LittleEndianBitIter(
            void) noexcept = default;

        /**
         * Returns a value of the currently examined bit
         *
         * @return the value of the currently examined bit
         */
        [[nodiscard]] constexpr bit operator* (void) const noexcept
            { return ((1 << bitIter) & (*iter)) >> bitIter; }

        /**
         * Sets a value of the currently examined bit
         *
         * @param value the new value of the bit
         */
        constexpr void setBit(bit value) noexcept;

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        constexpr LittleEndianBitIter& operator++(
            void) noexcept;

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr LittleEndianBitIter operator++(
            int) noexcept
                { auto temp = *this; ++(*this); return temp; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool operator== (
            LittleEndianBitIter const& left,
            LittleEndianBitIter const& right) noexcept
                { return left.iter == right.iter; }

        /**
         * Returns a number of bits in byte
         *
         * @return the number of bits in byte
         */
        [[nodiscard]] static inline consteval uint8 byteLength(
            void) noexcept
                { return CHAR_BIT; }

        /**
         * Jums to the begining of the next byte
         */
        constexpr void skipToNextByte(void) noexcept
            { ++iter; bitIter = 0; }

        /**
         * Reads the entire byte
         *
         * @return the readed byte
         */
        [[nodiscard]] constexpr std::byte readByte(void) noexcept;
    private:
        Iter                            iter;
        uint8                           bitIter;
    };

    /**
     * Iterator returning the values of the individual bits in the
     * big endian manner. Allows to override the bit value
     * with the special method
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteInputIterator Iter>
    class BigEndianBitIter : public std::iterator<
        BitIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitIteratorTag;

        /**
         * Constructs a new big endian bit iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr BigEndianBitIter(
            Iter const& iter) noexcept
                : iter{iter}, bitIter{7} {}

        /**
         * Constructs a new big endian bit iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr BigEndianBitIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, bitIter{7} {}

        /**
         * Constructs a new big endian bit iterator
         */
        explicit constexpr BigEndianBitIter(
                void) noexcept = default;

        /**
         * Returns a value of the currently examined bit
         *
         * @return the value of the currently examined bit
         */
        [[nodiscard]] constexpr bit operator* (void) const noexcept
            { return ((1 << bitIter) & (*iter)) >> bitIter; }

        /**
         * Sets a value of the currently examined bit
         *
         * @param value the new value of the bit
         */
        constexpr void setBit(bit value) noexcept;

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        constexpr BigEndianBitIter& operator++ (void) noexcept;

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr BigEndianBitIter  operator++(
            int) noexcept
                { auto temp = *this; ++(*this); return temp; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool operator== (
            BigEndianBitIter const& left,
            BigEndianBitIter const& right) noexcept
                { return left.iter == right.iter; }

        /**
         * Returns a number of bits in byte
         *
         * @return the number of bits in byte
         */
        [[nodiscard]] static inline consteval uint8 byteLength(
            void) noexcept
                { return CHAR_BIT; }

        /**
         * Jums to the begining of the next byte
         */
        constexpr void skipToNextByte(void) noexcept
            { ++iter; bitIter = 7; }

        /**
         * Reads the entire byte
         *
         * @return the readed byte
         */
        [[nodiscard]] constexpr std::byte readByte(void) noexcept;
    private:
        Iter                            iter;
        uint8                           bitIter;
    };

    template <ByteInputIterator Iter>
    constexpr void LittleEndianBitIter<Iter>::setBit(
        bit value) noexcept
    {
        uint8 mask = 1 << bitIter;
        *iter = (~mask & *iter) | (value << bitIter);
    }

    template <ByteInputIterator Iter>
    constexpr LittleEndianBitIter<Iter>&
        LittleEndianBitIter<Iter>::operator++ (void) noexcept
    {
        ++bitIter;
        if (bitIter == byteLength()) {
            ++iter;
            bitIter = 0;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        LittleEndianBitIter<Iter>::readByte(void) noexcept
    {
        bitIter = 0;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteInputIterator Iter>
    constexpr void BigEndianBitIter<Iter>::setBit(
        bit value) noexcept
    {
        uint8 mask = 1 << bitIter;
        *iter = (~mask & *iter) | (value << bitIter);
    }

    template <ByteInputIterator Iter>
    constexpr BigEndianBitIter<Iter>&
        BigEndianBitIter<Iter>::operator++ (void) noexcept
    {
        if (!(bitIter--)) {
            ++iter;
            bitIter = 7;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        BigEndianBitIter<Iter>::readByte(void) noexcept
    {
        bitIter = 7;
        return static_cast<std::byte>(*iter++);
    }

}

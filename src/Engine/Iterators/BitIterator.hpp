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
     * Defines the category of the input bit iterators
     */
    struct BitInputIteratorTag : public std::input_iterator_tag {};

    /**
     * Defines the category of the input bit iterators
     */
    struct BitOutputIteratorTag : public std::output_iterator_tag {};

    /**
     * Checks whether the given iterator is the input bit iterator
     *
     * @tparam Iter the iterator type
     */
    template <class Iter>
    concept BitInputIterator = std::input_iterator<Iter> &&
        requires {typename Iter::iterator_category;} &&
        std::derived_from<typename Iter::iterator_category,
            BitInputIteratorTag>
        && requires (Iter iter) {
            { iter.readByte() } -> std::same_as<std::byte>;
        };

    /**
     * Checks whether the given iterator is the output bit iterator
     *
     * @tparam Iter the iterator type
     */
    template <class Iter>
    concept BitOutputIterator = std::output_iterator<Iter, bool> &&
        requires {typename Iter::iterator_category;} &&
        std::derived_from<typename Iter::iterator_category,
            BitOutputIteratorTag>
        && requires (Iter iter, std::byte byte) {
            { iter.saveByte(byte) } -> std::same_as<void>;
        };

    /**
     * Checks whether the given range uses a bit input iterator
     *
     * @tparam Range the checked range
     */
    template <class Range>
    concept BitInputRange = std::ranges::input_range<Range> &&
        BitInputIterator<std::ranges::iterator_t<Range>>;

    /**
     * Checks whether the given range uses a bit output iterator
     *
     * @tparam Range the checked range
     */
    template <class Range>
    concept BitOutputRange = std::ranges::output_range<Range, bool> &&
        BitOutputIterator<std::ranges::iterator_t<Range>>;

    /**
     * Iterator returning the values of the individual bits in the
     * little endian manner
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteInputIterator Iter>
    class LittleEndianBitInputIter : public std::iterator<
        BitInputIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitInputIteratorTag;

        /**
         * Constructs a new little endian bit input iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr LittleEndianBitInputIter(
            Iter const& iter) noexcept
                : iter{iter}, bitIter{0} {}

        /**
         * Constructs a new little endian bit input iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr LittleEndianBitInputIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, bitIter{0} {}

        /**
         * Constructs a new little endian bit input iterator
         */
        explicit constexpr LittleEndianBitInputIter(
            void) noexcept = default;

        /**
         * Returns a value of the currently examined bit
         *
         * @return the value of the currently examined bit
         */
        [[nodiscard]] constexpr bit operator* (void) const noexcept
            { return ((1 << bitIter) & (*iter)) >> bitIter; }

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        constexpr LittleEndianBitInputIter& operator++(
            void) noexcept;

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr LittleEndianBitInputIter operator++(
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
            LittleEndianBitInputIter const& left,
            LittleEndianBitInputIter const& right) noexcept
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
     * Iterator saving the values of the individual bits in the
     * little endian manner
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteOutputIterator Iter>
    class LittleEndianBitOutputIter : public std::iterator<
        BitOutputIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitOutputIteratorTag;

        /**
         * Constructs a new little endian bit output iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr LittleEndianBitOutputIter(
            Iter const& iter) noexcept
                : iter{iter}, temporary{0}, bitIter{0} {}

        /**
         * Constructs a new little endian bit output iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr LittleEndianBitOutputIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, temporary{0}, bitIter{0} {}

        /**
         * Constructs a new little endian bit output iterator
         */
        explicit constexpr LittleEndianBitOutputIter(
            void) noexcept = default;

        /**
         * Saves the given value in the wrapped output iterator
         *
         * @param value the value of the current bit
         * @return reference to this object
         */
        constexpr LittleEndianBitOutputIter& operator=(
            bit value) noexcept;

        /**
         * Returns the reference to this object [allows to assing
         * the value]
         *
         * @return the reference to this object
         */
        [[nodiscard]] constexpr LittleEndianBitOutputIter&
            operator* (void) noexcept
                { return *this; }

        /**
         * Returns the reference to this object
         *
         * @return reference to this object
         */
        constexpr LittleEndianBitOutputIter& operator++(
            void) noexcept
                { return *this; }

        /**
         * Returns the copy of this object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr LittleEndianBitOutputIter operator++(
            int) noexcept
                { return *this; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool operator== (
            LittleEndianBitOutputIter const& left,
            LittleEndianBitOutputIter const& right) noexcept
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
        constexpr void skipToNextByte(void) noexcept;

        /**
         * Saves the entire byte
         *
         * @param byte the saved byte
         */
        constexpr void saveByte(std::byte byte) noexcept;
    private:
        Iter                            iter;
        uint8                           temporary;
        uint8                           bitIter;
    };

    /**
     * Iterator returning the values of the individual bits in the
     * big endian manner
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteInputIterator Iter>
    class BigEndianBitInputIter : public std::iterator<
        BitInputIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitInputIteratorTag;

        /**
         * Constructs a new big endian bit input iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr BigEndianBitInputIter(
            Iter const& iter) noexcept
                : iter{iter}, bitIter{7} {}

        /**
         * Constructs a new big endian bit input iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr BigEndianBitInputIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, bitIter{7} {}

        /**
         * Constructs a new big endian bit input iterator
         */
        explicit constexpr BigEndianBitInputIter(
                void) noexcept = default;

        /**
         * Returns a value of the currently examined bit
         *
         * @return the value of the currently examined bit
         */
        [[nodiscard]] constexpr bit operator* (void) const noexcept
            { return ((1 << bitIter) & (*iter)) >> bitIter; }

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        constexpr BigEndianBitInputIter& operator++ (void) noexcept;

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr BigEndianBitInputIter operator++(
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
            BigEndianBitInputIter const& left,
            BigEndianBitInputIter const& right) noexcept
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

    /**
     * Iterator saving the values of the individual bits in the
     * big endian manner
     *
     * @tparam Iter the iterator type that iterates through
     * an individual bytes
     */
    template <ByteOutputIterator Iter>
    class BigEndianBitOutputIter : public std::iterator<
        BitOutputIteratorTag, bool>
    {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitOutputIteratorTag;

        /**
         * Constructs a new big endian bit output iterator from
         * the given iterator
         *
         * @param iter the constant reference to the iterator
         */
        explicit constexpr BigEndianBitOutputIter(
            Iter const& iter) noexcept
                : iter{iter}, temporary{0}, bitIter{7} {}

        /**
         * Constructs a new big endian bit output iterator from
         * the given iterator
         *
         * @param iter the rvalue reference to the iterator
         */
        explicit constexpr BigEndianBitOutputIter(
            Iter&& iter) noexcept
                : iter{std::move(iter)}, temporary{0}, bitIter{7} {}

        /**
         * Constructs a new big endian bit output iterator
         */
        explicit constexpr BigEndianBitOutputIter(
                void) noexcept = default;

        /**
         * Saves the given value in the wrapped output iterator
         *
         * @param value the value of the current bit
         * @return reference to this object
         */
        constexpr BigEndianBitOutputIter& operator=(
            bit value) noexcept;

        /**
         * Returns the reference to this object [allows to assing
         * the value]
         *
         * @return the reference to this object
         */
        [[nodiscard]] constexpr BigEndianBitOutputIter&
            operator* (void) noexcept
                { return *this; }

        /**
         * Returns the reference to this object
         *
         * @return reference to this object
         */
        constexpr BigEndianBitOutputIter& operator++(
            void) noexcept
                { return *this; }

        /**
         * Returns the copy of this object
         *
         * @return the copied object
         */
        [[nodiscard]] constexpr BigEndianBitOutputIter operator++(
            int) noexcept
                { return *this; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend constexpr bool operator== (
            BigEndianBitOutputIter const& left,
            BigEndianBitOutputIter const& right) noexcept
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
        constexpr void skipToNextByte(void) noexcept;

        /**
         * Saves the entire byte
         *
         * @param byte the saved byte
         */
        constexpr void saveByte(std::byte byte) noexcept;
    private:
        Iter                            iter;
        uint8                           temporary;
        uint8                           bitIter;
    };

    template <ByteInputIterator Iter>
    constexpr LittleEndianBitInputIter<Iter>&
        LittleEndianBitInputIter<Iter>::operator++ (void) noexcept
    {
        if (++bitIter == byteLength()) {
            ++iter;
            bitIter = 0;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        LittleEndianBitInputIter<Iter>::readByte(void) noexcept
    {
        bitIter = 0;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteOutputIterator Iter>
    constexpr LittleEndianBitOutputIter<Iter>&
        LittleEndianBitOutputIter<Iter>::operator=(bit value) noexcept
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
    constexpr void LittleEndianBitOutputIter<Iter>::skipToNextByte(
        void) noexcept
    {
        *iter++ = temporary;
        bitIter = temporary = 0;
    }

    template <ByteOutputIterator Iter>
    constexpr void LittleEndianBitOutputIter<Iter>::saveByte(
        std::byte byte) noexcept
    {
        temporary = bitIter = 0;
        *iter++ = byte;
    }

    template <ByteInputIterator Iter>
    constexpr BigEndianBitInputIter<Iter>&
        BigEndianBitInputIter<Iter>::operator++ (void) noexcept
    {
        if (!(bitIter--)) {
            ++iter;
            bitIter = 7;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    [[nodiscard]] constexpr std::byte
        BigEndianBitInputIter<Iter>::readByte(void) noexcept
    {
        bitIter = 7;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteOutputIterator Iter>
    constexpr BigEndianBitOutputIter<Iter>&
        BigEndianBitOutputIter<Iter>::operator=(bit value) noexcept
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
    constexpr void BigEndianBitOutputIter<Iter>::skipToNextByte(
        void) noexcept
    {
        *iter++ = temporary;
        bitIter = 7;
        temporary = 0;
    }

    template <ByteOutputIterator Iter>
    constexpr void BigEndianBitOutputIter<Iter>::saveByte(
        std::byte byte) noexcept
    {
        temporary = 0;
        bitIter = 7;
        *iter++ = byte;
    }

}

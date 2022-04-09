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

#include <MPGL/Traits/Concepts.hpp>

#include <iterator>
#include <string>
#include <tuple>

namespace mpgl {

    /**
     * Represents the given unicode ID in the utf-8 format
     */
    class ToUTF8Converter {
    public:
        /**
         * Returns a string containing the utf-8 representation
         * of the given glyph
         *
         * @tparam Tp the type of the unicode id integer
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp>
            requires ConstexprConstructible<std::string>
        [[nodiscard]] constexpr std::string
            operator() (Tp unicodeID) const;

        /**
         * Returns a string containing the utf-8 representation
         * of the given glyph
         *
         * @tparam Tp the type of the unicode id integer
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp>
            requires (!ConstexprConstructible<std::string>)
        [[nodiscard]] std::string
            operator() (Tp unicodeID) const;
    private:
        typedef std::tuple<
            uint32, uint32, uint8, uint8, uint32>      Coefficients;

        /**
         * Adds the given number of the utf-8 chars into the given
         * string
         *
         * @tparam Tp the type of the unicode id integer
         * @tparam Length the length of the tail
         * @param string the start string
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp, uint8 Length>
            requires ConstexprConstructible<std::string>
        constexpr std::string addTail(
            std::string string,
            Tp unicodeID) const;

        /**
         * Adds the given number of the utf-8 chars into the given
         * string
         *
         * @tparam Tp the type of the unicode id integer
         * @tparam Length the length of the tail
         * @param string the start string
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp, uint8 Length>
            requires (!ConstexprConstructible<std::string>)
        std::string addTail(
            std::string string,
            Tp unicodeID) const;

        /**
         * Converts the unicode ID to the utf-8 representation
         * of the given length
         *
         * @tparam Tp the type of the unicode id integer
         * @tparam Index the length of the representation
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp, std::size_t Index>
            requires ConstexprConstructible<std::string>
        constexpr std::string convert(Tp unicodeID) const;

        /**
         * Converts the unicode ID to the utf-8 representation
         * of the given length
         *
         * @tparam Tp the type of the unicode id integer
         * @tparam Index the length of the representation
         * @param unicodeID the unicode id of the glyph
         * @return the string with the utf-8 representation
         */
        template <std::integral Tp, std::size_t Index>
            requires (!ConstexprConstructible<std::string>)
        std::string convert(Tp unicodeID) const;

        /**
         * Returns the length of the utf-8 representation
         *
         * @param length the length of the integral type
         * @return the length of the utf-8 representatio
         */
        static constexpr std::size_t sequenceLength(
            std::size_t length) noexcept;

        static constexpr std::array<Coefficients, 5>    coefficients {
            Coefficients{0x3FFFFFF, 0x80000000, 0x1E, 0xFC, 0x3FFFFFFF},
            Coefficients{0x1FFFFF, 0x6000000, 0x18, 0xF8, 0xFFFFFF},
            Coefficients{0xFFFF, 0x1C0000, 0x12, 0xF0, 0x3FFFF},
            Coefficients{0x7FF, 0xF000, 0x0C, 0xE0, 0xFFF},
            Coefficients{0x7F, 0x7C0, 0x06, 0xC0, 0x3F}
        };
    };

    /**
     * Represents the given utf-8 sequence in the unicode id
     */
    class FromUTF8Converter {
    public:
        /**
         * Returns the unicode id of the given utf-8 sequence
         *
         * @tparam Iter the type of the iterator
         * @param iter the iterator to the begining of the sequence
         * @param end the iterator to the end of the sequence
         * @return the if of the unicode character
         */
        template <std::random_access_iterator Iter>
            requires std::same_as<std::iter_value_t<Iter>, char>
        [[nodiscard]] uint32 operator()(
            Iter iter,
            Iter const& end) const noexcept (NothrowReadable<Iter>);

        /**
         * Returns the unicode id of the given utf-8 sequence
         *
         * @tparam Iter the type of the iterator
         * @param iter the iterator to the begining of the sequence
         * @param end the iterator to the end of the sequence
         * @return the if of the unicode character
         */
        [[nodiscard]] uint32 operator()(
            std::string unicodeString) const;
    private:
        /**
         * Returns the unicode id of the given utf-8 sequence tail
         *
         * @tparam Iter the type of the iterator
         * @param iter the iterator to the begining of the sequence
         * @param end the iterator to the end of the sequence
         * @return the if of the unicode character
         */
        template <std::random_access_iterator Iter>
            requires std::same_as<std::iter_value_t<Iter>, char>
        uint32 decodeTail(
            Iter iter,
            Iter const& end) const noexcept(NothrowReadable<Iter>);

        /**
         * Returns the unicode id of the given utf-8 sequence tail
         *
         * @tparam unicode the utf-8 string
         * @return the if of the unicode character
         */
        uint32 decodeTail(
            std::string const& unicode) const noexcept;
    };

    inline constexpr ToUTF8Converter                    toUTF8;

    inline constexpr FromUTF8Converter                  fromUTF8;

    /**
     * Returns the length of a utf-8 sequence [based on the first
     * char]
     *
     * @param firstChar the sequence first char
     * @return the sequence length
     */
    [[nodiscard]] uint8 getUTF8SequenceLength(char firstChar);

    constexpr std::size_t ToUTF8Converter::sequenceLength(
        std::size_t length) noexcept
    {
        if (length > 3)
            return 5;
        if (length > 2)
            return 3;
        if (length > 1)
            return 2;
        return 1;
    }

    template <std::integral Tp, std::size_t Index>
        requires ConstexprConstructible<std::string>
    constexpr std::string ToUTF8Converter::convert(
        Tp unicodeID) const
    {
        auto const& [treshold, fmask, shift, pls, smask]
            = coefficients[Index - 1];
        if (unicodeID > treshold)
            return addTail<Tp, Index>(
                {char(((fmask & unicodeID) >> shift) + pls)},
                unicodeID & smask);
        if constexpr (Index > 1)
            return convert<Tp, Index - 1>(unicodeID);
        else
            return {char(0x7F & unicodeID)};
    }

    template <std::integral Tp, std::size_t Index>
        requires (!ConstexprConstructible<std::string>)
    std::string ToUTF8Converter::convert(
        Tp unicodeID) const
    {
        auto const& [treshold, fmask, shift, pls, smask]
            = coefficients[Index - 1];
        if (unicodeID > treshold)
            return addTail<Tp, Index>(
                {char(((fmask & unicodeID) >> shift) + pls)},
                unicodeID & smask);
        if constexpr (Index > 1)
            return convert<Tp, Index - 1>(unicodeID);
        else
            return {char(0x7F & unicodeID)};
    }

    template <std::integral Tp>
        requires ConstexprConstructible<std::string>
    [[nodiscard]] constexpr std::string ToUTF8Converter::operator() (
        Tp unicodeID) const
    {
        constexpr std::size_t size = sequenceLength(sizeof(Tp));

        if constexpr (size == 1)
            return {char(0x7F & unicodeID)};
        else
            return convert<Tp, size>(unicodeID);
    }

    template <std::integral Tp>
        requires (!ConstexprConstructible<std::string>)
    [[nodiscard]] std::string ToUTF8Converter::operator() (
        Tp unicodeID) const
    {
        constexpr std::size_t size = sequenceLength(sizeof(Tp));

        if constexpr (size == 1)
            return {char(0x7F & unicodeID)};
        else
            return convert<Tp, size>(unicodeID);
    }

    template <std::integral Tp, uint8 Length>
        requires ConstexprConstructible<std::string>
    constexpr std::string ToUTF8Converter::addTail(
        std::string string,
        Tp unicodeID) const
    {
        for (uint8 i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))
                ) >> 6 * (i - 1)) + 0x80;
        return string;
    }

    template <std::integral Tp, uint8 Length>
        requires (!ConstexprConstructible<std::string>)
    std::string ToUTF8Converter::addTail(
        std::string string,
        Tp unicodeID)  const
    {
        for (uint8 i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))
                ) >> 6 * (i - 1)) + 0x80;
        return string;
    }

    template <std::random_access_iterator Iter>
        requires std::same_as<std::iter_value_t<Iter>, char>
    uint32 FromUTF8Converter::decodeTail(
        Iter iter,
        Iter const& end) const noexcept(NothrowReadable<Iter>)
    {
        uint32 sum = 0;
        for (; iter != end; ++iter)
            sum = (sum << 6) + (*iter & 0x3F);
        return sum;
    }

    template <std::random_access_iterator Iter>
        requires std::same_as<std::iter_value_t<Iter>, char>
    [[nodiscard]] uint32 FromUTF8Converter::operator()(
        Iter iter,
        Iter const& end) const noexcept (NothrowReadable<Iter>)
    {
        auto size = std::distance(iter, end);
        uint8 bitmask = size != 1 ? (1 << (7 - size)) - 1 : 0x7F;
        uint32 front = (*iter & bitmask) << (6 * (size - 1));
        return front + decodeTail(++iter, end);
    }

}

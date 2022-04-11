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

namespace mpgl {

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

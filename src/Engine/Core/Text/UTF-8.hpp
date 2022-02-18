#pragma once

#include "../../Traits/Concepts.hpp"

#include <iterator>
#include <string>

namespace mpgl {

    template <std::integral Tp, uint8 Length>
        requires ConstexprConstructible<std::string>
    constexpr std::string addTail(std::string string, Tp unicodeID) {
        for (uint8 i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))
                ) >> 6 * (i - 1)) + 0x80;
        return string;
    }

    template <std::integral Tp, uint8 Length>
        requires (!ConstexprConstructible<std::string>)
    std::string addTail(std::string string, Tp unicodeID) {
        for (uint8 i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))
                ) >> 6 * (i - 1)) + 0x80;
        return string;
    }

    template <std::integral Tp>
        requires ConstexprConstructible<std::string>
    constexpr std::string toUTF8(Tp unicodeID) {
        if constexpr (sizeof(Tp) > 3) if (unicodeID > 0x3FFFFFF)
            return addTail<Tp, 5>({char(((0x80000000 & unicodeID) >> 0x1E) + 0xFC)}, unicodeID & 0x3FFFFFFF);
        if constexpr (sizeof(Tp) > 3) if (unicodeID > 0x1FFFFF)
            return addTail<Tp, 4>({char(((0x6000000 & unicodeID) >> 0x18) + 0xF8)}, unicodeID & 0xFFFFFF);
        if constexpr (sizeof(Tp) > 2) if (unicodeID > 0xFFFF)
            return addTail<Tp, 3>({char(((0x1C0000 & unicodeID) >> 0x12) + 0xF0)}, unicodeID & 0x3FFFF);
        if constexpr (sizeof(Tp) > 1) if (unicodeID > 0x7FF)
            return addTail<Tp, 2>({char(((0xF000 & unicodeID) >> 0x0C) + 0xE0)}, unicodeID & 0xFFF);
        if constexpr (sizeof(Tp) > 1) if (unicodeID > 0x7F)
            return addTail<Tp, 1>({char(((0x7C0 & unicodeID) >> 0x06) + 0xC0)}, unicodeID & 0x3F);
        return {char(0x7F & unicodeID)};
    }

    template <std::integral Tp>
        requires (!ConstexprConstructible<std::string>)
    std::string toUTF8(Tp unicodeID) {
        if constexpr (sizeof(Tp) > 3) if (unicodeID > 0x3FFFFFF)
            return addTail<Tp, 5>({char(((0x80000000 & unicodeID) >> 0x1E) + 0xFC)}, unicodeID & 0x3FFFFFFF);
        if constexpr (sizeof(Tp) > 3) if (unicodeID > 0x1FFFFF)
            return addTail<Tp, 4>({char(((0x6000000 & unicodeID) >> 0x18) + 0xF8)}, unicodeID & 0xFFFFFF);
        if constexpr (sizeof(Tp) > 2) if (unicodeID > 0xFFFF)
            return addTail<Tp, 3>({char(((0x1C0000 & unicodeID) >> 0x12) + 0xF0)}, unicodeID & 0x3FFFF);
        if constexpr (sizeof(Tp) > 1) if (unicodeID > 0x7FF)
            return addTail<Tp, 2>({char(((0xF000 & unicodeID) >> 0x0C) + 0xE0)}, unicodeID & 0xFFF);
        if constexpr (sizeof(Tp) > 1) if (unicodeID > 0x7F)
            return addTail<Tp, 1>({char(((0x7C0 & unicodeID) >> 0x06) + 0xC0)}, unicodeID & 0x3F);
        return {char(0x7F & unicodeID)};
    }

    template <std::random_access_iterator Iter>
        requires std::same_as<std::iter_value_t<Iter>, char>
    uint32 decodeTail(Iter iter, Iter end) noexcept {
        uint32 sum = 0;
        for (; iter != end; ++iter)
            sum = (sum << 6) + (*iter & 0x3F);
        return sum;
    }

    template <std::random_access_iterator Iter>
        requires std::same_as<std::iter_value_t<Iter>, char>
    uint32 fromUTF8(Iter iter, Iter end) {
        auto size = std::distance(iter, end);
        uint8 bitmask = size != 1 ? (1 << (7 - size)) - 1 : 0x7F;
        uint32 front = (*iter & bitmask) << (6 * (size - 1));
        return front + decodeTail(++iter, end);
    }

    uint32 decodeTail(std::string const& unicode) noexcept;

    uint32 fromUTF8(std::string unicodeString);

    uint8 getUTF8SequenceLength(char const& firstChar);

}

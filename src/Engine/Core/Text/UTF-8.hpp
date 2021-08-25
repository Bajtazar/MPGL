#pragma once

#include "../../Traits/Concepts.hpp"

#include <iterator>
#include <string>

namespace ge {

    template <std::integral Tp, uint8_t Length>
        requires ConstexprConstructible<std::string>
    constexpr std::string addTail(std::string string, Tp unicodeID) {
        for (uint8_t i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))) >> 6 * (i - 1)) + 0x80;
        return string;
    }

    template <std::integral Tp, uint8_t Length>
        requires (!ConstexprConstructible<std::string>)
    std::string addTail(std::string string, Tp unicodeID) {
        for (uint8_t i = Length; i != 0; --i)
            string += char((unicodeID & (0x3F << 6 * (i - 1))) >> 6 * (i - 1)) + 0x80;
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

}

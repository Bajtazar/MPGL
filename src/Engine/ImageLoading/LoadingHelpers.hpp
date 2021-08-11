#pragma once

#include "../Utility/Concepts.hpp"

#include <algorithm>
#include <istream>

namespace ge {

    template <NotSameSize<std::byte> T, bool BigEndian = false>
        requires std::is_trivially_constructible_v<T>
    T readType(std::istream& file) noexcept {
        T data;
        if constexpr (BigEndian) {
            char* raw = reinterpret_cast<char*>(&data) + sizeof(T) - 1;
            for (uint16_t i = sizeof(T);i != 0; --i, --raw)
                file.get(*raw);
        } else {
            char* raw = reinterpret_cast<char*>(&data);
            for (uint16_t i = 0;i != sizeof(T); ++i, ++raw)
                file.get(*raw);
        }
        return data;
    }

    template <SameSize<std::byte> T, bool BigEndian = false>
        requires std::is_trivially_constructible_v<T>
    T readType(std::istream& file) noexcept {
        T data;
        file.get(reinterpret_cast<char&>(data));
        return data;
    }

    std::string readNChars(std::size_t length, std::istream& file) noexcept;
    void ignoreNBytes(std::size_t length, std::istream& is) noexcept;

}

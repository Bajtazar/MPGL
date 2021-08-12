#pragma once

#include "../Utility/Concepts.hpp"

#include <algorithm>
#include <iterator>

namespace ge {

    template <NotSameSize<std::byte> T, bool BigEndian = false, std::input_iterator Iter>
        requires (std::is_trivially_constructible_v<T>
            && std::same_as<std::iter_value_t<Iter>, char>)
    T readType(Iter& iterator) noexcept(NothrowReadable<Iter>) {
        T data;
        if constexpr (BigEndian) {
            char* raw = reinterpret_cast<char*>(&data) + sizeof(T) - 1;
            for (uint16_t i = sizeof(T);i != 0; --i, --raw, ++iterator)
                *raw = *iterator;
        } else {
            char* raw = reinterpret_cast<char*>(&data);
            for (uint16_t i = 0;i != sizeof(T); ++i, ++raw, ++iterator)
                *raw = *iterator;
        }
        return data;
    }

    template <SameSize<std::byte> T, bool BigEndian = false, std::input_iterator Iter>
        requires (std::is_trivially_constructible_v<T>
            && std::same_as<std::iter_value_t<Iter>, char>)
    inline T readType(Iter& iterator) noexcept(NothrowReadable<Iter>) {
        return static_cast<T>(*iterator++);
    }

    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    std::string readNChars(std::size_t length, Iter& iter) noexcept(NothrowReadable<Iter>) {
        std::string data(length, ' ');
        std::ranges::for_each(data, [&iter](auto& c){ c = *iter++; });
        return data;
    }

    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    void ignoreNBytes(std::size_t length, Iter& iter) noexcept(NothrowReadable<Iter>) {
        for ([[maybe_unused]] auto _ : std::views::iota(std::size_t(0), length))
            [[maybe_unused]] char c = *iter++;
    }

}

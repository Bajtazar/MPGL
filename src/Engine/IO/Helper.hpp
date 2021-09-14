#pragma once

#include "../Traits/Concepts.hpp"

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

    template <bool BigEndian = false, std::integral U = int32_t, std::floating_point T = float,
        std::size_t Shift = 16, std::input_iterator Iter>
    inline T readFixed(Iter& iterator) noexcept(NothrowReadable<Iter>) {
        return static_cast<T>(readType<U, BigEndian>(iterator)) / (1 << Shift);
    }

    template <std::input_iterator Iter>
        requires (std::same_as<std::iter_value_t<Iter>, char>)
    std::string readNChars(std::size_t length, Iter& iter) noexcept(NothrowReadable<Iter>) {
        std::string data(length, ' ');
        std::ranges::for_each(data, [&iter](auto& c){ c = *iter++; });
        return data;
    }

}
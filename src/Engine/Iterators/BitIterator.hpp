#pragma once

#include <iterator>
#include <climits>

#include "../Traits/Concepts.hpp"

namespace ge {

    struct BitIteratorTag : public std::input_iterator_tag {};

    template <class Iter>
    concept BitIterator = std::input_iterator<Iter> &&
        requires {typename Iter::iterator_category;} &&
        std::derived_from< typename Iter::iterator_category, BitIteratorTag>;

    template <class Range>
    concept BitRange = std::ranges::input_range<Range> &&
        BitIterator<std::ranges::iterator_t<Range>>;

    template <std::integral T, bool BigEndian, BitIterator Iter>
        requires requires (Iter iter) { { iter.readByte() } -> std::same_as<std::byte>; }
    constexpr T readType(Iter& iter) noexcept;

    template <std::integral T, BitIterator Iter>
    constexpr T readNBits(std::size_t length, Iter& iter) noexcept;

    template <std::integral T, BitIterator Iter>
    constexpr T readRNBits(std::size_t length, Iter& iter) noexcept;

    template <ByteInputIterator Iter>
    class LittleEndianBitIter : public std::iterator<BitIteratorTag, bool> {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitIteratorTag;

        explicit constexpr LittleEndianBitIter(Iter const& iter) noexcept : iter{iter}, bitIter{0} {}
        explicit constexpr LittleEndianBitIter(Iter&& iter) noexcept : iter{std::move(iter)}, bitIter{0} {}
        explicit constexpr LittleEndianBitIter(void) noexcept = default;

        constexpr bit operator* (void) const noexcept { return ((1 << bitIter) & (*iter)) >> bitIter; }

        constexpr LittleEndianBitIter& operator++ (void) noexcept;
        constexpr LittleEndianBitIter  operator++  (int) noexcept;

        friend constexpr bool operator== (const LittleEndianBitIter& left, const LittleEndianBitIter& right) noexcept { return left.iter == right.iter; }

        static inline consteval uint8_t byteLength(void) noexcept { return CHAR_BIT; }

        constexpr void skipToNextByte(void) noexcept { ++iter; bitIter = 0; }
        constexpr std::byte readByte(void) noexcept;
    private:
        Iter iter;
        uint8_t bitIter;
    };

    template <ByteInputIterator Iter>
    class BigEndianBitIter : public std::iterator<BitIteratorTag, bool> {
    public:
        using bit =                     bool;
        using value_type =              bit;
        using iterator_category =       BitIteratorTag;

        explicit constexpr BigEndianBitIter(Iter&& iter) noexcept : iter{std::move(iter)}, bitIter{7} {}
        explicit constexpr BigEndianBitIter(Iter const& iter) noexcept : iter{iter}, bitIter{7} {}
        explicit constexpr BigEndianBitIter(void) noexcept = default;

        constexpr bit operator* (void) const noexcept { return ((1 << bitIter) & (*iter)) >> bitIter; }

        constexpr BigEndianBitIter& operator++ (void) noexcept;
        constexpr BigEndianBitIter  operator++  (int) noexcept;

        friend constexpr bool operator== (const BigEndianBitIter& left, const BigEndianBitIter& right) noexcept { return left.iter == right.iter; }

        static inline consteval uint8_t byteLength(void) noexcept { return CHAR_BIT; }

        constexpr void skipToNextByte(void) noexcept { ++iter; bitIter = 7; }
        constexpr std::byte readByte(void) noexcept;
    private:
        Iter iter;
        uint8_t bitIter;
    };

    template <ByteInputIterator Iter>
    constexpr LittleEndianBitIter<Iter>& LittleEndianBitIter<Iter>::operator++ (void) noexcept {
        ++bitIter;
        if (bitIter == byteLength()) {
            ++iter;
            bitIter = 0;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    constexpr LittleEndianBitIter<Iter> LittleEndianBitIter<Iter>::operator++ (int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    template <ByteInputIterator Iter>
    constexpr std::byte LittleEndianBitIter<Iter>::readByte(void) noexcept {
        bitIter = 0;
        return static_cast<std::byte>(*iter++);
    }

    template <ByteInputIterator Iter>
    constexpr BigEndianBitIter<Iter>& BigEndianBitIter<Iter>::operator++ (void) noexcept {
        if (!(bitIter--)) {
            ++iter;
            bitIter = 7;
        }
        return *this;
    }

    template <ByteInputIterator Iter>
    constexpr BigEndianBitIter<Iter> BigEndianBitIter<Iter>::operator++ (int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    template <ByteInputIterator Iter>
    constexpr std::byte BigEndianBitIter<Iter>::readByte(void) noexcept {
        bitIter = 7;
        return static_cast<std::byte>(*iter++);
    }

    template <std::integral T, bool BigEndian, BitIterator Iter>
        requires requires (Iter iter) { { iter.readByte() } -> std::same_as<std::byte>; }
    constexpr T readType(Iter& iter) noexcept {
        T data;
        if constexpr (BigEndian) {
            std::byte* raw = reinterpret_cast<std::byte*>(&data) + sizeof(T) - 1;
            for (uint8_t i = sizeof(T); i != 0; --i, --raw)
                *raw = iter.readByte();
        } else {
            std::byte* raw = reinterpret_cast<std::byte*>(&data);
            for (uint8_t i = 0; i != sizeof(T); ++i, ++raw)
                *raw = iter.readByte();
        }
        return data;
    }

    template <std::integral T, BitIterator Iter>
    constexpr T readNBits(std::size_t length, Iter& iter) noexcept {
        T answer = 0;
        for (std::size_t i = 0; i < length; ++i)
            answer += (*iter++) << i;
        return answer;
    }

    template <std::integral T, BitIterator Iter>
    constexpr T readRNBits(std::size_t length, Iter& iter) noexcept {
        T answer = 0;
        for (std::size_t i = 1; i <= length; ++i)
            answer += (*iter++) << (length - i);
        return answer;
    }

}

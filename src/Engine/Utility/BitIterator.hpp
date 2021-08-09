#pragma once

#include <iterator>
#include <climits>
#include <bitset>

#include "Concepts.hpp"

namespace ge {

    struct BitIteratorTag : public std::input_iterator_tag {};

    template <class Iter>
    concept BitIteratorConcept = std::input_iterator<Iter> &&
        requires {typename Iter::iterator_category;} &&
        std::derived_from< typename Iter::iterator_category, BitIteratorTag>;

    template <class Range>
    concept BitRange = std::ranges::input_range<Range> &&
        BitIteratorConcept<std::ranges::iterator_t<Range>>;

    template <std::input_iterator Iter, bool Direction = false>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    class BitIterator : public std::iterator<BitIteratorTag, bool> {
    public:
        typedef bool                bit;
        typedef bit                 value_type;
        using iterator_category  =  BitIteratorTag;

        explicit BitIterator(Iter&& iter) noexcept : iter{std::move(iter)}, bits{uint8_t(*(this->iter))}, bitIter{0} {}
        explicit BitIterator(const Iter& iter) noexcept : iter{iter}, bits{*iter}, bitIter{0} {}
        explicit BitIterator(void) noexcept = default;

        bit operator* (void) const noexcept { if constexpr (Direction) return bits[CHAR_BIT - bitIter - 1]; else return bits[bitIter]; }

        BitIterator& operator++ (void) noexcept;
        BitIterator operator++ (int) noexcept;

        friend bool operator== (const BitIterator& left, const BitIterator& right) noexcept { return left.iter == right.iter; }

        void skipToNextByte(void) noexcept { bits = std::bitset<CHAR_BIT>{uint8_t(*(++iter))}; bitIter = 0; }
        auto readByte(void) noexcept;
        template <typename T, bool BigEndian>
        T readType(void) noexcept;
        template <typename T>
        T readNBits(std::size_t length) noexcept;
        template <typename T>
        T readRNBits(std::size_t length) noexcept;
    private:
        Iter iter;
        std::bitset<CHAR_BIT> bits;
        uint8_t bitIter;
    };

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    BitIterator<Iter, Dir>& BitIterator<Iter, Dir>::operator++ (void) noexcept {
        ++bitIter;
        if (bitIter == CHAR_BIT) {
            bits = std::bitset<CHAR_BIT>{uint8_t(*(++iter))};
            bitIter = 0;
        }
        return *this;
    }

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    BitIterator<Iter, Dir> BitIterator<Iter, Dir>::operator++ (int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    auto BitIterator<Iter, Dir>::readByte(void) noexcept {
        auto temp = *iter++;
        bits = std::bitset<CHAR_BIT>{uint8_t(*iter)};
        bitIter = 0;
        return temp;
    }

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    template <typename T, bool BigEndian>
    T BitIterator<Iter, Dir>::readType(void) noexcept {
        T data;
        if constexpr (BigEndian) {
            char* raw = reinterpret_cast<char*>(&data) + sizeof(T) - 1;
            for (uint16_t i = sizeof(T);i != 0; --i, --raw, ++iter)
                *raw = *iter;
        } else {
            char* raw = reinterpret_cast<char*>(&data);
            for (uint16_t i = 0;i != sizeof(T); ++i, ++raw, ++iter)
                *raw = *iter;
        }
        bits = std::bitset<CHAR_BIT>{uint8_t(*iter)};
        bitIter = 0;
        return data;
    }

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    template <typename T>
    T BitIterator<Iter, Dir>::readNBits(std::size_t length) noexcept {
        T answer = 0;
        for (std::size_t i = 0;i < length; ++i)
            answer += (*(*this)++) << i;
        return answer;
    }

    template <std::input_iterator Iter, bool Dir>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    template <typename T>
    T BitIterator<Iter, Dir>::readRNBits(std::size_t length) noexcept {
        T answer = 0;
        for ([[maybe_unused]] std::size_t i = 1;i <= length; ++i)
            answer += (*(*this)++) << (length - i);
        return answer;
    }

    template <std::input_iterator Iter>
    using ReverseBitIter = BitIterator<Iter, true>;

}

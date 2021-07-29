#pragma once

#include <iterator>
#include <climits>
#include <bitset>

#include "Concepts.hpp"

namespace ge {

    template <std::input_iterator Iter>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    class BitIterator : public std::iterator<std::input_iterator_tag, bool> {
    public:
        typedef bool                bit;
        typedef bit                 value_type;
        using iterator_category  =  std::input_iterator_tag;

        explicit BitIterator(Iter&& iter) noexcept : iter{std::move(iter)}, bits{uint8_t(*(this->iter))}, bitIter{0} {}
        explicit BitIterator(const Iter& iter) noexcept : iter{iter}, bits{*iter}, bitIter{0} {}
        explicit BitIterator(void) noexcept = default;

        bit operator* (void) const noexcept { return bits[bitIter]; }

        BitIterator& operator++ (void) noexcept;
        BitIterator operator++ (int) noexcept;

        friend bool operator== (const BitIterator& left, const BitIterator& right) noexcept { return left.iter == right.iter; }

    private:
        Iter iter;
        std::bitset<CHAR_BIT> bits;
        uint8_t bitIter;
    };

    template <std::input_iterator Iter>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    BitIterator<Iter>& BitIterator<Iter>::operator++ (void) noexcept {
        ++bitIter;
        if (bitIter == CHAR_BIT) {
            bits = std::bitset<CHAR_BIT>{uint8_t(*(++iter))};
            bitIter = 0;
        }
        return *this;
    }

    template <std::input_iterator Iter>
        requires requires (Iter it) { { *it } -> SameSize<std::byte>; }
    BitIterator<Iter> BitIterator<Iter>::operator++ (int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }

}

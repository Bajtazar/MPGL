#pragma once

#include "../Mathematics/Matrix.hpp"

#include <ranges>
#include <vector>
#include <array>

namespace ge {

    template <uint8_t Size>
        requires (Size > 1)
    class ZigZacRange {
    public:
        ZigZacRange(void) noexcept = delete;

        template <class Range>
        using RangeMatrix = Matrix<std::ranges::range_value_t<Range>, Size>;

        template <std::ranges::random_access_range Range>
        static RangeMatrix<Range> returnZigZac(Range const& range) noexcept;
    private:
        constexpr static void nextPosition(std::size_t& first, std::size_t& second, bool& direction) noexcept;
        consteval static std::array<std::array<std::size_t, Size>, Size> generateZigZacArray(void) noexcept;

        constexpr static std::array<std::array<std::size_t, Size>, Size> zigzac = generateZigZacArray();
    };

    template <uint8_t Size>
        requires (Size > 1)
    template <std::ranges::random_access_range Range>
    ZigZacRange<Size>::RangeMatrix<Range> ZigZacRange<Size>::returnZigZac(Range const& range) noexcept {
        RangeMatrix<Range> matrix;
        for (auto i : std::views::iota(uint8_t(0), Size))
            for (auto j : std::views::iota(uint8_t(0), Size))
                matrix[i][j] = range[zigzac[i][j]];
        return matrix;
    }

    template <uint8_t Size>
        requires (Size > 1)
    constexpr void ZigZacRange<Size>::nextPosition(std::size_t& first, std::size_t& second, bool& direction) noexcept {
        if (first == Size - 1) {
            ++second;
            direction = !direction;
        } else if (!second) {
            ++first;
            direction = !direction;
        } else {
            ++first;
            --second;
        }
    }

    template <uint8_t Size>
        requires (Size > 1)
    consteval std::array<std::array<std::size_t, Size>, Size> ZigZacRange<Size>::generateZigZacArray(void) noexcept {
        std::array<std::array<std::size_t, Size>, Size> matrix;
        bool direction = true;
        for (std::size_t iter = 0, x = 0, y = 0; iter != Size * Size; ++iter) {
            matrix[y][x] = iter;
            if (direction)
                nextPosition(x, y, direction);
            else
                nextPosition(y, x, direction);
        }
        return matrix;
    }

}

#pragma once

#include "../Mathematics/Matrix.hpp"

#include <ranges>
#include <vector>
#include <array>

namespace ge {

    template <uint8 Size>
        requires (Size > 1)
    class ZigZacRange {
    public:
        ZigZacRange(void) noexcept = delete;

        template <class Range>
        using RangeMatrix = Matrix<std::ranges::range_value_t<Range>, Size, Size>;

        template <std::ranges::random_access_range Range>
        static RangeMatrix<Range> returnZigZac(Range const& range) noexcept;
    private:
        typedef std::array<std::array<std::size_t, Size>, Size>     ZigZacBase;

        constexpr static void nextPosition(
            std::size_t& first, std::size_t& second, bool& direction) noexcept;
        consteval static ZigZacBase generateZigZacArray(void) noexcept;

        constexpr static ZigZacBase                                 zigzac
            = generateZigZacArray();
    };

    template <uint8 Size>
        requires (Size > 1)
    template <std::ranges::random_access_range Range>
    ZigZacRange<Size>::RangeMatrix<Range>
        ZigZacRange<Size>::returnZigZac(Range const& range) noexcept
    {
        RangeMatrix<Range> matrix;
        for (auto i : std::views::iota(uint8(0), Size))
            for (auto j : std::views::iota(uint8(0), Size))
                matrix[i][j] = range[zigzac[i][j]];
        return matrix;
    }

    template <uint8 Size>
        requires (Size > 1)
    constexpr void ZigZacRange<Size>::nextPosition(
        std::size_t& first, std::size_t& second, bool& direction) noexcept
    {
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

    template <uint8 Size>
        requires (Size > 1)
    consteval ZigZacRange<Size>::ZigZacBase
        ZigZacRange<Size>::generateZigZacArray(void) noexcept
    {
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

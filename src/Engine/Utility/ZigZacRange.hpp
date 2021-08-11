#pragma once

#include "../Mathematics/Matrix.hpp"

#include <ranges>
#include <vector>
#include <array>

// will be heavely modified

namespace ge {

    class ZigZacRange {
    public:
        ZigZacRange(void) noexcept = delete;

        template <class Range>
        using RangeMatrix = Matrix<std::ranges::range_value_t<Range>, 8>;

        template <std::ranges::random_access_range Range>
        static RangeMatrix<Range> returnZigZac(Range const& range) noexcept;
    private:
        constexpr static std::array<std::array<std::size_t, 8>, 8> zigzac {{
            {0,  1,  5,  6,  14, 15, 27, 28},
            {2,  4,  7,  13, 16, 26, 29, 42},
            {3,  8,  12, 17, 25, 30, 41, 43},
            {9,  11, 18, 24, 31, 40, 44, 53},
            {10, 19, 23, 32, 39, 45, 52, 54},
            {20, 22, 33, 38, 46, 51, 55, 60},
            {21, 34, 37, 47, 50, 56, 59, 61},
            {35, 36, 48, 49, 57, 58, 62, 63}
        }};
    };

    template <std::ranges::random_access_range Range>
    ZigZacRange::RangeMatrix<Range> ZigZacRange::returnZigZac(Range const& range) noexcept {
        RangeMatrix<Range> matrix;
        for (auto i : std::views::iota(0, 8))
            for (auto j : std::views::iota(0, 8))
                matrix[i][j] = range[zigzac[i][j]];
        return matrix;
    }

}

#pragma once

#include "../Mathematics/Matrix.hpp"
#include "../Utility/Concepts.hpp"

#include <numeric>
#include <vector>

namespace ge {

    template <uint8_t Precision = 8>
    class IDCT {
    public:
        explicit IDCT(void) noexcept;

        template <std::ranges::random_access_range Range>
        using InvertedMatrix = Matrix<std::ranges::range_value_t<Range>, Precision>;

        template <std::ranges::random_access_range Range>
        InvertedMatrix<Range> invertVector(const Range& range) noexcept;
    private:
        Matrix<double, Precision> inverseTable;
        double invertPoint(uint8_t i, uint8_t j) const noexcept;
    };

    template <uint8_t Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (auto i : std::views::iota(uint8_t(0), Precision))
            for (auto j : std::views::iota(uint8_t(0), Precision))
                inverseTable[i][j] = std::cos(
                    i * (2.*j + 1.) * std::numbers::pi_v<double> / 16.);
        for (auto& value : inverseTable.getColumn(0))
            value /= sqrt(2.);
    }

    template <uint8_t Precision>
    double IDCT<Precision>::invertPoint(uint8_t i, uint8_t j) const noexcept {
        double sum = 0;
        for (auto x : std::views::iota(uint8_t(0), Precision))
            for (auto y : std::views::iota(uint8_t(0), Precision))
                sum += inverseTable[x][i] * inverseTable[y][j];
        return sum / 4.;
    }

    template <uint8_t Precision>
    template <std::ranges::random_access_range Range>
    IDCT<Precision>::InvertedMatrix<Range> IDCT<Precision>::invertVector(const Range& range) noexcept {
        InvertedMatrix<Range> matrix;
        output.resize(Precision * Precision);
        for (auto i : std::views::iota(uint8_t(0), Precision))
            for (auto j : std::views::iota(uint8_t(0), Precision))
                matrix[i][j] = invertPoint(i, j);
        return matrix;
    }

}

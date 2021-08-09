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

        template <Arithmetic T>
        Matrix<T, Precision> invertVector(const Matrix<T, Precision>& range) const noexcept;
    private:
        Matrix<long double, Precision> inverseTable;
        template <Arithmetic T>
        long double invertPoint(uint8_t i, uint8_t j, const Matrix<T, Precision>& range) const noexcept;
    };

    template <uint8_t Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (auto i : std::views::iota(uint8_t(0), Precision))
            for (auto j : std::views::iota(uint8_t(0), Precision))
                inverseTable[i][j] = std::cos(
                    (2. * j + 1.) * i * std::numbers::pi_v<long double> / 16.);
        for (auto& value : inverseTable[0])
            value /= std::sqrt(2.);
    }

    template <uint8_t Precision>
    template <Arithmetic T>
    long double IDCT<Precision>::invertPoint(uint8_t i, uint8_t j, const Matrix<T, Precision>& range) const noexcept {
        long double sum = 0;
        for (auto x : std::views::iota(uint8_t(0), Precision))
            for (auto y : std::views::iota(uint8_t(0), Precision))
                sum += range[x][y] * inverseTable[y][j] * inverseTable[x][i];
        return 2. * sum / Precision;
    }

    template <uint8_t Precision>
    template <Arithmetic T>
    Matrix<T, Precision> IDCT<Precision>::invertVector(const Matrix<T, Precision>& range) const noexcept {
        Matrix<T, Precision> matrix;
        for (auto i : std::views::iota(uint8_t(0), Precision))
            for (auto j : std::views::iota(uint8_t(0), Precision))
                matrix[i][j] = invertPoint(i, j, range);
        // will be much optimylised
        return matrix;
    }

}

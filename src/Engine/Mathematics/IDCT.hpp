#pragma once

#include "../Utility/BitReversion.hpp"
#include "../Traits/Concepts.hpp"
#include "Matrix.hpp"

#include <numeric>
#include <complex>
#include <climits>
#include <vector>
#include <array>

namespace ge {

    template <uint8_t Precision = 8>
        requires IsPowerOf2<uint8_t, Precision>
    class IDCT {
    public:
        explicit IDCT(void) noexcept;

        template <Arithmetic T>
        Matrix<T, Precision> operator() (Matrix<T, Precision> const& matrix) const noexcept;
    private:
        consteval static uint8_t log2N(void) noexcept { return ::ge::log2N<uint8_t, Precision>(); }

        typedef std::complex<double>                                    Complex;
        typedef std::array<Complex, Precision>                          ComplexArray;
        typedef std::array<std::vector<Complex>, log2N()>               ComplexVector;
        typedef typename Matrix<double, Precision>::column_value_type   Column;

        template <typename T, typename U>
        void idct(T const& array, U& output) const noexcept;

        template <typename T, typename U>
        void idct(T const& array, U&& output) const noexcept;

        template <typename T>
        void prepareForFFT(ComplexArray& array, T& output) const noexcept;

        void fastFourierTransformation(ComplexArray& array) const noexcept;

        ComplexArray preprocessing;
        ComplexVector lookupTable;
    };

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <Arithmetic T>
    Matrix<T, Precision> IDCT<Precision>::operator() (Matrix<T, Precision> const& matrix) const noexcept {
        Matrix<double, Precision> helper;
        Matrix<T, Precision> output;
        for (uint8_t y : std::views::iota(uint8_t(0), Precision))
            idct(matrix[y], std::move(helper[y]));
        for (uint8_t x : std::views::iota(uint8_t(0), Precision)) {
            Column column;
            idct(helper.getColumn(x), column);
            column *= 2. / Precision;
            output.getColumn(x) = column;
        }
        return output;
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <typename T, typename U>
    void IDCT<Precision>::idct(T const& array, U& output) const noexcept {
        ComplexArray complexArray;
        std::ranges::transform(array, complexArray.begin(), [](const auto& v) -> Complex { return v; });
        prepareForFFT(complexArray, output);
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <typename T, typename U>
    void IDCT<Precision>::idct(T const& array, U&& output) const noexcept {
        idct(array, output);
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <typename T>
    void IDCT<Precision>::prepareForFFT(ComplexArray& array, T& output) const noexcept {
        std::ranges::transform(array, preprocessing, array.begin(),
            [](auto const& value, auto const& polar) -> Complex { return value.real() * polar; });
        fastFourierTransformation(array);
        for (uint8_t i : std::views::iota(uint8_t(0), uint8_t(Precision / 2))) {
            output[2 * i] = array[i].real();
            output[2 * i + 1] = array[Precision - i - 1].real();
        }
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    void IDCT<Precision>::fastFourierTransformation(ComplexArray& array) const noexcept {
        for (uint8_t i : std::views::iota(uint8_t(0), uint8_t(Precision / 2)))
            std::swap(array[reverseBits(i) >> (CHAR_BIT - log2N())], array[i]);
        for (uint8_t i = 1, m = 2, sm = 1; i <= log2N(); ++i, sm = m, m <<= 1) {
            for (uint8_t k = 0; k < Precision; k += m) {
                for (uint8_t j = k; j < k + sm; ++j) {
                    auto t = array[j + sm] * lookupTable[i - 1][j - k];
                    auto u = array[j];
                    array[j] = u + t;
                    array[j + sm] = u - t;
                }
            }
        }
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (uint8_t i : std::views::iota(uint8_t(0), Precision))
            preprocessing[i] = std::polar(1., -i * std::numbers::pi / (2. * Precision));
        preprocessing.front() /= std::sqrt(2.);
        for (uint8_t i = 0, k = 1; i < log2N(); ++i, k <<= 1) {
            auto complex = std::polar(1., -std::numbers::pi / k);
            lookupTable[i].emplace_back(1.);
            for (uint8_t j = 1; j < k; ++j)
                lookupTable[i].push_back(lookupTable[i][j - 1] * complex);
        }
    }

}
#pragma once

#include "../Utility/BitReversion.hpp"
#include "../Utility/FixedRange.hpp"
#include "../Traits/Concepts.hpp"
#include "Matrix.hpp"
#include "FFT.hpp"

#include <climits>
#include <vector>
#include <array>

namespace ge {

    template <uint8_t Precision = 8>
        requires IsPowerOf2<uint8_t, Precision>
    class IDCT {
    public:
        template <typename Tp>
        using PrecMatrix =                                  Matrix<Tp, Precision, Precision>;

        explicit IDCT(void) noexcept;

        template <Arithmetic Tp>
        PrecMatrix<Tp> operator() (
            PrecMatrix<Tp> const& matrix) const noexcept;
    private:
        consteval static uint8_t log2N(void) noexcept
            { return ::ge::log2N<uint8_t, Precision>(); }

        typedef std::complex<double>                        Complex;
        typedef FixedRange<Precision,
            std::array<Complex, Precision>>                 ComplexArray;
        typedef std::array<std::vector<Complex>, log2N()>   ComplexVector;

        template <typename Tp, typename Up>
        void idct(Tp const& array, Up& output) const noexcept;

        template <typename Tp>
        void prepareForFFT(ComplexArray& array, Tp& output) const noexcept;

        ComplexArray                                        preprocessing;

        static constexpr FFT                                fft{};
    };

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <Arithmetic Tp>
    Matrix<Tp, Precision, Precision>
        IDCT<Precision>::operator() (
            PrecMatrix<Tp> const& matrix) const noexcept
    {
        PrecMatrix<double> helper;
        PrecMatrix<Tp> output;
        for (uint8_t y = 0; y < Precision; ++y)
            idct(matrix.getColumn(y), helper.getColumn(y));
        for (uint8_t x = 0; x < Precision; ++x) {
            idct(helper[x], output[x]);
            output[x] /= Precision / 2;
        }
        return output;
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <typename Tp, typename Up>
    void IDCT<Precision>::idct(Tp const& array, Up& output) const noexcept {
        ComplexArray complexArray;
        std::ranges::transform(array, complexArray.begin(),
            [](auto const& v) -> Complex { return v; });
        prepareForFFT(complexArray, output);
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    template <typename Tp>
    void IDCT<Precision>::prepareForFFT(
        ComplexArray& array, Tp& output) const noexcept
    {
        std::ranges::transform(array, preprocessing, array.begin(),
            [](auto const& value, auto const& polar)
                -> Complex { return value.real() * polar; });
        fft(array);
        for (uint8_t i = 0; i < Precision / 2; ++i) {
            output[2 * i] = array[i].real();
            output[2 * i + 1] = array[Precision - i - 1].real();
        }
    }

    template <uint8_t Precision>
        requires IsPowerOf2<uint8_t, Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (uint8_t i : std::views::iota(uint8_t(0), Precision))
            preprocessing[i] = std::polar(1.,
                -i * std::numbers::pi / (2. * Precision));
        preprocessing.front() /= std::sqrt(2.);
    }

}

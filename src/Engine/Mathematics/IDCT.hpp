#pragma once

#include "../Utility/BitReversion.hpp"
#include "../Utility/FixedRange.hpp"
#include "../Traits/Concepts.hpp"
#include "Matrix.hpp"
#include "FFT.hpp"

#include <climits>
#include <array>

namespace ge {

    template <uint8 Precision = 8>
        requires IsPowerOf2<uint8, Precision>
    class IDCT {
    public:
        template <typename Tp>
        using PrecMatrix =                                  Matrix<Tp, Precision, Precision>;

        explicit IDCT(void) noexcept;

        template <Arithmetic Tp>
        PrecMatrix<Tp> operator() (
            PrecMatrix<Tp> const& matrix) const noexcept;
    private:
        consteval static uint8 log2N(void) noexcept
            { return ::ge::log2N<uint8, Precision>(); }

        typedef std::complex<float64>                       Complex;
        typedef FixedRange<Precision,
            std::array<Complex, Precision>>                 ComplexArray;

        template <typename Tp, typename Up>
        void idct(Tp const& array, Up& output) const noexcept;

        template <typename Tp>
        void prepareForFFT(ComplexArray& array, Tp& output) const noexcept;

        ComplexArray                                        preprocessing;

        static constexpr FFT                                fft{};
    };

    template <uint8 Precision>
        requires IsPowerOf2<uint8, Precision>
    template <Arithmetic Tp>
    Matrix<Tp, Precision, Precision>
        IDCT<Precision>::operator() (
            PrecMatrix<Tp> const& matrix) const noexcept
    {
        PrecMatrix<float64> helper;
        PrecMatrix<Tp> output;
        for (uint8 y = 0; y < Precision; ++y)
            idct(matrix.getColumn(y), helper.getColumn(y));
        for (uint8 x = 0; x < Precision; ++x) {
            idct(helper[x], output[x]);
            output[x] /= Precision / 2;
        }
        return output;
    }

    template <uint8 Precision>
        requires IsPowerOf2<uint8, Precision>
    template <typename Tp, typename Up>
    void IDCT<Precision>::idct(Tp const& array, Up& output) const noexcept {
        ComplexArray complexArray;
        std::ranges::transform(array, complexArray.begin(),
            [](auto const& v) -> Complex { return v; });
        prepareForFFT(complexArray, output);
    }

    template <uint8 Precision>
        requires IsPowerOf2<uint8, Precision>
    template <typename Tp>
    void IDCT<Precision>::prepareForFFT(
        ComplexArray& array, Tp& output) const noexcept
    {
        std::ranges::transform(array, preprocessing, array.begin(),
            [](auto const& value, auto const& polar)
                -> Complex { return value.real() * polar; });
        fft(array);
        for (uint8 i = 0; i < Precision / 2; ++i) {
            output[2 * i] = array[i].real();
            output[2 * i + 1] = array[Precision - i - 1].real();
        }
    }

    template <uint8 Precision>
        requires IsPowerOf2<uint8, Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (uint8 i : std::views::iota(uint8(0), Precision))
            preprocessing[i] = std::polar(1.,
                -i * std::numbers::pi / (2. * Precision));
        preprocessing.front() /= std::sqrt(2.);
    }

}

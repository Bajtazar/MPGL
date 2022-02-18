#pragma once

#include "../Utility/BitReversion.hpp"
#include "../Utility/FixedRange.hpp"
#include "../Traits/Concepts.hpp"
#include "Matrix.hpp"
#include "FFT.hpp"

#include <climits>
#include <array>

namespace mpgl {

    template <uint8 Precision = 8>
        requires IsPowerOf2<uint8, Precision>
    class IDCT {
    public:
        template <typename Tp>
        using PrecMatrix =                                  SquareMatrix<Tp, Precision>;

        explicit IDCT(void) noexcept;

        template <Arithmetic Tp>
        PrecMatrix<Tp> operator() (
            PrecMatrix<Tp> const& matrix) const noexcept;
    private:
        consteval static uint8 log2N(void) noexcept
            { return ::mpgl::log2N<uint8, Precision>(); }

        typedef VectorC<Precision>                          ComplexVec;

        template <typename Tp, typename Up>
        void idct(Tp const& array, Up& output) const noexcept;

        ComplexVec                                          preprocessing;
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
        ComplexVec vector = static_cast<Vector<
            std::ranges::range_value_t<Tp>, Precision>>(array);
        vector *= preprocessing;
        fft(vector);
        for (uint8 i = 0; i < Precision / 2; ++i) {
            output[2 * i] = vector[i].real();
            output[2 * i + 1] = vector[Precision - i - 1].real();
        }
    }

    template <uint8 Precision>
        requires IsPowerOf2<uint8, Precision>
    IDCT<Precision>::IDCT(void) noexcept {
        for (uint8 i = 0; i != Precision; ++i)
            preprocessing[i] = std::polar(1.,
                -i * std::numbers::pi / (2. * Precision));
        preprocessing[0] /= std::sqrt(2.);
    }

}

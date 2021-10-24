#pragma once

#include "../Utility/BitReversion.hpp"
#include "../Traits/Concepts.hpp"

#include <math.h>
#include <numeric>
#include <complex>
#include <climits>
#include <algorithm>

namespace ge {

    class FFT {
    public:
        typedef std::complex<double>    Complex;

        explicit FFT(void) noexcept = default;

        template <std::ranges::random_access_range Range>
            requires std::ranges::sized_range<Range>
        void operator() (Range&& range) noexcept;
    private:
        typedef std::vector<Complex>    ComplexVector;

        static uint8_t log2N(uint8_t number) noexcept;
        static uint16_t convolutionSize(uint8_t number) noexcept;

        template <std::ranges::random_access_range Range>
        void randomRangeFFT(Range&& range) noexcept;

        ComplexVector generateLookupTable(uint16_t size) noexcept;

        template <std::ranges::random_access_range Range>
        ComplexVector generateLeftSequence(Range&& range,
            ComplexVector const& lookupTable, uint16_t size) noexcept;

        ComplexVector generateRightSequence(ComplexVector const& lookupTable,
            uint16_t size) noexcept;

        ComplexVector convolve(ComplexVector leftSequence,
            ComplexVector rightSequence) noexcept;

        template <std::ranges::random_access_range Range>
        void cooleyTukey(Range&& range, double constant = -2.) noexcept;

        template <std::ranges::random_access_range Range>
        void bluestein(Range&& range) noexcept;
    };

    template <std::ranges::random_access_range Range>
    void FFT::randomRangeFFT(Range&& range) noexcept {
        std::size_t size = range.size();
        if ((size & (size - 1)) && size)
            bluestein(std::forward<Range>(range));
        else
            cooleyTukey(std::forward<Range>(range));
    }

    template <std::ranges::random_access_range Range>
        requires std::ranges::sized_range<Range>
    void FFT::operator() (Range&& range) noexcept {
        if constexpr (SameSignatures<std::size_t(void),
            &std::remove_cvref_t<Range>::size>)
        {
            if constexpr (IsConstexprEvaluableV<std::size_t(void),
                &std::remove_cvref_t<Range>::size>)
            {

            } else
                randomRangeFFT(std::forward<Range>(range));
        } else
            randomRangeFFT(std::forward<Range>(range));
    }


    template <std::ranges::random_access_range Range>
    void FFT::cooleyTukey(Range&& range, double constant) noexcept {
        auto log = log2N(range.size());
        for (uint8_t i = 0; i < range.size() / 2; ++i)
            std::swap(range[reverseBits(i) >> (CHAR_BIT - log)], range[i]);
        for (uint8_t i = 1, m = 2, sm = 1; i <= log; ++i, sm = m, m <<= 1) {
            auto omegaM = std::polar(1., constant * std::numbers::pi / m);
            for (uint8_t k = 0; k < range.size(); k += m) {
                std::complex<double> omega = 1.;
                for (uint8_t j = k; j < (k + sm); ++j, omega *= omegaM) {
                    auto t = omega * range[j + sm];
                    auto u = range[j];
                    range[j] += t;
                    range[j + sm] = u - t;
                }
            }
        }
    }

    template <std::ranges::random_access_range Range>
    void FFT::bluestein(Range&& range) noexcept {
        auto lookupTable = generateLookupTable(range.size());
        auto convolution = convolutionSize(range.size());
        ComplexVector convoluted = convolve(generateLeftSequence(range, lookupTable,
            convolution), generateRightSequence(lookupTable, convolution));
        for (uint16_t i = 0; i < range.size(); ++i)
            range[i] = convoluted[i] * std::conj(lookupTable[i]);
    }

    template <std::ranges::random_access_range Range>
    FFT::ComplexVector FFT::generateLeftSequence(Range&& range,
        ComplexVector const& lookupTable, uint16_t size) noexcept
    {
        ComplexVector leftSequence;
        leftSequence.reserve(size);
        for (uint16_t i = 0; i < range.size(); ++i)
            leftSequence.push_back(range[i] * std::conj(lookupTable[i]));
        leftSequence.resize(size);
        return leftSequence;
    }

}

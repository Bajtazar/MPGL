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
        void operator() (Range&& range) const noexcept;
    private:
        typedef std::vector<Complex>    ComplexVector;

        static uint8_t log2N(uint8_t number) noexcept;
        static uint16_t convolutionSize(uint8_t number) noexcept;

        template <std::ranges::random_access_range Range>
        static void randomRangeFFT(Range&& range) noexcept;

        static ComplexVector generateLookupTable(uint16_t size) noexcept;

        template <std::ranges::random_access_range Range>
        static ComplexVector generateLeftSequence(Range&& range,
            ComplexVector const& lookupTable, uint16_t size) noexcept;

        static ComplexVector generateRightSequence(
            ComplexVector const& lookupTable, uint16_t size) noexcept;

        static ComplexVector convolve(ComplexVector leftSequence,
            ComplexVector rightSequence) noexcept;

        template <std::ranges::random_access_range Range>
        static void cooleyTukey(Range&& range, double sign = -1.) noexcept;

        template <std::ranges::random_access_range Range>
        static void bluestein(Range&& range) noexcept;

        template <uint16_t Id, bool Inverse>
        struct CooleyTukeyBasePart {
            typedef std::array<Complex, 1 << Id>    Table;

            static Table const                      lookupTable;

            static Table generateLookupTable(void) noexcept;
        };

        template <uint16_t Size, bool Inverse>
        static constexpr decltype(auto) constructor(void) noexcept {
            if constexpr (Size == 0)
                return std::tuple<CooleyTukeyBasePart<0, Inverse>>{};
            else
                return std::tuple_cat(std::tuple<CooleyTukeyBasePart<Size, Inverse>>{},
                    constructor<Size - 1, Inverse>());
        }

        template <typename U>
        struct CooleyTukeyTupleHelper;

        template <typename... Args>
        struct CooleyTukeyTupleHelper<std::tuple<Args...>> : public Args... {};

        template <uint16_t Size, bool Inverse = false>
        class CooleyTukeyBase : private CooleyTukeyTupleHelper<
            decltype(constructor<::ge::log2N<uint16_t, Size>(), Inverse>())>
        {
        public:
            template <std::ranges::random_access_range Range>
            void operator() (Range&& range) const noexcept;
        private:
            template <std::size_t Index, std::ranges::random_access_range Range>
            void innerLoop(Range&& range) const noexcept;

            static constexpr uint16_t log2N() { return ::ge::log2N<uint16_t, Size>(); }
        };

        template <uint8_t Size>
        class BluesteinBase {
        public:
            template <std::ranges::random_access_range Range>
            void operator() (Range&& range) const noexcept;
        private:
            static constexpr uint16_t convoluteLen(void) noexcept;

            typedef std::array<Complex, convoluteLen()>     ConvolutionArray;
            typedef std::array<Complex, Size>               Table;

            template <std::ranges::random_access_range Range>
            ConvolutionArray generateLeftConvolution(Range&& range) const noexcept;

            static ConvolutionArray generateRightConvolution(void) noexcept;
            static Table generateLookupTable(void) noexcept;

            static Table const                              lookupTable;
            static ConvolutionArray const                   rightConvolution;
        };

        template <uint8_t Size>
        class ConstexprSizeFFT : public std::conditional_t<IsPowerOf2<uint8_t, Size>,
            CooleyTukeyBase<Size>, BluesteinBase<Size>> {};
    };

    template <uint16_t Id, bool Inverse>
    FFT::CooleyTukeyBasePart<Id, Inverse>::Table
        FFT::CooleyTukeyBasePart<Id, Inverse>::generateLookupTable(void) noexcept
    {
        Table table;
        uint16_t size = table.size();
        double constant = (Inverse ? 1. : -1.) * std::numbers::pi / size;
        for (uint16_t i = 0;i != size; ++i)
            table[i] = std::polar(1., constant * i);
        return table;
    }

    template <uint16_t Id, bool Inverse>
    FFT::CooleyTukeyBasePart<Id, Inverse>::Table const
        FFT::CooleyTukeyBasePart<Id, Inverse>::lookupTable = generateLookupTable();

    template <uint8_t Id>
    FFT::BluesteinBase<Id>::Table const FFT::BluesteinBase<Id>::lookupTable =
        FFT::BluesteinBase<Id>::generateLookupTable();

    template <uint8_t Id>
    FFT::BluesteinBase<Id>::ConvolutionArray const FFT::BluesteinBase<Id>::rightConvolution =
        FFT::BluesteinBase<Id>::generateRightConvolution();

    template <uint16_t Size, bool Inverse>
    template <std::ranges::random_access_range Range>
    void FFT::CooleyTukeyBase<Size, Inverse>::operator() (Range&& range) const noexcept {
        for (uint8_t i = 0;i != Size / 2; ++i)
            std::swap(range[reverseBits(i) >> (CHAR_BIT - log2N())], range[i]);
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (innerLoop<I>(std::forward<Range>(range)), ...);
        } (std::make_index_sequence<log2N()>{});
    }

    template <uint16_t Size, bool Inverse>
    template <std::size_t Index, std::ranges::random_access_range Range>
    void FFT::CooleyTukeyBase<Size, Inverse>::innerLoop(Range&& range) const noexcept {
        uint16_t m = 2 << Index, sm = 1 << Index;
        for (uint8_t k = 0;k != Size; k += m) {
            for (uint8_t i = k; i < k + sm; ++i) {
                auto t = range[i + sm] * CooleyTukeyBasePart<Index, Inverse>::lookupTable[i - k];
                range[i + sm] = range[i] - t;
                range[i] += t;
            }
        }
    }

    template <uint8_t Size>
    constexpr uint16_t FFT::BluesteinBase<Size>::convoluteLen(void) noexcept {
        uint16_t n = 2 * Size + 1, m = 0;
        for (;1 << m < n; m += 1);
        return 1 << m;
    }

    template <uint8_t Size>
    FFT::BluesteinBase<Size>::ConvolutionArray
        FFT::BluesteinBase<Size>::generateRightConvolution(void) noexcept
    {
        ConvolutionArray sequence;
        sequence[0] = 1.;
        for (uint16_t i = 1; i < Size; ++i)
            sequence[i] = sequence[sequence.size() - i] = sequence[i - 1] *
                std::polar(1., (2 * i - 1) * std::numbers::pi / Size);
        cooleyTukey(sequence);
        return sequence;
    }

    template <uint8_t Size>
    FFT::BluesteinBase<Size>::Table
        FFT::BluesteinBase<Size>::generateLookupTable(void) noexcept
    {
        Table table{};
        table[0] = 1.;
        for (uint8_t i = 1; i < Size; ++i)
            table[i] = table[i - 1] * std::polar(1.,
                -(2 * i - 1) * std::numbers::pi / Size);
        return table;
    }

    template <uint8_t Size>
    template <std::ranges::random_access_range Range>
    FFT::BluesteinBase<Size>::ConvolutionArray
        FFT::BluesteinBase<Size>::generateLeftConvolution(Range&& range) const noexcept
    {
        ConvolutionArray sequence{};
        for (uint8_t i = 0;i < range.size(); ++i)
            sequence[i] = range[i] * lookupTable[i];
        return sequence;
    }

    template <uint8_t Size>
    template <std::ranges::random_access_range Range>
    void FFT::BluesteinBase<Size>::operator() (Range&& range) const noexcept {
        auto leftConvolution = generateLeftConvolution(std::forward<Range>(range));
        CooleyTukeyBase<convoluteLen()>{}(leftConvolution);
        std::ranges::transform(leftConvolution, rightConvolution,
            leftConvolution.begin(), std::multiplies{});
        CooleyTukeyBase<convoluteLen(), true>{}(leftConvolution);
        std::ranges::for_each(leftConvolution, [this](auto& element) {
            element /= convoluteLen(); });
        for (uint16_t i = 0;i < Size; ++i)
            range[i] = leftConvolution[i] * lookupTable[i];
    }

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
    void FFT::operator() (Range&& range) const noexcept {
        if constexpr (SameSignatures<std::size_t(*)(void) noexcept,
            &std::remove_cvref_t<Range>::size>)
        {
            if constexpr (IsConstexprEvaluableV<std::size_t(*)(void) noexcept,
                &std::remove_cvref_t<Range>::size>) // constexpr constructible and executible
            {
                ConstexprSizeFFT<std::remove_reference_t<Range>::size()>{}
                    (std::forward<Range>(range));
            } else
                randomRangeFFT(std::forward<Range>(range));
        } else
            randomRangeFFT(std::forward<Range>(range));
    }

    template <std::ranges::random_access_range Range>
    void FFT::cooleyTukey(Range&& range, double sign) noexcept {
        auto log = log2N(range.size());
        for (uint8_t i = 0; i < range.size() / 2; ++i)
            std::swap(range[reverseBits(i) >> (CHAR_BIT - log)], range[i]);
        for (uint8_t i = 1, m = 2, sm = 1; i <= log; ++i, sm = m, m <<= 1) {
            auto omegaM = std::polar(1., 2. * sign * std::numbers::pi / m);
            for (uint8_t k = 0; k < range.size(); k += m) {
                std::complex<double> omega = 1.;
                for (uint8_t j = k; j < (k + sm); ++j, omega *= omegaM) {
                    auto t = omega * range[j + sm];
                    range[j + sm] = range[j] - t;
                    range[j] += t;
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
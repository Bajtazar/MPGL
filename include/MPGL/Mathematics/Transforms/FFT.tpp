/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include <numbers>

namespace mpgl {

    template <FFT::size_type Id, bool Inverse>
    FFT::CooleyTukeyBasePart<Id, Inverse>::Table
        FFT::CooleyTukeyBasePart<Id, Inverse>::generateLookupTable(
            void) noexcept
    {
        Table table;
        size_type size = table.size();
        float64 constant = (Inverse ? 1. : -1.)
            * std::numbers::pi / size;
        for (size_type i = 0;i != size; ++i)
            table[i] = std::polar(1., constant * i);
        return table;
    }

    template <FFT::size_type Id, bool Inverse>
    FFT::CooleyTukeyBasePart<Id, Inverse>::Table const
        FFT::CooleyTukeyBasePart<Id, Inverse>::lookupTable
            = generateLookupTable();

    template <FFT::size_type Id>
    FFT::BluesteinBase<Id>::Table const
        FFT::BluesteinBase<Id>::lookupTable =
            FFT::BluesteinBase<Id>::generateLookupTable();

    template <FFT::size_type Id>
    FFT::BluesteinBase<Id>::ConvolutionArray const
        FFT::BluesteinBase<Id>::rightConvolution =
            FFT::BluesteinBase<Id>::generateRightConvolution();

    template <FFT::size_type Size, bool Inverse>
    template <std::ranges::random_access_range Range>
    void FFT::CooleyTukeyBase<Size, Inverse>::operator() (
        Range&& range) const noexcept
    {
        for (size_type i = 0;i != Size / 2; ++i)
            std::swap(range[reverseBits(i) >>
                (sizeof(size_type) * CHAR_BIT - log2N())], range[i]);
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (innerLoop<I>(std::forward<Range>(range)), ...);
        } (std::make_index_sequence<log2N()>{});
    }

    template <FFT::size_type Size, bool Inverse>
    template <std::size_t Index, std::ranges::random_access_range Range>
    void FFT::CooleyTukeyBase<Size, Inverse>::innerLoop(
        Range&& range) const noexcept
    {
        size_type m = 2 << Index, sm = 1 << Index;
        for (size_type k = 0;k != Size; k += m) {
            for (size_type i = k; i < k + sm; ++i) {
                auto t = cast(range[i + sm],
                    CooleyTukeyBasePart<Index,
                        Inverse>::lookupTable[i - k]);
                range[i + sm] = range[i] - t;
                range[i] += t;
            }
        }
    }

    template <FFT::size_type Size, bool Inverse>
    template <typename Tp>
    inline Tp FFT::CooleyTukeyBase<Size, Inverse>::cast(
        Tp const& value, Complex const& lookup) const noexcept
    {
        if constexpr (std::same_as<Complex, Tp>)
            return value * lookup;
        else
            return Tp{value.real() * lookup.real(),
                value.imag() * lookup.imag()};
    }

    template <FFT::size_type Size>
    constexpr FFT::size_type FFT::BluesteinBase<Size>::convoluteLen(
        void) noexcept
    {
        size_type n = 2u * Size + 1u, m = 0u;
        for (;1u << m < n; m += 1u);
        return 1u << m;
    }

    template <FFT::size_type Size>
    FFT::BluesteinBase<Size>::ConvolutionArray
        FFT::BluesteinBase<Size>::generateRightConvolution(
            void) noexcept
    {
        ConvolutionArray sequence;
        sequence[0] = 1.;
        for (size_type i = 1; i < Size; ++i)
            sequence[i] = sequence[sequence.size() - i]
                = sequence[i - 1] * std::polar(1., (2 * i - 1)
                    * std::numbers::pi / Size);
        cooleyTukey(sequence);
        return sequence;
    }

    template <FFT::size_type Size>
    FFT::BluesteinBase<Size>::Table
        FFT::BluesteinBase<Size>::generateLookupTable(void) noexcept
    {
        Table table{};
        table[0] = 1.;
        for (size_type i = 1; i < Size; ++i)
            table[i] = table[i - 1] * std::polar(1.,
                -(2 * i - 1) * std::numbers::pi / Size);
        return table;
    }

    template <FFT::size_type Size>
    template <std::ranges::random_access_range Range>
    FFT::BluesteinBase<Size>::ConvolutionArray
        FFT::BluesteinBase<Size>::generateLeftConvolution(
            Range&& range) const noexcept
    {
        ConvolutionArray sequence{};
        for (size_type i = 0;i < range.size(); ++i)
            sequence[i] = range[i] * lookupTable[i];
        return sequence;
    }

    template <FFT::size_type Size>
    template <std::ranges::random_access_range Range>
    void FFT::BluesteinBase<Size>::operator() (
        Range&& range) const noexcept
    {
        auto leftConvolution = generateLeftConvolution(
            std::forward<Range>(range));
        CooleyTukeyBase<convoluteLen()>{}(leftConvolution);
        std::ranges::transform(leftConvolution, rightConvolution,
            leftConvolution.begin(), std::multiplies{});
        CooleyTukeyBase<convoluteLen(), true>{}(leftConvolution);
        std::ranges::for_each(leftConvolution, [this](auto& element) {
            element /= convoluteLen(); });
        for (size_type i = 0;i < Size; ++i)
            range[i] = leftConvolution[i] * lookupTable[i];
    }

    template <std::ranges::random_access_range Range>
    void FFT::randomRangeFFT(Range&& range) noexcept {
        size_type size = range.size();
        if ((size & (size - 1)) && size)
            bluestein(std::forward<Range>(range));
        else
            cooleyTukey(std::forward<Range>(range));
    }

    template <std::ranges::random_access_range Range>
        requires (std::ranges::sized_range<Range>
            && SpecializationOf<std::ranges::range_value_t<Range>,
                std::complex>)
    void FFT::operator() (Range&& range) const noexcept {
        if constexpr (FixedSizeRange<Range>) {
            ConstexprSizeFFT<std::remove_reference_t<Range>::size()>{
            }(std::forward<Range>(range));
        } else
            randomRangeFFT(std::forward<Range>(range));
    }

    template <std::ranges::random_access_range Range>
    void FFT::cooleyTukey(Range&& range, float64 sign) noexcept {
        auto log = fastLog2(range.size());
        for (size_type i = 0; i < range.size() / 2; ++i)
            std::swap(range[reverseBits(i) >>
                (sizeof(size_type) * CHAR_BIT - log)], range[i]);
        for (size_type i = 1, m = 2, sm = 1; i <= log; ++i, sm = m, m <<= 1) {
            auto omegaM = std::polar(1., 2. * sign * std::numbers::pi / m);
            for (size_type k = 0; k < range.size(); k += m) {
                std::complex<float64> omega = 1.;
                for (size_type j = k; j < (k + sm); ++j, omega *= omegaM) {
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
        ComplexVector convoluted = convolve(generateLeftSequence(
            range, lookupTable, convolution),
                generateRightSequence(lookupTable, convolution));
        for (size_type i = 0; i < range.size(); ++i)
            range[i] = convoluted[i] * std::conj(lookupTable[i]);
    }

    template <std::ranges::random_access_range Range>
    FFT::ComplexVector FFT::generateLeftSequence(
        Range&& range,
        ComplexVector const& lookupTable,
        size_type size) noexcept
    {
        ComplexVector leftSequence;
        leftSequence.reserve(size);
        for (size_type i = 0; i < range.size(); ++i)
            leftSequence.push_back(
                range[i] * std::conj(lookupTable[i]));
        leftSequence.resize(size);
        return leftSequence;
    }

}

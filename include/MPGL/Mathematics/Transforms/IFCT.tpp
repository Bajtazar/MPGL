/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Mathematics/Tensors/Views.hpp>

namespace mpgl {

    template <IFCT::size_type Size>
    IFCT::FixedSizeKernel<Size>::ComplexArray const
        IFCT::FixedSizeKernel<Size>::lookupTable
            = generateLookupTable();

    template <std::ranges::random_access_range Range>
    IFCT::ComplexVector IFCT::DefaultKernel::operator() (
        Range&& range) const
    {
        ComplexVector complexVec;
        complexVec.reserve(range.size());
        complexVec.emplace_back(std::sqrt(range[0] / range.size()), 0);
        for (size_type i = 1; i < range.size(); ++i)
            complexVec.push_back(std::polar(
                std::sqrt(2. * range[i] / range.size()),
                -i * std::numbers::pi / (2. * range.size())));
        return complexVec;
    }

    template <std::ranges::random_access_range Range>
    void IFCT::postprocessing(
        Range&& range,
        ComplexVector const& vector) const
    {
        for (size_type i = 0; i < vector.size() / 2; ++i) {
            range[2 * i] = vector[i].real();
            range[2 * i + 1] = vector[vector.size() - i - 1].real();
        }
    }

    template <IFCT::size_type Size>
    IFCT::FixedSizeKernel<Size>::ComplexArray
        IFCT::FixedSizeKernel<Size>::generateLookupTable(void)
    {
        ComplexArray lookup;
        lookup[0] = Complex{std::sqrt(1. / Size), 0.};
          for (int64 i = 1; i != Size; ++i)
            lookup[i] = std::polar(std::sqrt(2. / Size),
                -i * std::numbers::pi / (2. * Size));
        return lookup;
    }

    template <IFCT::size_type Size>
    template <std::ranges::random_access_range Range>
    IFCT::ComplexVector
        IFCT::FixedSizeKernel<Size>::operator() (
            Range&& range) const
    {
        ComplexVector complexVec;
        complexVec.reserve(range.size());
        auto iter = range.begin();
        for (auto const& coeff : lookupTable)
            complexVec.push_back(coeff * Complex{(double)*iter++, 0.});
        return complexVec;
    }

    template <Arithmetic Tp, IFCT::size_type Rows>
    void IFCT::operator() (
        Matrix<Tp, Rows, Rows>& matrix) const
    {
        for (auto column : matrix | views::columns)
            (*this)(column);
        for (auto& row : matrix)
            (*this)(row);
    }

    template <std::ranges::random_access_range Range>
    IFCT::ComplexVector IFCT::FixedKernel::operator() (
        Range&& range) const
    {
        return FixedSizeKernel<std::remove_cvref_t<Range>::size()>{}(
            std::forward<Range>(range));
    }

    template <std::ranges::random_access_range Range>
        requires (std::ranges::sized_range<Range>
            && Arithmetic<std::ranges::range_value_t<Range>>)
    void IFCT::operator() (Range&& range) const {
        auto complexVec = preprocessing(std::forward<Range>(range));
        fft(complexVec);
        postprocessing(std::forward<Range>(range), complexVec);
    }

    template <std::ranges::random_access_range Range>
    IFCT::ComplexVector IFCT::preprocessing(Range&& range) const {
        if constexpr (FixedSizeRange<Range>)
            return fixedKernel(std::forward<Range>(range));
        else
            return defaultKernel(std::forward<Range>(range));
    }

}

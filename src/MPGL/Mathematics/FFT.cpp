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
#include <MPGL/Mathematics/FFT.hpp>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace mpgl {

    #ifdef __GNUC__
    FFT::size_type FFT::convolutionSize(size_type number) noexcept {
        uint size = 2 * number + 1;
        // __builtin_clz cannot overflow - it is well defined
        return !(size & (size - 1)) ? size :
            1 << (sizeof(long) * CHAR_BIT - __builtin_clz(size));
    }
    #elif defined(_MSC_VER)
    FFT::size_type FFT::convolutionSize(size_type number) noexcept {
        uint size = 2 * number + 1;
        if (!(size & (size - 1)))
            return size;
        DWORD leading = 0;
        _BitScanReverse(&leading, size);
        return 1 << (sizeof(long) * CHAR_BIT - leading);
    }
    #endif

    FFT::ComplexVector
        FFT::generateLookupTable(size_type size) noexcept
    {
        ComplexVector lookupTable;
        lookupTable.reserve(size);
        lookupTable.emplace_back(1.);
        for (size_type i = 1; i < size; ++i)
            lookupTable.push_back(std::polar(1.,
                (2 * i - 1) * std::numbers::pi / size)
                    * lookupTable.back());
        return lookupTable;
    }

    FFT::ComplexVector FFT::generateRightSequence(
        ComplexVector const& lookupTable,
        size_type size) noexcept
    {
        ComplexVector rightSequence;
        rightSequence.reserve(size);
        std::ranges::copy(lookupTable,
            std::back_inserter(rightSequence));
        rightSequence.resize(size - lookupTable.size() + 1);
        rightSequence.reserve(size);
        std::ranges::copy(std::views::reverse(lookupTable
            | std::views::drop(1)), std::back_inserter(rightSequence));
        return rightSequence;
    }

    FFT::ComplexVector FFT::convolve(ComplexVector leftSequence,
        ComplexVector rightSequence) noexcept
    {
        cooleyTukey(leftSequence);
        cooleyTukey(rightSequence);
        std::ranges::transform(leftSequence, rightSequence,
            leftSequence.begin(), std::multiplies{});
        cooleyTukey(leftSequence, 1.); // inversed tranform
        std::ranges::for_each(leftSequence,
            [&leftSequence](auto& element)
                { element /= leftSequence.size(); });
        return leftSequence;
    }

}

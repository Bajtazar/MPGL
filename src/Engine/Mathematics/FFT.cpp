#include "FFT.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace mpgl {

    #ifdef __GNUC__
    uint16 FFT::convolutionSize(uint8 number) noexcept {
        uint size = 2 * number + 1;
        // __builtin_clz cannot overflow - it is well defined
        return !(size & (size - 1)) ? size :
            1 << (sizeof(long) * CHAR_BIT - __builtin_clz(size));
    }
    #elif defined(_MSC_VER)
    uint16 FFT::convolutionSize(uint8 number) noexcept {
        uint size = 2 * number + 1;
        if (!(size & (size - 1)))
            return size;
        DWORD leading = 0;
        _BitScanReverse(&leading, size);
        return 1 << (sizeof(long) * CHAR_BIT - leading);
    }
    #endif

    FFT::ComplexVector FFT::generateLookupTable(uint16 size) noexcept {
        ComplexVector lookupTable;
        lookupTable.reserve(size);
        lookupTable.emplace_back(1.);
        for (uint16 i = 1; i < size; ++i)
            lookupTable.push_back(std::polar(1., (2 * i - 1) * std::numbers::pi / size)
                * lookupTable.back());
        return lookupTable;
    }

    FFT::ComplexVector FFT::generateRightSequence(ComplexVector const& lookupTable,
        uint16 size) noexcept
    {
        ComplexVector rightSequence;
        rightSequence.reserve(size);
        std::ranges::copy(lookupTable, std::back_inserter(rightSequence));
        rightSequence.resize(size - lookupTable.size() + 1);
        rightSequence.reserve(size);
        std::ranges::copy(std::views::reverse(lookupTable | std::views::drop(1)),
            std::back_inserter(rightSequence));
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
        std::ranges::for_each(leftSequence, [&leftSequence](auto& element)
            { element /= leftSequence.size(); });
        return leftSequence;
    }

}

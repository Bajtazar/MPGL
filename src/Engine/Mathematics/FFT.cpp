#include "FFT.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace ge {

    #ifdef __GNUC__
    uint8_t FFT::log2N(uint8_t number) noexcept {
        return number & 0x01 ? 0 :
            static_cast<uint8_t>(__builtin_ctz(number));
    }
    #elif defined(_MSC_VER)
    uint8_t FFT::log2N(uint8_t number) noexcept {
        DWORD trailing = 0;
        return _BitScanForward(&trailing, number) ? trailing : 0;
    }
    #endif

    #ifdef __GNUC__
    uint16_t FFT::convolutionSize(uint8_t number) noexcept {
        uint size = 2 * number + 1;
        // __builtin_clz cannot overflow - it is well defined
        return !(size & (size - 1)) ? size :
            1 << (sizeof(long) * CHAR_BIT - __builtin_clz(size));
    }
    #elif defined(_MSC_VER)
    uint16_t FFT::convolutionSize(uint8_t number) noexcept {
        uint size = 2 * number + 1;
        if (!(size & (size - 1)))
            return size;
        DWORD leading = 0;
        _BitScanReverse(&leading, size);
        return 1 << (sizeof(long) * CHAR_BIT - leading);
    }
    #endif

    FFT::ComplexVector FFT::generateLookupTable(uint16_t size) noexcept {
        ComplexVector lookupTable;
        lookupTable.reserve(size);
        for (uint16_t i = 0; i < size; ++i) {
            lookupTable.push_back(std::polar(1.,
                ((uint64_t) (i * i) % (2 * size)) * std::numbers::pi / size));
        }
        return lookupTable;
    }

    FFT::ComplexVector FFT::generateRightSequence(ComplexVector const& lookupTable,
        uint16_t size) noexcept
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
        cooleyTukey(leftSequence, 2.); // works like with negative omega
        std::ranges::for_each(leftSequence, [&leftSequence](auto& element)
            { element /= leftSequence.size(); });
        return leftSequence;
    }

}

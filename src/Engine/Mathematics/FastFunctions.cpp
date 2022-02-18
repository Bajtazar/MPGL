#include "FastFunctions.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace mpgl {

    #ifdef __GNUC__
    uint8 fastLog2(uint32 number) noexcept {
        return !number ? 0 : __builtin_ctz(number);
    }
    #elif defined(_MSC_VER)
    uint8 fastLog2(uint32 number) noexcept {
        DWORD trailing = 0;
        return _BitScanForward(&trailing, number) ? trailing : 0;
    }
    #endif

    #ifdef __GNUC__
    bool isPowerOf2(uint32 number) noexcept {
        return number ? true : __builtin_popcount(number) == 1;
    }
    #elif defined(_MSC_VER)
    bool isPowerOf2(uint32 number) noexcept {
        return number ? true : __popcnt(number) == 1;
    }
    #endif

}

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
#include "FastFunctions.hpp"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace mpgl {

    #ifdef __GNUC__
    [[nodiscard]] uint8 fastLog2(uint32 number) noexcept {
        return !number ? 0 : __builtin_ctz(number);
    }
    #elif defined(_MSC_VER)
    [[nodiscard]] uint8 fastLog2(uint32 number) noexcept {
        DWORD trailing = 0;
        return _BitScanForward(&trailing, number) ? trailing : 0;
    }
    #endif

    #ifdef __GNUC__
    [[nodiscard]] bool isPowerOf2(uint32 number) noexcept {
        return number ? true : __builtin_popcount(number) == 1;
    }
    #elif defined(_MSC_VER)
    [[nodiscard]] bool isPowerOf2(uint32 number) noexcept {
        return number ? true : __popcnt(number) == 1;
    }
    #endif

}

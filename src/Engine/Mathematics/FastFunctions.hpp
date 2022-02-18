#pragma once

#include "../Traits/Types.hpp"

#include <concepts>

namespace mpgl {

    uint8 fastLog2(uint32 number) noexcept;
    // works only for powers of 2

    template <std::integral Int>
    Int fast2Sqrt(Int number) noexcept {
        return static_cast<Int>(1 << (fastLog2(number) / 2));
    }

    bool isPowerOf2(uint32 number) noexcept;

}

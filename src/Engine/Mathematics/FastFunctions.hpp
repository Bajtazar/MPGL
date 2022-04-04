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

#include "../Traits/Types.hpp"

#include <concepts>

namespace mpgl {

    /**
     * Calculates the binary logarithm of the given number
     * by using the single assembler instruction. Works
     * only for the powers of two
     *
     * @param number the logarithmed number
     * @return the log2 value
     */
    [[nodiscard]] uint8 fastLog2(uint32 number) noexcept;

    /**
     * Calculates the square root of the given number.
     * Works only for the powers of two
     *
     * @tparam Int the integer type
     * @param number the number that is the power of two
     * @return the square root of the number
     */
    template <std::integral Int>
    [[nodiscard]] Int fast2Sqrt(Int number) noexcept;

    /**
     * Checks whether the given number is the power of two
     *
     * @param number the checked number
     * @return if the given number is the power of two
     */
    [[nodiscard]] bool isPowerOf2(uint32 number) noexcept;

    template <std::integral Int>
    [[nodiscard]] Int fast2Sqrt(Int number) noexcept {
        return static_cast<Int>(1 << (fastLog2(number) / 2));
    }

}

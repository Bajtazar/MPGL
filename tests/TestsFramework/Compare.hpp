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

#include <algorithm>
#include <concepts>

namespace mpgl::tests {

    /**
     * Compares two values with each other
     *
     * @tparam Tp the value type
     * @param t1 the universal reference to the first value
     * @param t2 the universal reference to the second value
     * @returns if the values are equal
     */
    template <typename Tp>
    constexpr bool compare(Tp&& t1, Tp&& t2) noexcept
        requires requires (Tp t1)
            { { t1 == t2 } -> std::same_as<bool>; }
    {
        return t1 == t2;
    }

    /**
     * Compares two ranges with each other
     *
     * @tparam LeftIter the left iterator type
     * @tparam LeftSent the left sentinel type
     * @tparam RightIter the right iterator type
     * @tparam RightSent the right sentinel type
     * @param leftIter the left iterator object
     * @param leftSent the left sentinel object
     * @param rightIter the right iteator object
     * @param rightSent the right sentinel object
     * @return if the given ranges are equal
     */
    template <std::input_iterator LeftIter,
        std::sentinel_for<LeftIter> LeftSent,
        std::input_iterator RightIter,
        std::sentinel_for<RightIter> RightSent>
    constexpr bool compare(
        LeftIter leftIter,
        LeftSent leftSent,
        RightIter rightIter,
        RightSent rightSent) noexcept
    {
        return std::ranges::equal(leftIter, leftSent,
            rightIter, rightSent);
    }

    /**
     * Compares two ranges with each other
     *
     * @tparam Tp the left range type
     * @tparam Up the right range type
     * @param t1 the universal reference to the first range
     * @param t2 the universal reference to the second range
     * @return if the given ranges are equal
     */
    template <std::ranges::input_range Tp, std::ranges::input_range Up>
    constexpr bool compare(Tp&& t1, Up&& t2) noexcept {
        return std::ranges::equal(std::forward<Tp>(t1),
            std::forward<Up>(t2));
    }

}

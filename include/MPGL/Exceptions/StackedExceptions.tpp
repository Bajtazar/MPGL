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

namespace mpgl {

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
            requires SameIterType<Iter, std::exception_ptr>
    StackedExceptions::StackedExceptions(Iter iter, Sent const& sent)
        : exceptions{iter, sent},
        message{createMessageFromExceptions(iter, sent)} {}

    template <std::ranges::input_range Range>
        requires SameRangeType<Range, std::exception_ptr>
    StackedExceptions::StackedExceptions(Range&& range)
        : StackedExceptions{std::ranges::begin(range),
            std::ranges::end(range)} {}

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    std::string StackedExceptions::createMessageFromExceptions(
        Iter const& iter,
        Sent const& sent)
    {
        return "There is " + std::to_string(
            std::distance(iter, sent)) + " gathered exceptions\n";
    }

}

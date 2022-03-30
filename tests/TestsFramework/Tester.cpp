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
#include "Tester.hpp"

#include <algorithm>
#include <ranges>

namespace mpgl::tests {

    thread_local Tester::TestVector Tester::testVector{};

    [[nodiscard]] Tester::TestResult
        Tester::testFunction(TestPair const& pair) noexcept
    {
        auto const& [name, function] = pair;
        AssertVector results;
        function(results);
        auto positive = std::ranges::count(results, true);
        return {name, positive, results.size()};
    }

    [[nodiscard]] Tester::ResultVector
        Tester::testFunctions(void) noexcept
    {
        auto view = std::views::transform(testVector,
            [](auto const& pair) { return testFunction(pair); });
        return ResultVector{view.begin(), view.end()};
    }

}

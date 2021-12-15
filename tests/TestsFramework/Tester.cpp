#include "Tester.hpp"

#include <algorithm>
#include <ranges>

namespace ge::tests {

    thread_local Tester::TestVector Tester::testVector{};

    Tester::TestResult
        Tester::testFunction(TestPair const& pair) noexcept
    {
        auto const& [name, function] = pair;
        AssertVector results;
        function(results);
        auto positive = std::ranges::count(results, true);
        return {name, positive, results.size()};
    }

    Tester::ResultVector
        Tester::testFunctions(void) noexcept
    {
        auto view = std::views::transform(testVector,
            [](auto const& pair) { return testFunction(pair); });
        return ResultVector{view.begin(), view.end()};
    }

}

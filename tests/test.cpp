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
#include "TestsFramework/Tests.hpp"

#include <iostream>
#include <vector>
#include <array>

Test(Test1) {
    std::vector<int> v1{1, 2, 3};
    std::array<int, 3> a1{1, 2, 3};
    Equal(v1, a1)
    Equal(true, true)
    Assert(true)
}

Test(Test2) {
    std::vector<int> v1{1, 2, 3};
    std::array<int, 3> a1{1, 2, 3};
    Equal(v1, a1)
    Equal(1., 1.)
    Assert(false)
}

Test(Test3) {
    std::vector<int> v1{1, 2, 3};
    std::array<int, 3> a1{1, 2, 3};
    Equal(v1, a1)
    Equal(1, 2)
}

Test(Test4) {
    std::vector<int> v1{1, 2, 3};
    std::array<int, 3> a1{1, 2, 3};
    Equal(v1, a1)
    Equal(1, 2)
}

Test(Test5) {
    std::vector<int> v1{1, 2, 3};
    std::array<int, 3> a1{1, 2, 3};
    Equal(v1, a1)
    Equal(1, 2)
}

int main(void) noexcept {
    auto results = mpgl::tests::Tester::testFunctions();
    for (auto const& [name, positives, all] : results)
        std::cout << name << "\tPositive: " << positives
            << "\tNegatives: " << (all - positives) << '\n';
    return 0;
}

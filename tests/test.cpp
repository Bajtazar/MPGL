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
    mpgl::tests::TestsWindow window;
    return window.run();
}

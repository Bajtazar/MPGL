#pragma once

#include "ResultWindow.hpp"

namespace ge::tests {

    class TestsWindow {
    public:
        explicit TestsWindow(void) noexcept;

        int run(void) const noexcept;
    private:
        typedef typename Tester::ResultVector   Results;

        Results                                 results;
    };

}

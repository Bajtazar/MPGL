#include "TestsWindow.hpp"

namespace ge::tests {

    TestsWindow::TestsWindow(void) noexcept
        : results{Tester::testFunctions()} {}

    int TestsWindow::run(void) const noexcept {
        Window window{640_x + 480_y, "Tests results"};
        window.setFPSLimit(140);

        Font font{"Hack", "/usr/share/fonts/truetype/hack/"};

        window.emplaceDrawable<ResultWindow>(font, results);

        return window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
    }

}

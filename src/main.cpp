#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

#include <iostream>

using namespace ge;

int main(void) noexcept {
    ge::RenderWindow window{640_x + 480_y, "Hello World!"};

    window.emplaceDrawable<Circle>(400_x + 200_y, 100.f, ge::Color::literals::Yellow);

    Circle& circle = dynamic_cast<Circle&>(*window[0]);
    auto iter = 0.;
    for (auto& color : circle | std::views::drop(1) | ge::views::color) {
        color = ge::Color(std::sin(iter), std::cos(iter), -std::sin(iter), 0.f);
        iter += 2 * std::numbers::pi_v<double> / (circle.size() - 1);
    }

    window.emplaceDrawable<DrawableArray<Triangle<true>>>();
    auto& array = dynamic_cast<DrawableArray<Triangle<true>>&>(*window[1]);
    array.emplaceBack(400_x + 400_y, 300_x + 300_y, 400_x + 300_y, ge::Color::literals::White);
    array.emplaceBack(200_x + 200_y, 200_x + 300_y, 300_x + 200_y, ge::Color::literals::Green);

    return window.windowLoop();
}
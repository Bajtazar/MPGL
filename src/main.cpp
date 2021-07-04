#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>
using namespace ge;

int main(void) noexcept {
    ge::RenderWindow window{640_x + 480_y, "Hello World!"};

    auto rectangle = std::make_unique<Rectangle>(window.getWindowBaseDimmensions(), 200_x + 200_y, 100_x + 100_y, ge::Color::literals::Green);
    Rectangle& component = *rectangle;
    component[0].color = ge::Color::literals::Red;
    component[1].color = ge::Color::literals::Green;
    component[2].color = ge::Color::literals::Blue;
    component[3].color = ge::Color::literals::Yellow;

    window.addDrawable(std::move(rectangle));
    window.emplaceDrawable<PolygonTriangle>(200_x + 200_y, 300_x + 200_y, 200_x + 300_y, ge::Color::literals::Blue);
    window.emplaceDrawable<Circle>(400_x + 200_y, 100.f, ge::Color::literals::Yellow);

    Circle& circle = dynamic_cast<Circle&>(*window[2]);
    auto iter = 0.;
    for (auto& color : circle | std::views::drop(1) | ge::views::color) {
        color = ge::Color(std::sin(iter), std::cos(iter), -std::sin(iter), 0.f);
        iter += 2 * std::numbers::pi_v<double> / (circle.size() - 1);
    }

    window.emplaceDrawable<Line>(100_x + 100_y, 200_x + 100_y, ge::Color::literals::Green);
    window.emplaceDrawable<LineStrip>(4, ge::Color::literals::White);
    LineStrip& lineStrip = dynamic_cast<LineStrip&>(*window[4]);
    lineStrip[0].position = 40_x + 40_y;
    lineStrip[1].position = 50_x + 50_y;
    lineStrip[2].position = 60_x + 40_y;
    lineStrip[3].position = 50_x + 30_y;

    return window.windowLoop();
}
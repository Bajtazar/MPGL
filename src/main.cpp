#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

#include <iostream>

#include "Engine/Core/Text/TTFLoader.hpp"

using namespace ge;

int main(void) noexcept {
    RenderWindow window{640_x + 480_y, "Hello World!"};
    window.setFPSLimit(140);

    window.emplaceDrawable<Circle>(400_x + 200_y, 100.f, ge::Color::literals::Yellow);

    Circle& circle = dynamic_cast<Circle&>(*window[0]);
    auto iter = 0.;
    for (auto& color : circle | std::views::drop(1) | ge::views::color) {
        color = ge::Color(std::sin(iter), std::cos(iter), -std::sin(iter), 0.f);
        iter += 2 * std::numbers::pi_v<double> / (circle.size() - 1);
    }

    TextureLoader loader{execution::parallel, "textures"};
    loader.loadAll();
    auto pack = loader.getTextures();

    TTFLoader<> font{"/usr/share/fonts/truetype/hack/Hack-Regular.ttf"};
    auto glyph = font.getGlyph('y');
    std::cout << glyph.exist() << ' ' << glyph.isSimple() << '\n';
    auto xChar = glyph.getSimpleGlyph();
    std::cout << "Size: " << xChar.points.size() << '\n';

    for (auto const& point : xChar.points)
       std::cout << point.position[0] << ' ' << point.position[1] << '\n';

    window.emplaceDrawable<LineStrip>(xChar.points.size() + 1);
    auto& points = dynamic_cast<LineStrip&>(*window[1]);
    for (uint32_t i = 0;i < xChar.points.size(); ++i)
        points[i].position = static_cast<Vector2f>(xChar.points[i].position) / 10.f + Vector2f{20.f, 300.f};
    points.back().position = points.front().position;
    for (auto& color : points | views::color)
        color = Color::literals::White;

    window.emplaceDrawable<DrawableArray<Triangle<true>>>();
    auto& array = dynamic_cast<DrawableArray<Triangle<true>>&>(*window[2]);
    array.emplaceBack(400_x + 400_y, 300_x + 300_y, 400_x + 300_y, Color::literals::White);
    array.emplaceBack(200_x + 200_y, 200_x + 300_y, 300_x + 200_y, Color::literals::Green);

    window.emplaceDrawable<DefaultSprite>(pack["pad.jpeg"], 100_x + 100_y, 100_x + 100_y);
    auto& sprite = dynamic_cast<DefaultSprite&>(*window[3]);
    sprite.rotate(150_x + 150_y, std::numbers::pi_v<double> / 4);

    return window.windowLoop();
}

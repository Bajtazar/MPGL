#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

using namespace ge;

int main(void) noexcept {
    Window window{640_x + 480_y, "Hello World!"};
    window.setFPSLimit(140);

    window.emplaceDrawable<Polygon>(400_x + 200_y, 100.f, 10, ge::Color::literals::Yellow);

    Polygon& circle = dynamic_cast<Polygon&>(*window[0]);
    auto iter = 0.;
    for (auto& color : circle | std::views::drop(1) | ge::views::color) {
        color = ge::Color(std::sin(iter), std::cos(iter), -std::sin(iter), 0.f);
        iter += 2 * std::numbers::pi_v<double> / (circle.size() - 1);
    }

    TextureLoader loader{execution::parallel, "textures"};
    loader.loadAll();
    auto pack = loader.getTextures();

    Font font{"Hack", "/usr/share/fonts/truetype/hack/"};

    window.emplaceDrawable<Text<>>(font, 18, 20_x + 300_y, "Witaj świecie!");

    window.emplaceDrawable<DrawableArray<Triangle<true>>>();
    auto& array = dynamic_cast<DrawableArray<Triangle<true>>&>(*window[2]);
    array.emplaceBack(400_x + 400_y, 300_x + 300_y, 400_x + 300_y, Color::literals::White);
    array.emplaceBack(200_x + 200_y, 200_x + 300_y, 300_x + 200_y, Color::literals::Green);

    window.emplaceDrawable<DefaultSprite>(pack["pad.jpeg"], 100_x + 100_y, 100_x + 100_y);
    auto& sprite = dynamic_cast<DefaultSprite&>(*window[3]);
    sprite.rotate(150_x + 150_y, std::numbers::pi_v<double> / 4);

    window.emplaceDrawable<Ellipse>(200_x + 400_y, 40.f, Color::literals::Red);
    window.emplaceDrawable<Ellipse>(200_x + 200_y, Vector2f{80.f, 100.f}, 20.f, Color::literals::Red);

    return window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
}

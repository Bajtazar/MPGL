#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

using namespace ge;

int main(void) noexcept {
    Window window{640_x + 480_y, "Hello World!"};
    window.setFPSLimit(140);

    window.emplaceDrawable<Polygon>(400_x + 200_y, 100.f, 10, Color::Yellow);

    Polygon& circle = dynamic_cast<Polygon&>(*window[0]);
    auto iter = 0.;
    for (auto& color : circle | std::views::drop(1) | views::color) {
        color = Color(std::sin(iter), std::cos(iter), -std::sin(iter), 0.f);
        iter += 2 * std::numbers::pi_v<float64> / (circle.size() - 1);
    }

    TextureLoader loader{execution::parallel, "textures"};
    loader.loadAll();
    auto pack = loader.getTextures();

    Font font{"Hack", "/usr/share/fonts/truetype/hack/"};

    window.emplaceDrawable<Text<>>(font, 18, 20_x + 300_y, "Witaj świecie!");

    window.emplaceDrawable<DrawableCollection<LineStrip>>();
    auto& array = dynamic_cast<DrawableCollection<LineStrip>&>(*window[2]);
    array.emplace_back(Color::White, 400_x + 400_y, 300_x + 300_y, 400_x + 300_y);
    array.emplace_back(Color::Green, 200_x + 200_y, 200_x + 300_y, 300_x + 200_y);

    window.emplaceDrawable<DefaultSprite>(pack["pad.jpeg"], 100_x + 100_y, 100_x + 100_y);
    auto& sprite = dynamic_cast<DefaultSprite&>(*window[3]);
    sprite.rotate(150_x + 150_y, std::numbers::pi_v<float64> / 4);

    window.emplaceDrawable<Ellipse>(200_x + 400_y, 40_x + 20_y, Color::Red);

    return window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
}

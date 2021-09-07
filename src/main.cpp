#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

#include <iostream>

#include "Engine/Core/Text/TTFLoader.hpp"
#include "Engine/Core/Text/FontRasterizer.hpp"

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
    char character;
    std::cin >> character;

    auto glyphData = font.getGlyphs()[character];
    FontRasterizer raster{font.getFontData(), glyphData};
    auto glyph = raster.rasterize(120);
    window.emplaceDrawable<DefaultSprite>(Texture<>{glyph}, 20_x + 300_y, static_cast<Vector2f>(glyph.size()));

    window.emplaceDrawable<DrawableArray<Triangle<true>>>();
    auto& array = dynamic_cast<DrawableArray<Triangle<true>>&>(*window[2]);
    array.emplaceBack(400_x + 400_y, 300_x + 300_y, 400_x + 300_y, Color::literals::White);
    array.emplaceBack(200_x + 200_y, 200_x + 300_y, 300_x + 200_y, Color::literals::Green);

    window.emplaceDrawable<DefaultSprite>(pack["pad.jpeg"], 100_x + 100_y, 100_x + 100_y);
    auto& sprite = dynamic_cast<DefaultSprite&>(*window[3]);
    sprite.rotate(150_x + 150_y, std::numbers::pi_v<double> / 4);

    return window.windowLoop();
}

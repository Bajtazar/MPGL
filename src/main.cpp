#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

#include <iostream>

#include "Engine/Core/Text/UTF-8.hpp"
#include "Engine/Core/Text/GlyphSprite.hpp"
#include "Engine/Core/Text/Subfont.hpp"

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

    Subfont font{"/usr/share/fonts/truetype/hack/Hack-Regular.ttf"};
    std::string character;
    std::cin >> character;

    auto refglyph = font(fromUTF8(character), 128);
    auto glyph = refglyph->get();

    window.emplaceDrawable<PoliGlyphSprite>(*glyph.texture, 20_x + 300_y, vectorCast<float>(glyph.dimmensions));

    auto& text = dynamic_cast<PoliGlyphSprite&>(*window[1]);
    text[0].color = Color::literals::Red;
    text[1].color = Color::literals::Green;
    text[2].color = Color::literals::Blue;
    text[3].color = Color::literals::Yellow;

    window.emplaceDrawable<DrawableArray<Triangle<true>>>();
    auto& array = dynamic_cast<DrawableArray<Triangle<true>>&>(*window[2]);
    array.emplaceBack(400_x + 400_y, 300_x + 300_y, 400_x + 300_y, Color::literals::White);
    array.emplaceBack(200_x + 200_y, 200_x + 300_y, 300_x + 200_y, Color::literals::Green);

    window.emplaceDrawable<DefaultSprite>(pack["pad.jpeg"], 100_x + 100_y, 100_x + 100_y);
    auto& sprite = dynamic_cast<DefaultSprite&>(*window[3]);
    sprite.rotate(150_x + 150_y, std::numbers::pi_v<double> / 4);

    return window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
}

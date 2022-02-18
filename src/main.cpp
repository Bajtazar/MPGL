/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022 Grzegorz Czarnecki (wahadlowiec@interia.pl)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include "Engine/2DGraphics.hpp"
#include <math.h>
#include <numbers>

using namespace mpgl;

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

    TextureLoader loader{"textures"};
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

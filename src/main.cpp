/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
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

class RotatingSprite : public Drawable,
    public TickEvent, public ScreenTransformationEvent
{
public:
    RotatingSprite(uint32 rotationTime,
            Texture const& texture)
        : square{texture, 100_x + 100_y, 100_x + 100_y},
        rotationTime{rotationTime}
    {
        get<"color">(square[0]) = Color::Red;
        get<"color">(square[1]) = Color::Green;
        get<"color">(square[2]) = Color::Blue;
        get<"color">(square[3]) = Color::Yellow;
    }

    void onTick(std::chrono::milliseconds const& duration) noexcept {
        square.rotate(150_x + 150_y,
            2 * std::numbers::pi / (1000 * rotationTime)
            * duration.count());
    }

    void onScreenTransformation(Vector2u const& oldDim) noexcept
        { square.onScreenTransformation(oldDim); }

    void draw(void) const noexcept
        { square.draw(); }

    ~RotatingSprite(void) noexcept = default;
private:
    Sprite<true>                            square;
    uint32 const                            rotationTime;
};

class RotatingRing : public Drawable,
    public TickEvent, public ScreenTransformationEvent
{
public:
    RotatingRing(void) : ring{200_x + 400_y, 50.f,
        Ring::InnerEllipse{200_x + 400_y, 25.f}, Color::Red},
            time{0} {}

    void onTick(std::chrono::milliseconds const& duration) noexcept {
        time += duration.count();
        ring.getInnerEllipse().translate({
            std::cos(time * std::numbers::pi / 2000.f) * duration.count()
            * std::numbers::pi / 80.f, 0.f});
    }

    void onScreenTransformation(Vector2u const& oldDim) noexcept
        { ring.onScreenTransformation(oldDim); }

    void draw(void) const noexcept
        { ring.draw(); }

    ~RotatingRing(void) noexcept = default;
private:
    Ring                                    ring;
    uint32                                  time;
};

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

    window.emplaceDrawable<DrawableCollection<LineLoop>>();
    auto& array = dynamic_cast<DrawableCollection<LineLoop>&>(*window[2]);
    array.emplace_back(Color::White, 400_x + 400_y, 300_x + 300_y, 400_x + 300_y);
    array.emplace_back(Color::Green, 200_x + 200_y, 200_x + 300_y, 300_x + 200_y);

    window.emplaceDrawable<RotatingSprite>(4, pack["pad.jpeg"]);

    window.emplaceDrawable<RotatingRing>();

    return window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
}

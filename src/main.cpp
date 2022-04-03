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

class RotatingSprite : public Drawable2D,
    public TickEvent, public ScreenTransformationEvent,
    public KeyPressEvent, public MouseMotionEvent
{
public:
    RotatingSprite(uint32 rotationTime,
            Texture const& texture, Font& font)
        : square{texture, 150_x + 150_y, 50.f, 25.f},
        rotationTime{rotationTime},
        name{font, 100_x + 80_y, "Konwolucja: Brak", {.size=18}}
    {
        square[0] & cast::color = Color::Red;
        square[1] & cast::color = Color::Green;
        square[2] & cast::color = Color::Blue;
        square[3] & cast::color = Color::Yellow;
    }

    void onTick(std::chrono::milliseconds const& duration) noexcept {
        square.rotate(150_x + 150_y,
            sign * 2 * std::numbers::pi / (1000 * rotationTime)
            * duration.count());
    }

    void onScreenTransformation(Vector2u const& oldDim) noexcept {
        square.onScreenTransformation(oldDim);
        name.onScreenTransformation(oldDim);
    }

    void draw(void) const noexcept {
        square.draw();
        name.draw();
    }

    void onKeyPress(Key const& key) noexcept {
        switch (key) {
            case Key::Q:
                square.resetConvolution();
                name.setString("Konwolucja: Brak");
                return;
            case Key::W:
                square.setConvolution(kernels::gaussian);
                name.setString("Konwolucja: Gaussowska");
                return;
            case Key::E:
                square.setConvolution(kernels::mean);
                name.setString("Konwolucja: Pudełkowa");
                return;
            case Key::R:
                square.setConvolution(kernels::laplacian);
                name.setString("Konwolucja: Laplace'a");
                return;
            case Key::T:
                square.setConvolution(kernels::embos);
                name.setString("Konwolucja: Embos");
                return;
            case Key::Y:
                square.setConvolution(kernels::sharpness);
                name.setString("Konwolucja: Wyostrzająca");
                return;
        }
    }

    void onMouseMotion(Vector2f const& motion) noexcept {
        sign = square.contains(motion) ? 2 : 1;
    }

    ~RotatingSprite(void) noexcept = default;
private:
    Text<false>                                 name;
    RingSprite<true>                            square;
    uint32 const                                rotationTime;
    uint32                                      sign = 1;
};

class RotatingRing : public Drawable2D,
    public TickEvent, public ScreenTransformationEvent,
    public MouseMotionEvent
{
public:
    RotatingRing(void) : ring{200_x + 400_y, 50.f, 25.f, Color::MPGLBlue},
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

    void onMouseMotion(Vector2f const& motion) noexcept {
        if (ring.contains(motion) != clicked) {
            if (clicked)
                ring.setColor(Color::Red);
            else
                ring.setColor(Color::Blue);
            clicked = !clicked;
        }
    }

    ~RotatingRing(void) noexcept = default;
private:
    Ring                                    ring;
    uint32                                  time;
    bool                                    clicked = false;
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

    window.emplaceDrawable<Text<>>(font, 20_x + 300_y, "Witaj\nświecie!",
        TextOptions{.size=18, .angle=std::numbers::pi / 4.f});

    window.emplaceDrawable<DrawableCollection<LineLoop>>();
    auto& array = dynamic_cast<DrawableCollection<LineLoop>&>(*window[2]);
    array.emplace_back(Color::White, 400_x + 400_y, 300_x + 300_y, 400_x + 300_y);
    array.emplace_back(Color::Green, 200_x + 200_y, 200_x + 300_y, 300_x + 200_y);

    window.emplaceDrawable<RotatingSprite>(4, pack["test.png"], font);
    window.emplaceDrawable<RotatingRing>();

    window.windowLoop(Color{0.2f, 0.3f, 0.3f, 1.f});
    return 0;
}

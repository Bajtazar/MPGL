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
#include <Snake.hpp>

#include <MPGL/Core/Figures/Views.hpp>

#include <ranges>

using namespace mpgl;

Snake::Snake(World& world)
    : momentum{1, 0}, world{world}, freeze{false}
{
    if (world.applePosition() == Vector2f{0, 0}) {
        snakeModules.emplace_back(returnModule({1, 0}));
        modules.emplace_back(1, 0);
    } else {
        snakeModules.emplace_back(returnModule({0, 0}));
        modules.emplace_back(0, 0);
    }
}

Tetragon Snake::returnModule(Vector2si const& vector) const noexcept {
    return Tetragon{
        {117 + 36 * vector[0], 36 * (vector[1] + 3) + 9},
        {18, 18},
        Color::Green
    };
}

bool Snake::isFreeze(void) noexcept {
    if (!between({0, 0}, {24, 14}, modules.front()))
        return onFreeze();
    auto moduleView = modules | std::views::drop(1)
        | std::views::reverse | std::views::drop(1);
    for (auto const& module : moduleView)
        if (module == modules.front())
            return onFreeze();
    return false;
}

bool Snake::onFreeze(void) noexcept{
    for (auto& tetragon : snakeModules)
        for (auto& color : tetragon | views::color)
            color = Color::Blue;
    return freeze = true;
}

void Snake::onTick(
    [[maybe_unused]] std::chrono::milliseconds const& delta) noexcept
{
    if (freeze)
        return;
    modules.push_front(modules.front() + vectorCast<float32>(momentum));
    if (isFreeze())
        return modules.pop_back();
    snakeModules.push_front(returnModule(modules.front()));
    if (modules.front() == world.applePosition())
        world.createNewApple();
    else {
        snakeModules.pop_back();
        modules.pop_back();
    }
}

void Snake::onKeyPress(Key const& key) noexcept {
    switch (key) {
        case Key::Right:
        case Key::D:
            momentum = Vector2si{1, 0};
            break;
        case Key::Left:
        case Key::A:
            momentum = Vector2si{-1, 0};
            break;
        case Key::Down:
        case Key::S:
            momentum = Vector2si{0, -1};
            break;
        case Key::Up:
        case Key::W:
            momentum = Vector2si{0, 1};
            break;
        default:
            break;
    }
}

void Snake::onScreenTransformation(
    Layout& layout,
    Vector2u const& oldDimensions) noexcept
{
    snakeModules.onScreenTransformation(layout, oldDimensions);
}

void Snake::draw(void) const noexcept {
    snakeModules.draw();
}

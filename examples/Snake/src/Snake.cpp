#include <Snake.hpp>

#include <MPGL/Core/Figures/Views.hpp>

#include <ranges>

using namespace mpgl;

Snake::Snake(World& world)
    : world{world}, freeze{false}, momentum{1, 0}
{
    if (totalCompare(world.applePosition(), {0, 0})
        == std::partial_ordering::equivalent)
    {
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

bool Snake::isFreeze(void) noexcept{
    if (!between(0.f, 14.f, modules.front()[0]))
        return onFreeze();
    if (!between(0.f, 23.f, modules.front()[1]))
        return onFreeze();
    for (auto const& module : modules)
        if (totalCompare(module, modules.front())
            == std::partial_ordering::equivalent)
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
    modules.push_front(modules.back() + vectorCast<float32>(momentum));
    if (isFreeze())
        return modules.pop_back();
    snakeModules.push_front(returnModule(modules.front()));
    if (totalCompare(modules.front(), world.applePosition())
        == std::partial_ordering::equivalent)
    {
        world.createNewApple();
    } else {
        snakeModules.pop_back();
        modules.pop_back();
    }
}


void Snake::onKeyPress(Key const& key) noexcept {
    switch (key) {
        case Key::Right:
            momentum = Vector2si{1, 0};
            break;
        case Key::Left:
            momentum = Vector2si{-1, 0};
            break;
        case Key::Up:
            momentum = Vector2si{0, -1};
            break;
        case Key::Down:
            momentum = Vector2si{0, 1};
            break;
    }
}

void Snake::onScreenTransformation(
    Vector2u const& oldDimensions) noexcept
{
    snakeModules.onScreenTransformation(oldDimensions);
}

void Snake::draw(void) const noexcept {
    snakeModules.draw();
}

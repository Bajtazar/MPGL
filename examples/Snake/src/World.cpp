/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <World.hpp>

#include <MPGL/Core/Vertex/VertexCast.hpp>

#include <chrono>

using namespace mpgl;

World::World(void)
    : apple{Vector2f{}, Tetragon2D{Color::Red}}
{
    generateTable();
    createNewApple();
}

void World::createNewApple(void) noexcept {
    apple.first = genApplePosition();
    auto const& [k, j] = apple.first;
    apple.second[0] | cast::position
        = Vector2f{117 + 36*j, 36*(k+3)+9};
    apple.second[1] | cast::position
        = Vector2f{99 + 36*(j+1), 36*(k+3)+9};
    apple.second[2] | cast::position
        = Vector2f{99 + 36*(j+1), 36*(k+4)-9};
    apple.second[3] | cast::position
        = Vector2f{117 + 36*j, 36*(k+4)-9};
}

Vector2f World::genApplePosition(void) noexcept {
    uint64_t time = std::chrono::duration_cast<
        std::chrono::nanoseconds>(std::chrono::system_clock::now(
            ).time_since_epoch()).count();
    return {time % 24, time % 14};
}

void World::generateTable(void) {
    table.reserve(40);
    for (uint16_t i = 0;i < 15;i++)
        table.emplace_back(Vector2f{108, 36*(i+3)},
            Vector2f{972, 36*(i+3)}, colors::white);
    for (uint16_t i = 0;i < 25;i++)
        table.emplace_back(Vector2f{108 + 36*i, 108},
            Vector2f{108 + 36*i, 612}, colors::white);
}

void World::transform(
    Transformation2D const& transformator) noexcept
{
    table.transform(transformator);
    apple.second.transform(transformator);
}

void World::draw(void) const noexcept {
    table.draw();
    apple.second.draw();
}

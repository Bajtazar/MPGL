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
#pragma once

#include <MPGL/Events/Types/KeyPressEvent.hpp>
#include <MPGL/Events/Types/TickEvent.hpp>

#include <World.hpp>

#include <list>

class Snake :
    public mpgl::Transformable2D,
    public mpgl::KeyPressEvent,
    public mpgl::Drawable2D,
    public mpgl::TickEvent
{
public:
    explicit Snake(World& world);

    virtual void onKeyPress(mpgl::KeyboardKey const& key) noexcept;

    virtual void transform(
        Transformation2D const& transformator) noexcept;

    virtual void draw(void) const noexcept;

    virtual void onTick(
        std::chrono::milliseconds const& delta) noexcept;

    ~Snake(void) noexcept = default;
private:
    typedef std::list<mpgl::Vector2f>               Modules;
    typedef mpgl::DrawableCollection<
        mpgl::Tetragon2D,
        std::list<mpgl::Tetragon2D>>                SnakeModules;

    SnakeModules                                    snakeModules;
    Modules                                         modules;
    mpgl::Vector2si                                 momentum;
    World&                                          world;
    bool                                            freeze;

    bool isFreeze(void) noexcept;

    bool onFreeze(void) noexcept;

    mpgl::Tetragon2D returnModule(
        mpgl::Vector2si const& vector) const noexcept;
};

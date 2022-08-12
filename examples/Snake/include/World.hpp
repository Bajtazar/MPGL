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
#pragma once

#include <MPGL/Events/Types/ScreenTransformationEvent.hpp>
#include <MPGL/Core/Figures/Primitives/Tetragon.hpp>
#include <MPGL/Core/Figures/Primitives/Line.hpp>
#include <MPGL/Core/DrawableCollection.hpp>
#include <MPGL/Core/Drawable.hpp>

#include <utility>

class World :
    public mpgl::Transformable2D,
    public mpgl::Drawable2D
{
public:
    explicit World(void);

    virtual void transform(
        Transformation2D const& transformator) noexcept;

    virtual void draw(void) const noexcept;

    void createNewApple(void) noexcept;

    mpgl::Vector2f const& applePosition(void) const noexcept
        { return apple.first; }

    ~World(void) noexcept = default;
private:
    typedef mpgl::DrawableCollection<
        mpgl::Line2D>                           Lines;
    typedef std::pair<
        mpgl::Vector2f, mpgl::Tetragon>         AppleTuple;

    Lines                                       table;
    AppleTuple                                  apple;

    void generateTable(void);

    static mpgl::Vector2f genApplePosition(void) noexcept;
};

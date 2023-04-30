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

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    template <
        class ColorTp,
        AllConvertible<
        typename LineStrip<Dim, Spec>::Vector>... Vectors>
            requires std::constructible_from<Color, ColorTp>
    LineStrip<Dim, Spec>::LineStrip(
        ColorTp&& color,
        Vectors&&... vertices)
            : ResizableAngular<Dim, Spec>{
                std::forward<ColorTp>(color),
                std::forward<Vectors>(vertices)...} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    template <AllConvertible<
        typename LineStrip<Dim, Spec>::Vector>... Vectors>
    LineStrip<Dim, Spec>::LineStrip(Vectors&&... vertices)
        : ResizableAngular<Dim, Spec>{
            std::forward<Vectors>(vertices)...} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    LineStrip<Dim, Spec>::Drawer const
        LineStrip<Dim, Spec>::drawer = {};

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    LineStrip<Dim, Spec>::Clicker const
        LineStrip<Dim, Spec>::clicker = {};

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    LineStrip<Dim, Spec>::LineStrip(
        std::size_t vertices,
        Color const& color)
            : ResizableAngular<Dim, Spec>{vertices, color} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void LineStrip<Dim, Spec>::draw(void) const noexcept {
        drawer(*this);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool LineStrip<Dim, Spec>::contains(
        Vector2u const& position) const noexcept
    {
        return clicker(*this, position);
    }

}

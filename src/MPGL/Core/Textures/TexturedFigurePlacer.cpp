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
#include <MPGL/Core/Textures/TexturedFigurePlacer.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Triangle>::Setter<Dim, Spec>::operator() (
        Triangle<Dim, Spec>& triangle) const noexcept
    {
        get<"texCoords">(triangle[0]) = Vector2f{0, 0};
        get<"texCoords">(triangle[1]) = Vector2f{1, 0};
        get<"texCoords">(triangle[2]) = Vector2f{0.5, 1.};
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Tetragon>::Setter<Dim, Spec>::operator() (
        Tetragon<Dim, Spec>& tetragon) const noexcept
    {
        get<"texCoords">(tetragon[0]) = Vector2f{0, 0};
        get<"texCoords">(tetragon[1]) = Vector2f{0, 1};
        get<"texCoords">(tetragon[2]) = Vector2f{1, 1};
        get<"texCoords">(tetragon[3]) = Vector2f{1, 0};
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Line>::Setter<Dim, Spec>::operator() (
        Line<Dim, Spec>& line) const noexcept
    {
        get<"texCoords">(line[0]) = Vector2f{0, 0};
        get<"texCoords">(line[1]) = Vector2f{1, 0};
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<LineStrip>::Setter<Dim, Spec>::operator() (
        LineStrip<Dim, Spec>& lineStrip) const noexcept
    {
        if (lineStrip.size() <= 1)
            return;
        auto const piece = 1.f / (lineStrip.size() - 1);
        std::remove_const_t<decltype(piece)> iter = 0.;
        for (auto& vertex : lineStrip | views::texCoords) {
            vertex = Vector2f{iter, 0};
            iter += piece;
        }
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<LineLoop>::Setter<Dim, Spec>::operator() (
        LineLoop<Dim, Spec>& lineLoop) const noexcept
    {
        if (lineLoop.size() <= 1)
            return;
        auto const piece = 1.f / (lineLoop.size() - 1);
        std::remove_const_t<decltype(piece)> iter = 0.;
        for (auto& vertex : lineLoop | views::texCoords) {
            vertex = Vector2f{iter, 0};
            iter += piece;
        }
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Points>::Setter<Dim, Spec>::operator() (
        Points<Dim, Spec>& points) const noexcept
    {
        if (points.size() <= 1)
            return;
        auto const piece = 1.f / (points.size() - 1);
        std::remove_const_t<decltype(piece)> iter = 0.;
        for (auto& vertex : points | views::texCoords) {
            vertex = Vector2f{iter, 0};
            iter += piece;
        }
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Polygon>::Setter<Dim, Spec>::operator() (
        Polygon<Dim, Spec>& polygon) const noexcept
    {
        if (polygon.size() <= 1)
            return;
        auto const piece = 1.f / (polygon.size() - 1);
        std::remove_const_t<decltype(piece)> iter = 0.;
        for (auto& vertex : polygon | views::texCoords) {
            vertex = Vector2f{iter, 0};
            iter += piece;
        }
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Ellipse>::Setter<Dim, Spec>::operator() (
        Ellipse<Dim, Spec>& ellipse) const noexcept
    {
        get<"texCoords">(ellipse[0]) = Vector2f{0, 0};
        get<"texCoords">(ellipse[1]) = Vector2f{0, 1};
        get<"texCoords">(ellipse[2]) = Vector2f{1, 1};
        get<"texCoords">(ellipse[3]) = Vector2f{1, 0};
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Ring>::Setter<Dim, Spec>::operator() (
        Ring<Dim, Spec>& ring) const noexcept
    {
        get<"texCoords">(ring[0]) = Vector2f{0, 0};
        get<"texCoords">(ring[1]) = Vector2f{0, 1};
        get<"texCoords">(ring[2]) = Vector2f{1, 1};
        get<"texCoords">(ring[3]) = Vector2f{1, 0};
    }

}

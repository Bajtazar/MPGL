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
#include <MPGL/Core/Figures/Primitives/Line.hpp>

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Line<Dim, Spec>::Drawer const
        Line<Dim, Spec>::drawer = {};

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Line<Dim, Spec>::Clicker const
        Line<Dim, Spec>::clicker = {};

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Line<Dim, Spec>::Line(
        Vector const& firstVertex,
        Vector const& secondVertex,
        Color const& color)
            : Angular<Dim, Spec>{{
                VertexTraits::buildVertex(firstVertex, color),
                VertexTraits::buildVertex(secondVertex, color)}} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Line<Dim, Spec>::Line(Color const& color)
        : Angular<Dim, Spec>{2, color} {}

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] Line<Dim, Spec>::Vector
        Line<Dim, Spec>::getLineCenter(void) const noexcept
    {
        return (static_cast<Vector>(get<"position">(this->vertices[0]))
            + static_cast<Vector>(get<"position">(this->vertices[1]))) / 2.f;
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Line<Dim, Spec>::draw(void) const noexcept {
        drawer(*this);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool Line<Dim, Spec>::contains(
        Vector2u const& position) const noexcept
    {
        return clicker(*this, position);
    }

}

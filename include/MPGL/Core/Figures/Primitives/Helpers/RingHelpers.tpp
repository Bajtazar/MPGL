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

#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range, Adapter2D>
    [[nodiscard]] RingOutlineCalculator<dim::Dim2, Spec>::ResultT
        RingOutlineCalculator<dim::Dim2, Spec>::operator() (
            std::remove_cvref_t<Range> const& ring) const noexcept
    {
        return invert(transpose(Matrix2f{
            Vector2f{ring[1]} - Vector2f{ring[0]},
            Vector2f{ring[3]} - Vector2f{ring[0]}}));
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range, Adapter3D>
    [[nodiscard]] RingOutlineCalculator<dim::Dim3, Spec>::ResultT
        RingOutlineCalculator<dim::Dim3, Spec>::operator() (
            std::remove_cvref_t<Range> const& ring) const noexcept
    {
        auto const& [xVersor, yVersor, zVersor] = getVersors(ring);
        if (cross(xVersor, yVersor))
            return { getXYMatrix(xVersor, yVersor) };
        else if (cross(xVersor, zVersor))
            return { getXZMatrix(xVersor, zVersor) };
        else if (cross(yVersor, zVersor))
            return { getYZMatrix(yVersor, zVersor) };
        return std::nullopt;
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range, Adapter3D>
    [[nodiscard]] RingOutlineCalculator<dim::Dim3, Spec>::Versors
        RingOutlineCalculator<dim::Dim3, Spec>::getVersors(
            std::remove_cvref_t<Range> const& ring) const noexcept
    {
        Vector3f const a = Vector3f{ring[3]} - Vector3f{ring[0]};
        Vector3f const b = Vector3f{ring[1]} - Vector3f{ring[0]};
        return {
            Vector2f{a[0], b[0]},
            Vector2f{a[1], b[1]},
            Vector2f{a[2], b[2]}
        };
    }

    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    template <std::ranges::input_range Range>
        requires SameRangeType<Range, Adapter2D>
    [[nodiscard]] bool RingClickChecker<dim::Dim2, Spec>::insideSystem(
        std::remove_cvref_t<Range> const& ring,
        MatrixT const& outline,
        Vector2u const& position) const noexcept
    {
        Vector2f local = outline * (vectorCast<float32>(position) -
            Vector2f{ring[0]});
        return (local - 0.5f).length() <= 0.5f;
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range, Adapter3D>
    [[nodiscard]] bool RingClickChecker<dim::Dim3, Spec>::insideSystem(
        std::remove_cvref_t<Range> const& ring,
        Matrix4f const& model,
        Vector2u const& position) const noexcept
    {
        auto iter = (ring | views::project(ellipse.model)).begin();
        auto const p = Adapter2D{position}.get();
        auto const v0 = Adapter2D{*iter++}.get();
        auto const v1 = Adapter2D{*iter++}.get();
        auto const v2 = Adapter2D{*iter++}.get();
        auto const v3 = Adapter2D{*iter++}.get();
        return isInsideTriangle(p, v0, v1, v2) ||
            isInsideTriangle(p, v0, v2, v3);
    }

}

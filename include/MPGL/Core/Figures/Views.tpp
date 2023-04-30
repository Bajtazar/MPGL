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

#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <std::ranges::view Range>
        requires (std::same_as<std::remove_cvref_t<
            std::ranges::range_value_t<Range>>, Adapter3D>)
    Matrix4f const ProjectedPositionView<Range>::Default
        = identityMatrix<float32, 4>();

    template <std::ranges::view Range>
        requires (std::same_as<std::remove_cvref_t<
            std::ranges::range_value_t<Range>>, Adapter3D>)
    [[nodiscard]] Vector2f
        ProjectedPositionView<Range>::iterator::operator*(
            void) const noexcept
    {
        Vector4f projected = context.getViewProjection() * matrix.get()
            * extend(Vector3f(*iter));
        Vector2f result{ projected[0], projected[1] };
        return vectorCast<float32>(context.windowDimensions) * (
            result / projected[3] + 1.f) / 2.f;
    }

}

namespace mpgl::details {

    template <TemplateString Field>
    template <std::ranges::viewable_range Range>
    [[nodiscard]] constexpr auto
        VertexComponentViewAdaptorClosure<Field>::operator() (
            Range&& range) const noexcept
    {
        return VertexComponentView<Field,
            std::views::all_t<Range>>{
                std::forward<Range>(range) };
    }

    template <std::ranges::viewable_range Range>
    requires (std::same_as<std::remove_cvref_t<
        std::ranges::range_value_t<Range>>, Adapter3D>)
    [[nodiscard]] auto ProjectedPositionViewAdaptorClosure::operator() (
        Range&& range) const noexcept
    {
        return ProjectedPositionView<std::views::all_t<Range>>{
            range, matrix};
    }

    template <TemplateString Field>
    template <std::ranges::viewable_range Range>
    [[nodiscard]] constexpr auto
        VertexComponentViewAdaptor<Field>::operator() (
            Range&& range) const noexcept
    {
        return VertexComponentView<Field,
            std::views::all_t<Range>>{
                std::forward<Range>(range) };
    }

    template <TemplateString Field>
    [[nodiscard]] constexpr auto
        VertexComponentViewAdaptor<Field>::operator() (
            void) const noexcept
    {
        return VertexComponentViewAdaptorClosure<Field>{};
    }

    template <std::ranges::viewable_range Range>
    requires (std::same_as<std::remove_cvref_t<
        std::ranges::range_value_t<Range>>, Adapter3D>)
    [[nodiscard]] auto ProjectedPositionViewAdaptor::operator() (
        Range&& range,
        Matrix4f const& matrix) const noexcept
    {
        return ProjectedPositionView<std::views::all_t<Range>>{
            range, matrix};
    }

    template <std::ranges::viewable_range Range,
        TemplateString Field>
    [[nodiscard]] constexpr auto operator | (
        Range&& range,
        VertexComponentViewAdaptorClosure<Field> const& closure
        ) noexcept
    {
        return closure(std::forward<Range>(range));
    }

    template <std::ranges::viewable_range Range>
    requires (std::same_as<std::remove_cvref_t<
        std::ranges::range_value_t<Range>>, Adapter3D>)
    [[nodiscard]] auto operator | (
        Range&& range,
        ProjectedPositionViewAdaptorClosure const& closure
        ) noexcept
    {
        return closure(std::forward<Range>(range));
    }

    template <std::ranges::viewable_range Range,
        TemplateString Field>
    [[nodiscard]] constexpr auto operator | (
        Range&& range,
        VertexComponentViewAdaptor<Field> const& adaptor) noexcept
    {
        return adaptor(std::forward<Range>(range));
    }

    template <std::ranges::viewable_range Range>
    requires (std::same_as<std::remove_cvref_t<
        std::ranges::range_value_t<Range>>, Adapter3D>)
    [[nodiscard]] constexpr auto operator | (
        Range&& range,
        ProjectedPositionViewAdaptor const& adaptor) noexcept
    {
        return adaptor(std::forward<Range>(range));
    }

}

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

    template <std::ranges::viewable_range Range,
        TemplateString Field>
    [[nodiscard]] constexpr auto operator | (
        Range&& range,
        VertexComponentViewAdaptorClosure<Field> const& closure
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

}

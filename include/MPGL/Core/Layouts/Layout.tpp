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

#include <MPGL/Core/Layouts/Layout.hpp>

namespace mpgl {

    template <std::derived_from<Transformable2D> Tp>
    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
        requires std::convertible_to<std::iter_value_t<Iter>,
            typename Layout<Tp>::LayoutPair>
    Layout<Tp>::Layout(Iter first, Sent const& last)
        : shapes{std::move(first), last} {}

    template <std::derived_from<Transformable2D> Tp>
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_value_t<
            Range>, typename Layout<Tp>::LayoutPair>
    Layout<Tp>::Layout(Range&& range)
        : shapes{std::ranges::begin(range), std::ranges::end(range)}
            {}

    template <std::derived_from<Transformable2D> Tp>
    Layout<Tp>::Layout(std::initializer_list<LayoutPair> init)
        : shapes{std::move(init)} {}

    template <std::derived_from<Transformable2D> Tp>
    [[nodiscard]] std::size_t Layout<Tp>::Hash::operator() (
        LayoutPair const& layoutPair) const noexcept
    {
        return hasher(layoutPair.transformable);
    }

    template <std::derived_from<Transformable2D> Tp>
    [[nodiscard]] bool Layout<Tp>::EqualTo::operator() (
        LayoutPair const& left,
        LayoutPair const& right) const noexcept
    {
        return left.transformable == right.transformable;
    }

    template <std::derived_from<Transformable2D> Tp>
    Layout<Tp>::iterator Layout<Tp>::insert(
        const_iterator hint,
        Vector2u const& hook,
        pointer pointer)
    {
        return shapes.insert(hint, LayoutPair{hook, pointer});
    }

    template <std::derived_from<Transformable2D> Tp>
    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
        requires std::convertible_to<std::iter_value_t<Iter>,
            typename Layout<Tp>::LayoutPair>
    Layout<Tp>::iterator Layout<Tp>::insert(
        Iter first,
        Sent const& last)
    {
        return shapes.insert(std::move(first), last);
    }

    template <std::derived_from<Transformable2D> Tp>
    template <std::ranges::input_range Range>
        requires std::convertible_to<std::ranges::range_value_t<
            Range>, typename Layout<Tp>::LayoutPair>
    Layout<Tp>::iterator Layout<Tp>::insert(Range&& range) {
        return shapes.insert(std::ranges::begin(range),
            std::ranges::end(range));
    }

    template <std::derived_from<Transformable2D> Tp>
    template <typename... Args>
        requires std::constructible_from<
            typename Layout<Tp>::LayoutPair, Args...>
    Layout<Tp>::InsertResult Layout<Tp>::emplace(Args&&... args) {
        return shapes.emplace(std::forward(args)...);
    }

    template <std::derived_from<Transformable2D> Tp>
    template <typename... Args>
        requires std::constructible_from<
            typename Layout<Tp>::LayoutPair, Args...>
    Layout<Tp>::iterator Layout<Tp>::emplaceHint(
        const_iterator hint,
        Args&&... args)
    {
        return shapes.emplace_hint(std::move(hint),
            std::forward(args)...);
    }


}

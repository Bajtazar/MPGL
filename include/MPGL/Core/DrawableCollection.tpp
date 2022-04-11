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

namespace mpgl {

    template <InstanceOf<Drawable> Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(void) const noexcept {
        std::ranges::for_each(*this,
            [](auto const& drawable){ drawable.draw(); });
    }

    template <InstanceOf<Drawable> Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(
        std::size_t begin,
        std::size_t end) const noexcept
            requires std::ranges::random_access_range<Range>
    {
        for (std::size_t i = begin; i < end; ++i)
            (*this)[i].draw();
    }

    template <details::ScreenExtDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(void) const noexcept {
        std::ranges::for_each(*this,
            [](auto const& drawable){ drawable.draw(); });
    }

    template <details::ScreenExtDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(
        std::size_t begin,
        std::size_t end) const noexcept
            requires std::ranges::random_access_range<Range>
    {
        for (std::size_t i = begin; i < end; ++i)
            (*this)[i].draw();
    }

    template <details::ScreenExtDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        std::ranges::for_each(*this, [&oldDimensions](auto& drawable)
            { drawable.onScreenTransformation(oldDimensions); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(void) const noexcept {
        std::ranges::for_each(*this,
            [](auto const& drawable){ drawable.draw(); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(
        std::size_t begin,
        std::size_t end) const noexcept
            requires std::ranges::random_access_range<Range>
    {
        for (std::size_t i = begin; i < end; ++i)
            (*this)[i].draw();
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        std::ranges::for_each(*this, [&oldDimensions](auto& drawable)
            { drawable.onScreenTransformation(oldDimensions); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::translate(
        Vector2f const& shift) noexcept
    {
        std::ranges::for_each(*this, [&shift](auto& drawable)
            { drawable.translate(shift); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        std::ranges::for_each(*this, [&center, &factor](auto& drawable)
            { drawable.scale(center, factor); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        auto rot = rotationMatrix<float32>(angle);
        std::ranges::for_each(*this, [&center, &rot](auto& drawable)
            { drawable.rotate(center, rot); });
    }

    template <details::Trans2DDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        std::ranges::for_each(*this, [&center, &rotation]
            (auto& drawable){ drawable.rotate(center, rotation); });
    }

}

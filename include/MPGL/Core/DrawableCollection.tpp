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

    template <SpecializationOf<Drawable> Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(void) const noexcept {
        std::ranges::for_each(*this,
            [](auto const& drawable){ drawable.draw(); });
    }

    template <SpecializationOf<Drawable> Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(
        std::size_t begin,
        std::size_t end) const noexcept
            requires std::ranges::random_access_range<Range>
    {
        for (std::size_t i = begin; i < end; ++i)
            (*this)[i].draw();
    }

    template <details::TransDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(void) const noexcept {
        std::ranges::for_each(*this,
            [](auto const& drawable){ drawable.draw(); });
    }

    template <details::TransDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::draw(
        std::size_t begin,
        std::size_t end) const noexcept
            requires std::ranges::random_access_range<Range>
    {
        for (std::size_t i = begin; i < end; ++i)
            (*this)[i].draw();
    }

    template <details::TransDrawable Base,
        std::ranges::input_range Range>
    void DrawableCollection<Base, Range>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        for (auto& drawable : *this)
            drawable.transform(transformator);
    }

}

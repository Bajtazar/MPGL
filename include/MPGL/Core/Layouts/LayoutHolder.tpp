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

    template <std::derived_from<mpgl::Layout> Tp, class... Args>
    void LayoutHolder::Layout<Tp, Args...>::operator() (
        Transformable2D& transformable,
        Vector2u const& dimensions) const noexcept
    {
        if constexpr (sizeof...(Args)) {
            std::apply([&]<typename... Inner>(Inner&&... args) {
                transformable.transform(
                    Tp{dimensions, std::forward<Inner>(args)...});
            }, layoutTag.args);
        } else {
            transformable.transform(Tp{dimensions});
        }
    }

    template <std::derived_from<mpgl::Layout> Tp, class... Args>
    void LayoutHolder::Layout<Tp, Args...>::move(
        std::byte& memory) noexcept
    {
        new (&memory) Layout<Tp, Args...>{std::move(*this)};
    }

    template <std::derived_from<mpgl::Layout> Tp, class... Args>
    LayoutHolder::InlineMemory::InlineMemory(
        LayoutTag<Tp, Args...>&& tag) noexcept
    {
        new (&memory) Layout{std::move(tag)};
    }

    template <std::derived_from<mpgl::Layout> Tp, class... Args>
    LayoutHolder::Storage LayoutHolder::createStorage(
        LayoutTag<Tp, Args...>&& tag)
    {
        if constexpr (sizeof(Layout<Tp, Args...>) > InlineSize) {
            return { std::make_unique<Layout<Tp, Args...>>(
                std::move(tag)) };
        } else {
            return { InlineMemory{std::move(tag)} };
        }
    }

    template <std::derived_from<mpgl::Layout> Tp, class... Args>
    LayoutHolder::LayoutHolder(
        TransformablePtr pointer,
        LayoutTag<Tp, Args...>&& layoutTag)
            : storage{createStorage(std::move(layoutTag))},
            transformable{std::move(pointer)} {}

}

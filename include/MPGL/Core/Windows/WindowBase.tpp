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

    template <Event Tp>
        requires (!std::derived_from<Tp, WindowBase::STEvent>)
    void WindowBase::pushEvent(std::shared_ptr<Tp> const& event) {
        eventManager->push(event);
    }

    template <std::derived_from<WindowBase::STEvent> Tp>
    void WindowBase::pushEvent(
        std::shared_ptr<Tp> const& event,
        LayoutPtr layout)
    {
        eventManager->push(event);
        layouts.push_back(std::move(layout));
    }

    template <Event Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceEvent(Args&&... args) {
        eventManager->emplace(std::forward(args)...);
        if constexpr (std::derived_from<Tp, STEvent>)
            layouts.push_back(std::make_unique<DefaultLayout>());
    }

    template <std::derived_from<WindowBase::STEvent> Tp,
        typename... Args>
            requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceEventWithLayout(
        LayoutPtr&& layout,
        Args&&... args)
    {
        eventManager->emplace(std::forward(args)...);
        layouts.push_back(std::move(layout));
    }

    template <std::derived_from<Drawable2D> Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceDrawable(Args&&... args) {
        auto ptr = std::make_shared<Tp>(std::forward<Args>(args)...);
        if constexpr (Event<Tp>)
            eventManager->push(ptr);
        drawables.push_back(std::move(ptr));
        if constexpr (std::derived_from<Tp, STEvent>)
            layouts.push_back(std::make_unique<DefaultLayout>());
    }

    template <std::derived_from<Drawable2D> Tp, typename... Args>
        requires (std::constructible_from<Tp, Args...>
            && std::derived_from<Tp, WindowBase::STEvent>)
    void WindowBase::emplaceDrawableWithLayout(
        LayoutPtr&& layout,
        Args&&... args)
    {
        auto ptr = std::make_shared<Tp>(std::forward<Args>(args)...);
        if constexpr (Event<Tp>)
            eventManager->push(ptr);
        drawables.push_back(std::move(ptr));
        layouts.push_back(std::move(layout));
    }

    template <std::derived_from<Drawable2D> Tp>
        requires (!std::derived_from<Tp, WindowBase::STEvent>)
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp> const& drawable)
    {
        if constexpr (Event<Tp>)
            eventManager->push(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable2D>(
            drawable));
    }

    template <std::derived_from<Drawable2D> Tp>
        requires std::derived_from<Tp, WindowBase::STEvent>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp> const& drawable,
        LayoutPtr layout)
    {
        if constexpr (Event<Tp>)
            eventManager->push(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable2D>(
            drawable));
        layouts.push_back(std::move(layout));
    }

    template <std::derived_from<Drawable2D> Tp>
        requires (!std::derived_from<Tp, WindowBase::STEvent>)
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp>&& drawable)
    {
        if constexpr (Event<Tp>)
            eventManager->push(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable2D>(
            std::move(drawable)));
    }

    template <std::derived_from<Drawable2D> Tp>
        requires std::derived_from<Tp, WindowBase::STEvent>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp>&& drawable,
        LayoutPtr layout)
    {
        if constexpr (Event<Tp>)
            eventManager->push(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable2D>(
            std::move(drawable)));
        layouts.push_back(std::move(layout));
    }

}

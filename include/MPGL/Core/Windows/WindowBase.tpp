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

#include <MPGL/Core/Layouts/DefaultLayout.hpp>
#include <MPGL/Core/Layouts/LayoutHolder.hpp>

namespace mpgl {

    template <Event Tp>
    void WindowBase::pushEvent(std::shared_ptr<Tp> const& event) {
        eventManager->push(event);
    }

    template <Event Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceEvent(Args&&... args) {
        eventManager->emplace(std::forward(args)...);
    }

    template <std::derived_from<Drawable2D> Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceDrawable(Args&&... args) {
        auto ptr = std::make_shared<Tp>(std::forward<Args>(args)...);
        addEventIfDerived(ptr);
        addDefaultLayoutIfTransformable(ptr);
        drawables2D.push_back(std::move(ptr));
    }

    template <std::derived_from<Drawable3D> Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void WindowBase::emplaceDrawable(Args&&... args) {
        auto ptr = std::make_shared<Tp>(std::forward<Args>(args)...);
        addEventIfDerived(ptr);
        drawables3D.push_back(std::move(ptr));
    }

    template <
        std::derived_from<Drawable2D> Tp,
        std::derived_from<Layout> Lp,
        typename... Args,
        typename... LArgs>
            requires (
                std::constructible_from<Tp, Args...>
                && std::derived_from<Tp, Transformable2D>)
    void WindowBase::emplaceDrawableWithLayout(
        LayoutTag<Lp, LArgs...>&& layout,
        Args&&... args)
    {
        auto ptr = std::make_shared<Tp>(std::forward<Args>(args)...);
        addEventIfDerived(ptr);
        addLayout(ptr, std::move(layout));
        drawables2D.push_back(std::move(ptr));
    }

    template <std::derived_from<Drawable2D> Tp>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp> const& drawable)
    {
        addEventIfDerived(drawable);
        addDefaultLayoutIfTransformable(drawable);
        drawables2D.push_back(std::static_pointer_cast<Drawable2D>(
            drawable));
    }

    template <std::derived_from<Drawable3D> Tp>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp> const& drawable)
    {
        addEventIfDerived(drawable);
        drawables3D.push_back(std::static_pointer_cast<Drawable3D>(
            drawable));
    }

    template <
        std::derived_from<Drawable2D> Tp,
        std::derived_from<Layout> Lp,
        typename... Args>
            requires std::derived_from<Tp, Transformable2D>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp> const& drawable,
        LayoutTag<Lp, Args...>&& tag)
    {
        addEventIfDerived(drawable);
        addLayout(drawable, std::move(tag));
        drawables2D.push_back(std::static_pointer_cast<Drawable2D>(
            drawable));
    }

    template <std::derived_from<Drawable2D> Tp>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp>&& drawable)
    {
        addEventIfDerived(drawable);
        addDefaultLayoutIfTransformable(drawable);
        drawables2D.push_back(std::static_pointer_cast<Drawable2D>(
            std::move(drawable)));
    }

    template <std::derived_from<Drawable3D> Tp>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp>&& drawable)
    {
        addEventIfDerived(drawable);
        drawables3D.push_back(std::static_pointer_cast<Drawable3D>(
            std::move(drawable)));
    }

    template <
        std::derived_from<Drawable2D> Tp,
        std::derived_from<Layout> Lp,
        typename... Args>
            requires std::derived_from<Tp, Transformable2D>
    void WindowBase::pushDrawable(
        std::shared_ptr<Tp>&& drawable,
        LayoutTag<Lp, Args...>&& tag)
    {
        addEventIfDerived(drawable);
        addLayout(drawable, std::move(tag));
        drawables2D.push_back(std::static_pointer_cast<Drawable2D>(
            std::move(drawable)));
    }

    template <typename Tp>
    void WindowBase::addDefaultLayoutIfTransformable(
        std::shared_ptr<Tp> const& pointer)
    {
        if constexpr (std::derived_from<Tp, Transformable2D>)
            addLayout(pointer, LayoutTag<DefaultLayout>{});
    }

    template <typename Tp, typename Lp, typename... Args>
    void WindowBase::addLayout(
        std::shared_ptr<Tp> const& pointer,
        LayoutTag<Lp, Args...>&& tag)
    {
        eventManager->push(std::shared_ptr<LayoutHolder>{
            new LayoutHolder{
            std::static_pointer_cast<Transformable2D>(pointer),
            std::move(tag)}});
    }

    template <typename Tp>
    void WindowBase::addEventIfDerived(
        std::shared_ptr<Tp> const& pointer)
    {
        if constexpr (Event<Tp>)
            eventManager->push(pointer);
    }

}

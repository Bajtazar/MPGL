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

    namespace details {

        template <Event EventTp>
        template <Event Ep>
        void EventManagerBase<EventTp>::pushIfDerived(
            std::shared_ptr<Ep> const& event)
        {
            if constexpr (std::same_as<Ep, EventTp>)
                this->push(event);
            else if constexpr (std::derived_from<Ep, EventTp>)
                this->push(std::static_pointer_cast<EventTp>(event));
        }

        template <Event Ep>
        void EventManagerBase<TickEvent>::pushIfDerived(
            std::shared_ptr<Ep> const& event)
        {
            if constexpr (std::same_as<Ep, TickEvent>)
                this->push(event);
            else if constexpr (std::derived_from<Ep, TickEvent>)
                this->push(std::static_pointer_cast<Ep>(event));
        }

    }

    template <Event... Events>
    template <Event Ep>
    void EventManager<Events...>::push(
        std::shared_ptr<Ep> const& event)
    {
        (details::EventManagerBase<Events>::pushIfDerived(event),
            ...);
    }

    template <Event... Events>
    template <Event Ep, typename... Args>
        requires std::constructible_from<Ep, Args...>
    void EventManager<Events...>::emplace(Args&&... args) {
        EventManager<Events...>::push(std::make_shared<Ep>(
            std::forward(args)...));
    }

}

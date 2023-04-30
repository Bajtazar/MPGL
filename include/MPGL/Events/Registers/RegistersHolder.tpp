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

    template <class... Registers>
        requires AllInstancesOf<RegisterInterface, Registers...>
    template <class Tp>
    void RegistersHolder<Registers...>::addIfDerived(
        std::shared_ptr<Tp> const& pointer)
    {
        std::apply([&]<typename... Args>(Args&... args)
            { (pushIfDerived(args, pointer), ...); }, registers);
    }

    template <class... Registers>
        requires AllInstancesOf<RegisterInterface, Registers...>
    template <class Tp, SpecializationOf<RegisterInterface> Register>
    void RegistersHolder<Registers...>::pushIfDerived(
        Register& reg,
        std::shared_ptr<Tp> const& pointer)
    {
        using Event = typename Register::event;

        if constexpr (std::derived_from<Tp, Event>)
            reg.pushBack(std::static_pointer_cast<Event>(pointer));
    }

}

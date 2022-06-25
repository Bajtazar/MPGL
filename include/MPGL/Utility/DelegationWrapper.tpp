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

    template <InstanceOf<std::shared_ptr>... Handles>
    template <typename... Args>
    constexpr auto
        DelegationWrapper<Handles...>::WeakPointers::weaken(
            Args... handles)
    {
        return std::tuple{std::weak_ptr{handles}...};
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
    DelegatedFunctor<Function>::DelegatedFunctor(
        DelegationWrapper&& handles,
        ArgumentsWrapper&& arguments,
        Function&& invocable)
            : handles{std::move(handles)},
            arguments{std::move(arguments)},
            invocable{std::move(invocable)} {}

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    void DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
        DelegatedFunctor<Function>::operator() (void) const
    {
        if (hasExpired())
            return;
        std::apply(invocable,
            std::tuple_cat(
                replenishSharedPointers(),
                arguments.args
            )
        );
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    bool DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
        DelegatedFunctor<Function>::hasExpired(void) const
    {
        return std::apply([](auto&&... args) {
            return (args.expired() && ...);
        }, handles.handles);
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    auto DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
        DelegatedFunctor<Function>::replenishSharedPointers(void) const
    {
        return std::apply([](auto&&... args) {
            return std::tuple{args.lock()...};
        }, handles.handles);
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
        ArgumentsWrapper(DelegationWrapper&& wrapper, Args... args)
            : wrapper{std::move(wrapper)}, args{args...} {}

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    [[nodiscard]] DelegationWrapper<Handles...>::
        ArgumentsWrapper<Args...>::DelegatedFunctor<Function>
    DelegationWrapper<Handles...>::ArgumentsWrapper<Args...>::
        operator() (Function&& func)
    {
        return DelegatedFunctor{std::move(wrapper),
            std::move(*this), std::move(func)};
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    DelegationWrapper<Handles...>::DelegationWrapper(Handles... handles)
        : handles{std::weak_ptr{handles}...} {}


    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args,
        std::invocable<Handles..., Args...> Func>
    [[nodiscard]] auto DelegationWrapper<Handles...>::operator() (
        Func&& functor,
        Args... args)
    {
        return ArgumentsWrapper<Args...>{std::move(*this),
            args...}(std::move(functor));
    }

}

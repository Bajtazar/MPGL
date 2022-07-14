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

#include <utility>

namespace mpgl {

    template <InstanceOf<std::shared_ptr>... Handles>
    template <typename... Args>
    constexpr auto
        DeferredExecutionWrapper<Handles...>::WeakPointers::weaken(
            Args... handles)
    {
        return std::tuple{std::weak_ptr{handles}...};
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    DeferredExecutionWrapper<Handles...>::InvocableArgumentsWrapper<
        Args...>::DeferredExecutor<Function>::DeferredExecutor(
            DeferredExecutionWrapper&& handles,
            InvocableArgumentsWrapper&& arguments,
            Function&& invocable)
                : handles{std::move(handles)},
                arguments{std::move(arguments)},
                invocable{std::move(invocable)} {}

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    void DeferredExecutionWrapper<Handles...
        >::InvocableArgumentsWrapper<Args...>::DeferredExecutor<
        Function>::operator() (void) const
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
    bool DeferredExecutionWrapper<Handles...
        >::InvocableArgumentsWrapper<Args...>::DeferredExecutor<
        Function>::hasExpired(void) const
    {
        return std::apply([](auto&&... args) {
            return (args.expired() && ...);
        }, handles.handles);
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    auto DeferredExecutionWrapper<Handles...
        >::InvocableArgumentsWrapper<Args...>::DeferredExecutor<
        Function>::replenishSharedPointers(void) const
    {
        return std::apply([](auto&&... args) {
            return std::tuple{args.lock()...};
        }, handles.handles);
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    DeferredExecutionWrapper<Handles...>::InvocableArgumentsWrapper<
        Args...>::InvocableArgumentsWrapper(
            DeferredExecutionWrapper&& wrapper,
            Args... args)
            : wrapper{std::move(wrapper)}, args{args...} {}

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args>
    template <std::invocable<Handles..., Args...> Function>
    [[nodiscard]] DeferredExecutionWrapper<Handles...>::
        InvocableArgumentsWrapper<Args...>::DeferredExecutor<Function>
    DeferredExecutionWrapper<Handles...>::InvocableArgumentsWrapper<
        Args...>::operator() (Function&& func)
    {
        return DeferredExecutor{std::move(wrapper),
            std::move(*this), std::move(func)};
    }

    template <InstanceOf<std::shared_ptr>... Handles>
    DeferredExecutionWrapper<Handles...>::DeferredExecutionWrapper(
        Handles&&... handles)
            : handles{std::weak_ptr{std::forward<Handles>(handles)}...}
                {}

    template <InstanceOf<std::shared_ptr>... Handles>
    template <PureType... Args,
        std::invocable<Handles..., Args...> Func>
    [[nodiscard]] auto DeferredExecutionWrapper<Handles...>::operator()(
        Func&& functor,
        Args... args)
    {
        return InvocableArgumentsWrapper<Args...>{std::move(*this),
            args...}(std::move(functor));
    }

}

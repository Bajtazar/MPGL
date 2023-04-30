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

    template <PureType Base>
    Monitor<Base>::NotifyGuard::NotifyGuard(ConditionGuard guard,
        Condition& condition, Flag& flag) noexcept
            : guard{std::move(guard)}, condition{condition}, flag{flag}
    {
        condition.wait(guard, [&flag]{ return !flag.test_and_set(); });
    }

    template <PureType Base>
    Monitor<Base>::NotifyGuard::~NotifyGuard(void) noexcept {
        guard.unlock();
        flag.clear();
        condition.notify_one();
    }

    template <PureType Base>
    Base Monitor<Base>::extract(void) const & noexcept(
        NothrowCopyConstructible<Base>)
    {
        Guard lock{mutex};
        return base;
    }

    template <PureType Base>
    Base Monitor<Base>::extract(void) && noexcept(
        NothrowMoveConstructible<Base>)
    {
        Guard lock{mutex};
        return std::move(base);
    }

    template <PureType Base>
    Monitor<Base>::Monitor(Base base) noexcept(
        NothrowMoveConstructible<Base>)
            : base{std::move(base)} {}

    template <PureType Base>
    Monitor<Base>::Monitor(Monitor const& monitor) noexcept(
        NothrowCopyConstructible<Base>)
            : base{monitor.extract()} {}

    template <PureType Base>
    Monitor<Base>::Monitor(Monitor&& monitor) noexcept(
        NothrowMoveConstructible<Base>)
            : base{std::move(monitor.extract())} {}

    template <PureType Base>
    Monitor<Base>& Monitor<Base>::operator=(
        Monitor const& monitor) noexcept(
            NothrowCopyAssignable<Base>)
    {
        std::scoped_lock {mutex, monitor.mutex};
        base = monitor.base;
        return *this;
    }

    template <PureType Base>
    Monitor<Base>& Monitor<Base>::operator=(
        Monitor&& monitor) noexcept(
            NothrowMoveAssignable<Base>)
    {
        std::scoped_lock {mutex, monitor.mutex};
        base = std::move(monitor.base);
        return *this;
    }

    template <PureType Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base&, Args...>
    std::invoke_result_t<Fn, Base&, Args...>
        Monitor<Base>::protect(Fn method, Args&&... args) & noexcept(
            std::is_nothrow_invocable_v<Fn, Base&, Args...>)
    {
        Guard lock{mutex};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

    template <PureType Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base const&, Args...>
    std::invoke_result_t<Fn, Base const&, Args...>
        Monitor<Base>::protect(Fn method, Args&&... args) const& noexcept(
            std::is_nothrow_invocable_v<Fn, Base const&, Args...>)
    {
        Guard lock{mutex};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

    template <PureType Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base&, Args...>
    std::invoke_result_t<Fn, Base&, Args...>
        Monitor<Base>::notify(Fn method, Args&&... args) noexcept(
            std::is_nothrow_invocable_v<Fn, Base&, Args...>)
    {
        Notifier notifier{condition};
        Guard lock{mutex};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

    template <PureType Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base&, Args...>
    std::invoke_result_t<Fn, Base&, Args...>
        Monitor<Base>::wait(Fn method, Args&&... args) noexcept(
            std::is_nothrow_invocable_v<Fn, Base&, Args...>)
    {
        NotifyGuard notifier{ConditionGuard{mutex}, condition, flag};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

}

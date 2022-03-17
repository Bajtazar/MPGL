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

#include "../Traits/Concepts.hpp"

#include <mutex>
#include <atomic>
#include <condition_variable>

namespace mpgl {

    /**
     * Wrapper which allows to perform given operations
     * thread safely on given base type object
     *
     * @tparam Base the type of underlying object
     */
    template <PureType Base>
    class Monitor {
    public:
        /**
         * Construct a new Monitor object
         */
        explicit Monitor(void) noexcept(
            NothrowDefaultConstructible<Base>)
                requires DefaultConstructible<Base> = default;

        /**
         * Construct a new Monitor object with given
         * arguments which are passed to underlying object
         * constructor
         *
         * @tparam Args the types of arguments
         * @param args the arguments for underlying object
         * constructor
         */
        template <typename... Args>
            requires std::constructible_from<Base, Args...>
        explicit Monitor(Args&&... args) noexcept (
            NothrowConstructible<Base, Args...>)
                : base{std::forward<Args>(args)...} {}

        /**
         * Construct a new Monitor object from the given
         * base object
         *
         * @param base the given base object
         */
        explicit Monitor(Base base) noexcept(
            NothrowMoveConstructible<Base>);

        /**
         * Construct a new Monitor object from the given
         * lvalue monitor object
         *
         * @param monitor the given lvalue monitor object
         */
        Monitor(Monitor const& monitor) noexcept(
            NothrowCopyConstructible<Base>);

        /**
         * Construct a new Monitor object from the given
         * rvalue monitor object
         *
         * @param monitor the given rvalue monitor object
         */
        Monitor(Monitor&& monitor) noexcept(
            NothrowMoveConstructible<Base>);

        /**
         * Assigns the given lvalue monitor object
         *
         * @param monitor the given lvalue monitor object
         * @return reference to this object
         */
        Monitor& operator= (Monitor const& monitor) noexcept(
            NothrowCopyAssignable<Base>);

        /**
         * Assigns the given rvalue monitor object
         *
         * @param monitor the given rvalue monitor object
         * @return reference to this object
         */
        Monitor& operator= (Monitor&& monitor) noexcept(
            NothrowMoveAssignable<Base>);

        /**
         * Invokes given invocable with base object reference
         * and passed arguments and guarantes operation thread
         * safety
         *
         * @tparam Fn the invocable type
         * @tparam Args the arguments type
         * @param method the invocable object
         * @param args the passed arguments
         * @return result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            protect(Fn method, Args&&... args) & noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        /**
         * Invokes given invocable with base object constant
         * reference and passed arguments and guarantes operation
         * thread safety
         *
         * @tparam Fn the invocable type
         * @tparam Args the arguments type
         * @param method the invocable object
         * @param args the passed arguments
         * @return result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base const&, Args...>
        std::invoke_result_t<Fn, Base const&, Args...>
            protect(Fn method, Args&&... args) const& noexcept(
                std::is_nothrow_invocable_v<Fn, Base const&, Args...>);

        /**
         * Invokes given invocable with base object reference
         * and passed arguments. Guarantes operation thread
         * safety and notifies sleeping threads
         *
         * @tparam Fn the invocable type
         * @tparam Args the arguments type
         * @param method the invocable object
         * @param args the passed arguments
         * @return result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            notify(Fn method, Args&&... args) noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        /**
         * Waits for notification from notify method and invokes
         * given invocable with base object reference and passed
         * arguments. Guarantes operation thread safety
         *
         * @tparam Fn the invocable type
         * @tparam Args the arguments type
         * @param method the invocable object
         * @param args the passed arguments
         * @return result of invocation
         */
        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            wait(Fn method, Args&&... args) noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        /**
         * Returns the copy of wrapped object
         *
         * @return the base object copy
         */
        Base extract(void) const & noexcept(
            NothrowCopyConstructible<Base>);

        /**
         * Returns the rvalue of wrapped object
         *
         * @return the base object rvalue
         */
        Base extract(void) && noexcept(
            NothrowMoveConstructible<Base>);

        /**
         * Destroy the Monitor object
         */
        ~Monitor(void) noexcept(
            NothrowDestructible<Base>) = default;
    private:
        typedef std::mutex                  Mutex;
        typedef std::lock_guard<Mutex>      Guard;
        typedef std::condition_variable     Condition;
        typedef std::unique_lock<Mutex>     ConditionGuard;
        typedef std::atomic_flag            Flag;

        /**
         * Notifies condition variable when destroyed
         */
        class Notifier {
        public:
            /**
             * Construct a new Notifier object
             *
             * @param condition the reference to condition variable
             */
            explicit Notifier(Condition& condition) noexcept
                : condition{condition} {}

            /**
             * Destroy the Notifier object and notifies
             * condition variable
             */
            ~Notifier(void) noexcept
                { condition.notify_one(); }
        private:
            Condition&                      condition;
        };

        /**
         * Waits for given condition and notifies other threads
         * uppon destruction
         */
        class NotifyGuard {
        public:
            /**
             * Construct a new Notify Guard object and waits
             * until condition is called and flag is not setted
             *
             * @param guard the lock guard reference
             * @param condition the condition variable reference
             * @param flag the atomic flag reference
             */
            explicit NotifyGuard(
                ConditionGuard guard,
                Condition& condition,
                Flag& flag) noexcept;

            /**
             * Destroy the Notify Guard object. Unlocks lock guard,
             * clears flag and notifies condition variable
             */
            ~NotifyGuard(void) noexcept;
        private:
            ConditionGuard                  guard;
            Condition&                      condition;
            Flag&                           flag;
        };

        Base                                base;
        mutable Mutex                       mutex;
        Condition                           condition;
        Flag                                flag;
    };

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

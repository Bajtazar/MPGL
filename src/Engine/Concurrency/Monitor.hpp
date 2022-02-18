#pragma once

#include "../Traits/Concepts.hpp"

#include <mutex>
#include <atomic>
#include <condition_variable>

namespace mpgl {

    template <NotReference Base>
    class Monitor {
    public:
        explicit Monitor(void) noexcept(
            NothrowDefaultConstructible<Base>)
                requires DefaultConstructible<Base> = default;

        template <typename... Args>
            requires std::constructible_from<Base, Args...>
        explicit Monitor(Args&&... args) noexcept (
            NothrowConstructible<Base, Args...>)
                : base{std::forward<Args>(args)...} {}

        explicit Monitor(Base base) noexcept(
            NothrowMoveConstructible<Base>);

        Monitor(Monitor const& monitor) noexcept(
            NothrowCopyConstructible<Base>);

        Monitor(Monitor&& monitor) noexcept(
            NothrowMoveConstructible<Base>);

        Monitor& operator= (Monitor const& monitor) noexcept(
            NothrowCopyAssignable<Base>);

        Monitor& operator= (Monitor&& monitor) noexcept(
            NothrowMoveAssignable<Base>);

        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            protect(Fn method, Args&&... args) & noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base const&, Args...>
        std::invoke_result_t<Fn, Base const&, Args...>
            protect(Fn method, Args&&... args) const& noexcept(
                std::is_nothrow_invocable_v<Fn, Base const&, Args...>);

        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            notify(Fn method, Args&&... args) noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        template <typename Fn, typename... Args>
            requires std::is_invocable_v<Fn, Base&, Args...>
        std::invoke_result_t<Fn, Base&, Args...>
            wait(Fn method, Args&&... args) noexcept(
                std::is_nothrow_invocable_v<Fn, Base&, Args...>);

        Base extract(void) const & noexcept(
            NothrowCopyConstructible<Base>);
        Base extract(void) && noexcept(
            NothrowMoveConstructible<Base>);

        ~Monitor(void) noexcept(
            NothrowDestructible<Base>) = default;
    private:
        typedef std::mutex                  Mutex;
        typedef std::lock_guard<Mutex>      Guard;
        typedef std::condition_variable     Condition;
        typedef std::unique_lock<Mutex>     ConditionGuard;
        typedef std::atomic_flag            Flag;

        class Notifier {
        public:
            explicit Notifier(Condition& condition) noexcept
                : condition{condition} {}

            ~Notifier(void) noexcept
                { condition.notify_one(); }
        private:
            Condition&                      condition;
        };

        class NotifyGuard {
        public:
            explicit NotifyGuard(
                ConditionGuard guard,
                Condition& condition,
                Flag& flag) noexcept;

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

    template <NotReference Base>
    Monitor<Base>::NotifyGuard::NotifyGuard(ConditionGuard guard,
        Condition& condition, Flag& flag) noexcept
            : guard{std::move(guard)}, condition{condition}, flag{flag}
    {
        condition.wait(guard, [&flag]{ return !flag.test_and_set(); });
    }

    template <NotReference Base>
    Monitor<Base>::NotifyGuard::~NotifyGuard(void) noexcept {
        guard.unlock();
        flag.clear();
        condition.notify_one();
    }

    template <NotReference Base>
    Base Monitor<Base>::extract(void) const & noexcept(
        NothrowCopyConstructible<Base>)
    {
        Guard lock{mutex};
        return base;
    }

    template <NotReference Base>
    Base Monitor<Base>::extract(void) && noexcept(
        NothrowMoveConstructible<Base>)
    {
        Guard lock{mutex};
        return std::move(base);
    }

    template <NotReference Base>
    Monitor<Base>::Monitor(Base base) noexcept(
        NothrowMoveConstructible<Base>)
            : base{std::move(base)} {}

    template <NotReference Base>
    Monitor<Base>::Monitor(Monitor const& monitor) noexcept(
        NothrowCopyConstructible<Base>)
            : base{monitor.extract()} {}

    template <NotReference Base>
    Monitor<Base>::Monitor(Monitor&& monitor) noexcept(
        NothrowMoveConstructible<Base>)
            : base{std::move(monitor.extract())} {}

    template <NotReference Base>
    Monitor<Base>& Monitor<Base>::operator=(
        Monitor const& monitor) noexcept(
            NothrowCopyAssignable<Base>)
    {
        std::scoped_lock {mutex, monitor.mutex};
        base = monitor.base;
        return *this;
    }

    template <NotReference Base>
    Monitor<Base>& Monitor<Base>::operator=(
        Monitor&& monitor) noexcept(
            NothrowMoveAssignable<Base>)
    {
        std::scoped_lock {mutex, monitor.mutex};
        base = std::move(monitor.base);
        return *this;
    }

    template <NotReference Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base&, Args...>
    std::invoke_result_t<Fn, Base&, Args...>
        Monitor<Base>::protect(Fn method, Args&&... args) & noexcept(
            std::is_nothrow_invocable_v<Fn, Base&, Args...>)
    {
        Guard lock{mutex};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

    template <NotReference Base>
    template <typename Fn, typename... Args>
        requires std::is_invocable_v<Fn, Base const&, Args...>
    std::invoke_result_t<Fn, Base const&, Args...>
        Monitor<Base>::protect(Fn method, Args&&... args) const& noexcept(
            std::is_nothrow_invocable_v<Fn, Base const&, Args...>)
    {
        Guard lock{mutex};
        return std::invoke(method, base, std::forward<Args>(args)...);
    }

    template <NotReference Base>
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

    template <NotReference Base>
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

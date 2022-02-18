#pragma once

#include <concepts>
#include <memory>

namespace mpgl {

    template <class Derived, class Base = Derived>
        requires std::derived_from<Derived, Base>
    struct FunctionalWrapper {
        typedef std::unique_ptr<Base>           BasePtr;

        template <typename... Args>
        constexpr static bool NothrowConstructible =
            std::is_nothrow_constructible_v<Derived, Args...>;

        FunctionalWrapper(void) noexcept = default;

        template <typename... Args>
            requires std::constructible_from<Derived, Args...>
        BasePtr operator() (Args&&... args
            ) const noexcept(NothrowConstructible<Args...>)
        {
            return std::make_unique<Derived>(
                std::forward<Args>(args)...);
        }
    };

}

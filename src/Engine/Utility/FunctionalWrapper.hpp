#pragma once

#include <concepts>
#include <memory>

namespace ge {

    template <class T, class U>
        requires std::derived_from<T, U>
    struct FunctionalWrapper {
        FunctionalWrapper(void) noexcept = default;

        template <typename... Args>
            requires std::constructible_from<T, Args...>
        std::unique_ptr<U> operator() (Args&&... args) const noexcept { return std::make_unique<T>(std::forward<Args>(args)...); }
    };

}

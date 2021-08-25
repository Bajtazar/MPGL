#pragma once

#include <type_traits>
#include <functional>
#include <concepts>
#include <utility>

namespace ge {

    template <typename T, typename... Args>
        requires std::constructible_from<T, Args...>
    class IsConstexprConstructible {
    template <class Invocable, int = (Invocable{}(), 0)>
        static constexpr auto helper(Invocable) -> std::true_type;
        static constexpr auto helper(...) -> std::false_type;
    public:
        static constexpr bool value = std::same_as<
            decltype(helper([]{ T{std::declval<Args>()...}; })), std::true_type>;
    };

    template <typename T, typename... Args>
        requires std::constructible_from<T, Args...>
    constexpr bool IsConstexprConstructibleV = IsConstexprConstructible<T, Args...>::value;

}

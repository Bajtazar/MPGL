#pragma once

#include <concepts>
#include <tuple>

namespace ge {

    struct TupleHelperFn {

        template <std::constructible_from T, std::size_t Size>
            requires (Size > 0)
        static constexpr decltype(auto) tupleConstructor(void) noexcept {
            if constexpr (Size > 1)
                return std::tuple_cat(tupleConstructor<T, Size-1>(), std::tuple<T>{});
            else
                return std::tuple<T>{};
        }

        template <std::constructible_from T, std::size_t Size, std::size_t Dimmensions>
            requires (Size > 0)
        static constexpr decltype(auto) tensorConstructor(void) noexcept {
            if constexpr (Dimmensions)
                return tupleConstructor<decltype(
                    tensorConstructor<T, Size, Dimmensions - 1>()), Size>();
            else
                return T{};
        }

    };

    template <typename... Args>
    constexpr auto tupleReverser(Args&&... args) noexcept {
        return []<std::size_t... I>(std::tuple<std::remove_reference_t<Args>...> tuple,
                std::index_sequence<I...>)
        {
            return std::tuple{std::get<sizeof...(Args) - I - 1>(tuple)...};
        }(std::tuple{std::forward<Args>(args)...}, std::make_index_sequence<sizeof...(Args)>{});
    }

    template <std::constructible_from T, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tupleConstructor<T, Size>());

    template <std::constructible_from T, std::size_t Size, std::size_t Dimmensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<T, Size, Dimmensions>());

    template <typename... Args>
    using ReversedTuple = decltype(tupleReverser<Args...>(std::declval<Args>()...));

}

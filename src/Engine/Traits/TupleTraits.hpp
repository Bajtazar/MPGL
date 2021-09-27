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

        template <typename T>
        static constexpr std::tuple<typename std::remove_reference_t<T>>
            tupleReverser(T&& arg) noexcept
        {
            return {std::forward<T>(arg)};
        }

        template <typename T, typename... Args>
        static constexpr decltype(auto) tupleReverser(T&& arg, Args&&... args) noexcept {
            return std::tuple_cat(tupleReverser<Args...>(std::forward<Args>(args)...),
                tupleReverser<T>(std::forward<T>(arg)));
        }

    };

    template <std::constructible_from T, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tupleConstructor<T, Size>());

    template <std::constructible_from T, std::size_t Size, std::size_t Dimmensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<T, Size, Dimmensions>());

    template <typename... Args>
    using ReversedTuple = decltype(TupleHelperFn::tupleReverser<Args...>(std::declval<Args>()...));

    template <typename... Args>
    constexpr auto tupleReverser(Args&&... args) noexcept {
        return TupleHelperFn::tupleReverser(std::forward<Args>(args)...);
    }

}

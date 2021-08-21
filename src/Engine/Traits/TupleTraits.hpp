#pragma once

#include <concepts>
#include <tuple>

namespace ge {

    struct TupleHelperFn {

        template <std::constructible_from T, std::size_t Size>
        static constexpr decltype(auto) tupleConstructor(void) noexcept {
            if constexpr (Size > 1)
                return std::tuple_cat(tupleConstructor<T, Size-1>(), std::tuple<T>{});
            else
                return std::tuple<T>{};
        }

        template <std::constructible_from T, std::size_t Size, std::size_t Dimmensions>
        static constexpr decltype(auto) tensorConstructor(void) noexcept {
            if constexpr (Dimmensions)
                return tupleConstructor<decltype(
                    tensorConstructor<T, Size, Dimmensions - 1>()), Size>();
            else
                return T{};
        }

    };

    template <std::constructible_from T, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tupleConstructor<T, Size>());

    template <std::constructible_from T, std::size_t Size, std::size_t Dimmensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<T, Size, Dimmensions>());

}

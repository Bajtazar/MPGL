#pragma once

#include "Concepts.hpp"

#include <tuple>

namespace mpgl {

    class  TupleHelperFn {
    private:
        template <PureType Tp, std::size_t... I>
        static constexpr auto tupleConstructor(
            std::index_sequence<I...>) noexcept
                -> decltype(std::tuple{(I, std::declval<Tp>())...});
    public:
        template <PureType Tp, std::size_t Dimm>
            requires (Dimm != 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tupleConstructor<Tp>(
                std::make_index_sequence<Dimm>{}));

        template <PureType Tp, std::size_t Dimm, std::size_t... Dimensions>
            requires (Dimm != 0 && sizeof...(Dimensions) > 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tensorConstructor<decltype(
            tensorConstructor<Tp, Dimensions...>()), Dimm>());
    };

    template <typename... Args>
    constexpr auto tupleReverser(Args&&... args) noexcept {
        return []<std::size_t... I>(std::tuple<std::remove_reference_t<Args>...> tuple,
                std::index_sequence<I...>)
        {
            return std::tuple{std::get<sizeof...(Args) - I - 1>(tuple)...};
        }(std::tuple{std::forward<Args>(args)...}, std::make_index_sequence<sizeof...(Args)>{});
    }

    template <PureType T, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tensorConstructor<T, Size>());

    template <PureType T, std::size_t... Dimensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<T, Dimensions...>());

    template <typename... Args>
    using ReversedTuple = decltype(tupleReverser<Args...>(std::declval<Args>()...));

}

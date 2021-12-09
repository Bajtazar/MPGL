#pragma once

#include "Concepts.hpp"

#include <tuple>

namespace ge {

    class  TupleHelperFn {
    private:
        template <NotReference Tp, std::size_t... I>
        static constexpr auto tupleConstructor(
            std::index_sequence<I...>) noexcept
                -> decltype(std::tuple{(I, std::declval<Tp>())...});
    public:
        template <NotReference Tp, std::size_t Dimm>
            requires (Dimm != 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tupleConstructor<Tp>(
                std::make_index_sequence<Dimm>{}));

        template <NotReference Tp, std::size_t Dimm, std::size_t... Dimmensions>
            requires (Dimm != 0 && sizeof...(Dimmensions) > 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tensorConstructor<decltype(
            tensorConstructor<Tp, Dimmensions...>()), Dimm>());
    };

    template <typename... Args>
    constexpr auto tupleReverser(Args&&... args) noexcept {
        return []<std::size_t... I>(std::tuple<std::remove_reference_t<Args>...> tuple,
                std::index_sequence<I...>)
        {
            return std::tuple{std::get<sizeof...(Args) - I - 1>(tuple)...};
        }(std::tuple{std::forward<Args>(args)...}, std::make_index_sequence<sizeof...(Args)>{});
    }

    template <NotReference T, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tensorConstructor<T, Size>());

    template <NotReference T, std::size_t... Dimmensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<T, Dimmensions...>());

    template <typename... Args>
    using ReversedTuple = decltype(tupleReverser<Args...>(std::declval<Args>()...));

}

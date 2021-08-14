#pragma once

#include <concepts>
#include <ranges>

namespace ge {

    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <typename T, typename... Args>
    concept AllSame = std::conjunction_v<std::is_same<T, Args>...>;

    template <typename T>
    concept Constant = std::is_const_v<std::remove_reference_t<T>>;

    template <typename T>
    concept NotConstant = !Constant<T>;

    template <typename T>
    concept SizeType = std::is_unsigned_v<T> && Arithmetic<T>;

    template <typename T>
    concept Addable = requires (T a) { a + a; };

    template <typename T, typename U>
    concept SameSize = (sizeof(T) == sizeof(U));

    template <typename T, typename U>
    concept NotSameSize = !SameSize<T, U>;

    template <class Range, typename T>
    concept ForwardConvertible = std::ranges::forward_range<Range>
        && std::convertible_to<std::ranges::range_value_t<Range>, T>;

    template <class Range, typename T>
    concept RandomAccessConvertible = std::ranges::random_access_range<Range>
        && std::convertible_to<std::ranges::range_value_t<Range>, T>;

    template <class Iter>
    concept SameSizeInputIterator = std::input_iterator<Iter>
        && requires (Iter iter) { {*iter} -> SameSize<std::byte>; };

    template <class Iter, typename... Args>
    concept NothrowReadable = std::indirectly_readable<std::remove_cvref_t<Iter>> &&
        std::is_nothrow_invocable_v<decltype(&std::remove_cvref_t<Iter>::operator*),
            std::remove_cvref_t<Iter>, Args...>;

    template <std::integral T, T Size>
    constexpr T log2N(void) noexcept {
        return Size == 1 ? 0 : 1 + log2N<T, (Size >> 1)>();
    }

    template <typename T, T Size>
    concept IsPowerOf2 = std::integral<T> && (Size == (1 << log2N<T, Size>()));

}

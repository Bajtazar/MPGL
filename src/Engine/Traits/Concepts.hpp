#pragma once

#include "Traits.hpp"

#include <ranges>

namespace ge {

    template <typename T>
    concept Arithmetic = std::constructible_from<T>
        && requires (T left, T right)
        {
            {left + right} -> std::convertible_to<T>;
            {left - right} -> std::convertible_to<T>;
            {left * right} -> std::convertible_to<T>;
            {left / right} -> std::convertible_to<T>;
        }
        && requires (std::remove_cv_t<T> number)
        {
            {number += number} -> std::same_as<std::remove_cv_t<T>&>;
            {number -= number} -> std::same_as<std::remove_cv_t<T>&>;
        };

    template <typename T>
    concept AbsolutelyArithmetic = Arithmetic<std::remove_cvref_t<T>>;

    template <typename T, typename... Args>
    concept AllSame = std::conjunction_v<std::is_same<T, Args>...>;

    template <typename... Args>
    concept AllIntegrals = (std::integral<Args> && ...);

    template <typename... Args>
    concept AllUnsignedIntegrals = (std::unsigned_integral<Args> && ...);

    template <typename... Args>
    concept AllFloatingPoints = (std::floating_point<Args> && ...);

    template <typename T>
    concept Absolute = std::same_as<T, std::remove_cvref_t<T>>;

    template <typename T, typename... Args>
    concept AllAbsolutelySame = AllSame<T, std::remove_cvref_t<Args>...>;

    template <typename T, typename... Args>
    concept AllConvertible = std::conjunction_v<std::is_convertible<Args, T>...>;

    template <class T, class... Args>
    concept AllDerives = std::conjunction_v<std::is_base_of<T, Args>...>;

    template <typename T, class... Args>
    concept Included = std::disjunction_v<std::is_same<T, Args>...>;

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
    concept ByteInputIterator = std::input_iterator<Iter>
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

    template <class T>
    concept Allocator = std::default_initializable<T> && std::copy_constructible<T> &&
        std::same_as<std::remove_reference_t<typename T::value_type>, typename T::value_type>
        && std::copyable<T> && requires {
            typename std::allocator_traits<T>::value_type;
            typename std::allocator_traits<T>::size_type;
            typename std::allocator_traits<T>::difference_type;
        } &&
        requires (typename T::value_type* ptr, typename T::size_type size, T alloc) {
            { alloc.allocate(size) } -> std::same_as<typename T::value_type*>;
            { alloc.deallocate(ptr, size) } -> std::same_as<void>;
        };

    template <typename T>
    concept NotReference = !std::is_reference_v<T>;

    template <typename T>
    concept Adaptable = requires(T& a, typename T::value_type b) { a / a; a * a; a * b; a / b; a - b; a + b; }
        && std::is_convertible_v<uint32_t, typename T::value_type>;

    template <typename T, typename... Args>
    concept ConstexprConstructible = IsConstexprConstructibleV<T, Args...>;

    template <class Range>
    concept FlexibleRange = std::ranges::random_access_range<Range>
        && requires(Range range, std::size_t size,
            typename Range::value_type value)
        {
            range.reserve(size),
            range.resize(size, value);
        };

    template <typename T>
    concept DefaultBaseType = Absolute<T> && std::is_default_constructible_v<T>;

    template <class Range, typename Base>
    concept UnderlyingRange = FlexibleRange<Range>
        && std::same_as<std::ranges::range_value_t<Range>, Base>;

    template <template<class, class> class Range,
        typename Type, typename Alloc = std::allocator<Type>>
    concept UnderlyiesType = Allocator<Alloc> &&
        UnderlyingRange<Range<Type, Alloc>, Type>;

    template <template<class, class> class Range, typename... Types>
    concept UnderlyiesTypes = (UnderlyiesType<Range, Types> && ...);

    template <typename Signature, auto Method>
    concept SameSignatures = std::same_as<Signature, decltype(Method)>;

    template <class... Classes>
    concept NothrowCopyConstructible = (
        std::is_nothrow_copy_constructible_v<Classes> &&
        ...
    );

    template <class... Classes>
    concept NothrowMoveConstructible = (
        std::is_nothrow_move_constructible_v<Classes> &&
        ...
    );

    template <class... Classes>
    concept NothrowCopyAssignable = (
        std::is_nothrow_copy_assignable_v<Classes> &&
        ...
    );

    template <class... Classes>
    concept NothrowMoveAssignable = (
        std::is_nothrow_move_assignable_v<Classes> &&
        ...
    );

}

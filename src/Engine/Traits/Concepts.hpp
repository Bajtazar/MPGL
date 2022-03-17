/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include "Traits.hpp"

#include <ranges>

namespace mpgl {

    /**
     * Defines macro factory which creates requires statements to
     * detect whether the given operation is allowed on the given type
     */
    #ifndef mpgl_Operable
    #define mpgl_Operable(Tp, Operator) requires ( Tp a, Tp b ) { \
        { a Operator b } -> std::convertible_to< Tp >; \
    }
    #endif

    /**
     * Checks whether the given type can be used in the basic
     * arithmetic operations
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept Arithmetic =
        requires (Tp left, Tp right)
        {
            {left + right} -> std::convertible_to<Tp>;
            {left - right} -> std::convertible_to<Tp>;
            {left * right} -> std::convertible_to<Tp>;
            {left / right} -> std::convertible_to<Tp>;
        }
        && requires (std::remove_cv_t<Tp> number)
        {
            {number += number} -> std::same_as<std::remove_cv_t<Tp>&>;
            {number -= number} -> std::same_as<std::remove_cv_t<Tp>&>;
        };

    /**
     * Checks whether an absolute type of the given type is arithmetic
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept AbsolutelyArithmetic = Arithmetic<std::remove_cvref_t<Tp>>;

    /**
     * Checks whether all of the given types are same
     *
     * @tparam Tp the first type
     * @tparam Args the rest of types
     */
    template <typename Tp, typename... Args>
    concept AllSame = std::conjunction_v<std::is_same<Tp, Args>...>;

    /**
     * Checks whether all of the given types are integers
     *
     * @tparam Args the types to check
     */
    template <typename... Args>
    concept AllIntegrals = (std::integral<Args> && ...);

    /**
     * Checks whether all of the given types are unsigned integers
     *
     * @tparam Args the types to check
     */
    template <typename... Args>
    concept AllUnsignedIntegrals = (std::unsigned_integral<Args> && ...);

    /**
     * Checks whether all of the given types are floating points
     *
     * @tparam Args the types to check
     */
    template <typename... Args>
    concept AllFloatingPoints = (std::floating_point<Args> && ...);

    /**
     * Checks whether the given type is not a cv reference
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept Absolute = std::same_as<Tp, std::remove_cvref_t<Tp>>;

    /**
     * Checks whether all of the given types are absolutely the
     * same tyoe
     *
     * @tparam Tp the first type
     * @tparam Args the rest of types
     */
    template <typename Tp, typename... Args>
    concept AllAbsolutelySame = AllSame<std::remove_cvref_t<Tp>,
        std::remove_cvref_t<Args>...>;

    /**
     * Checks whether all of the given types can be converted
     * to the desire one
     *
     * @tparam Tp the desired type
     * @tparam Args the checked types
     */
    template <typename Tp, typename... Args>
    concept AllConvertible = std::conjunction_v<
        std::is_convertible<Args, Tp>...>;

    /**
     * Checks whether all of the given types derives from a base type
     *
     * @tparam Tp the base type
     * @tparam Args the checked types
     */
    template <class Tp, class... Args>
    concept AllDerives = std::conjunction_v<
        std::is_base_of<Tp, Args>...>;

    /**
     * Checks whether the given type is included in the parameter pack
     *
     * @tparam Tp the checked  type
     * @tparam Args the parameter pack
     */
    template <typename Tp, class... Args>
    concept Included = std::disjunction_v<std::is_same<Tp, Args>...>;

    /**
     * Checks whether the given type can be used as a size_type
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept SizeType = std::is_unsigned_v<Tp> && Arithmetic<Tp>;

    /**
     * Checks whether the given type can be added to itself
     *
     * @tparam Tp the given type
     */
    template <typename Tp>
    concept Addable = requires (Tp a) { a + a; };

    /**
     * Checks whether two types takes the same ammount of space
     * in the memory
     *
     * @tparam Tp the first type
     * @tparam Up the second type
     */
    template <typename Tp, typename Up>
    concept SameSize = (sizeof(Tp) == sizeof(Up));

    /**
     * Checks whether two types takes different ammount of space
     * in the memory
     *
     * @tparam Tp the first type
     * @tparam Up the second type
     */
    template <typename Tp, typename Up>
    concept NotSameSize = !SameSize<Tp, Up>;

    /**
     * Checks whether the given range is a forward range and its
     * value can be converted to the given type
     *
     * @tparam Range the range type
     * @tparam Tp the checked type
     */
    template <class Range, typename Tp>
    concept ForwardConvertible = std::ranges::forward_range<Range>
        && std::convertible_to<std::ranges::range_value_t<Range>, Tp>;

    /**
     * Checks whether the given range is a random access range and its
     * value can be converted to the given type
     *
     * @tparam Range the range type
     * @tparam Tp the checked type
     */
    template <class Range, typename Tp>
    concept RandomAccessConvertible = std::ranges::random_access_range<
        Range> && std::convertible_to<
            std::ranges::range_value_t<Range>, Tp>;

    /**
     * Checks whether the given iterator returns a byte-like type
     *
     * @tparam Iter the checked iterator
     */
    template <class Iter>
    concept ByteInputIterator = std::input_iterator<Iter>
        && requires (Iter iter) { {*iter} -> SameSize<std::byte>; };

    /**
     * Checks whether the given iterator can be read without
     * throwing an exception
     *
     * @tparam Iter the iterator type
     */
    template <class Iter>
    concept NothrowReadable = std::indirectly_readable<
        std::remove_cvref_t<Iter>> &&
        std::is_nothrow_invocable_v<
            decltype(&std::remove_cvref_t<Iter>::operator*),
                std::remove_cvref_t<Iter>>;

    /**
     * Returns the constexpr result of a binary logarithm
     *
     * @tparam Tp the integer type
     * @tparam Size the number to logarithm
     * @return the binary logarithm of the Number
     */
    template <std::integral Tp, Tp Number>
    constexpr Tp log2N(void) noexcept {
        return Number == 1 ? 0 : 1 + log2N<Tp, (Number >> 1)>();
    }

    /**
     * Checks whether the given number is a power of 2
     *
     * @tparam Tp the integer type
     * @tparam Size the number to check
     */
    template <typename Tp, Tp Number>
    concept IsPowerOf2 = std::integral<Tp> && (
        Number == (1 << log2N<Tp, Number>()));

    /**
     * Checks whether the given type is not a reference
     *
     * @tparam Tp the type to check
     */
    template <typename Tp>
    concept PureType = !std::is_reference_v<Tp>;

    /**
     * Checks whether the given type can be used inside an adapter
     *
     * @tparam Tp the type to check
     */
    template <typename Tp>
    concept Adaptable = Absolute<Tp> &&
        requires { typename Tp::value_type; } &&
        requires(Tp& a, typename Tp::value_type b)
            { a / a; a * a; a * b; a / b; a - b; a + b; }
        && std::is_convertible_v<uint32, typename Tp::value_type>;

    /**
     * Checks whether the given type is a constexpr constructible
     *
     * @tparam Tp the type to check
     * @tparam Args the constructor arguments types
     */
    template <typename Tp, typename... Args>
    concept ConstexprConstructible =
        IsConstexprConstructibleV<Tp, Args...>;

    /**
     * Checks whether the given range is a random acces range
     * and whether it can be resized and it can reserve space
     *
     * @tparam Range the range to check
     */
    template <class Range>
    concept FlexibleRange = std::ranges::random_access_range<Range>
        && requires(Range range, std::size_t size,
            typename Range::value_type value)
        {
            range.reserve(size),
            range.resize(size, value);
        };

    /**
     * Checks whether the given type is absolute and it has
     * default constructor
     *
     * @tparam Tp the checked type
     */
    template <typename Tp>
    concept DefaultBaseType = Absolute<Tp>
        &&  std::is_default_constructible_v<Tp>;

    /**
     * Checks whether the given range value type is same as the
     * given type
     *
     * @tparam Range the range type
     * @tparam Tp the underlying type
     */
    template <class Range, typename Tp>
    concept SameRangeType = std::same_as<
        std::ranges::range_value_t<Range>, Tp>;

    /**
     * Checks whether the given iterator value type is same as the
     * given type
     *
     * @tparam Iter the iterator tyoe
     * @tparam Tp the underlying type
     */
    template <class Iter, typename Tp>
    concept SameIterType = std::same_as<
        std::iter_value_t<Iter>, Tp>;

    /**
     * Checks whether the given range can be used to proxy a data
     * and its flexible
     *
     * @tparam Range the range type
     * @tparam Base the underlying type
     */
    template <class Range, typename Base>
    concept UnderlyingRange = FlexibleRange<Range>
        && SameRangeType<Range, Base>;

    /**
     * Checks whether the given method match desired signature
     *
     * @tparam Signature the signature type
     * @tparam Method the method pointer
     */
    template <typename Signature, auto Method>
    concept SameSignatures = std::same_as<Signature, decltype(Method)>;

    /**
     * Checks whether the given classes are nothrow default
     * constructible
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowDefaultConstructible = (
        std::is_nothrow_default_constructible_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are nothrow copy
     * constructible
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowCopyConstructible = (
        std::is_nothrow_copy_constructible_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are nothrow move
     * constructible
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowMoveConstructible = (
        std::is_nothrow_move_constructible_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are nothrow copy
     * assignable
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowCopyAssignable = (
        std::is_nothrow_copy_assignable_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are nothrow move
     * assignable
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowMoveAssignable = (
        std::is_nothrow_move_assignable_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are nothrow destructible
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept NothrowDestructible = (
        std::is_nothrow_destructible_v<Classes> && ...
    );

    /**
     * Checks whether the given classes are default constructible
     *
     * @tparam Classes the classes types
     */
    template <class... Classes>
    concept DefaultConstructible = (
        std::is_default_constructible_v<Classes> && ...
    );

    /**
     * Checks whether the given type is nothrow constructible
     *
     * @tparam Tp the checked type
     * @tparam Args the type constructor's arguments types
     */
    template <class Tp, typename... Args>
    concept NothrowConstructible =
        std::is_nothrow_constructible_v<Tp, Args...>;

    /**
     * Checks whether the given range is a random access range
     * and its size is equal to desired size
     *
     * @tparam Range the range type
     * @tparam Tp the size integer
     * @tparam Size the desired range size
     */
    template <class Range, typename Tp, std::size_t Size>
    concept SizedRange = std::ranges::random_access_range<Range> &&
        SameRangeType<Range, Tp> &&
        requires { { Range::size() } -> std::convertible_to<std::size_t>;  }
        && Range::size() == Size;

    /**
     * Checks whether the given type is an allocator of a given type
     *
     * @tparam Alloc the checked type
     * @tparam Tp the allocator's base type
     */
    template <class Alloc, typename Tp>
    concept Allocator = PureType<Tp> && requires {
        typename Alloc::value_type;
        typename Alloc::size_type;
    } && std::same_as<typename Alloc::value_type, Tp>
      && std::integral<typename Alloc::size_type>
      && requires (
            Alloc alloc,
            Alloc ralloc,
            typename Alloc::size_type size,
            Tp* pointer)
    {
        { alloc.allocate(size) } -> std::same_as<Tp*>;
        alloc.deallocate(pointer, size);
        { alloc == ralloc } -> std::same_as<bool>;
        { alloc != ralloc } -> std::same_as<bool>;
    } && std::copy_constructible<Alloc>
     && NothrowCopyConstructible<Alloc>
     && std::is_copy_assignable_v<Alloc>
     && NothrowCopyAssignable<Alloc>
     && std::move_constructible<Alloc>
     && NothrowMoveConstructible<Alloc>
     && std::is_move_assignable_v<Alloc>
     && NothrowMoveAssignable<Alloc>;

    /**
     * Checks whether the given type is bindable
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept Bindable = PureType<Tp> && requires(Tp const& type) {
        type.bind();
        { type.unbind() } -> std::same_as<void>;
    };

    /**
     * Checks whether the given type is return bindable
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept ReturnBindable = Bindable<Tp> && requires(Tp const& type) {
        { type.bind() } -> std::move_constructible;
    };

    /**
     * Checks whether the given type is non-return bindable
     *
     * @tparam Tp the checked type
     */
    template <class Tp>
    concept NonReturnBindable = Bindable<Tp>
        && requires(Tp const& type)
    {
        { type.bind() } -> std::same_as<void>;
    };

}

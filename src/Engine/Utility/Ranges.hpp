#pragma once

#include <ranges>
#include <iterator>
#include <functional>

#include "../Traits/Concepts.hpp"

namespace ge {

    struct AccumulateFn {

        template <std::incrementable Iter, std::sentinel_for<Iter> Sent, Addable Init, typename Proj = std::identity>
            requires std::convertible_to<typename std::iterator_traits<Iter>::value_type, Init>
        constexpr Init operator() (Iter iter, Sent sent, Init init, Proj proj = {}) const noexcept {
            for (; iter != sent; ++iter)
                init = std::move(init) + std::invoke(proj, *iter);
            return init;
        }

        template <std::incrementable Iter, std::sentinel_for<Iter> Sent, typename Init, typename Proj = std::identity,
                std::indirect_binary_predicate<Init, std::projected<Iter, Proj>> Pred>
            requires std::convertible_to<typename std::iterator_traits<Iter>::value_type, Init>
                && std::is_nothrow_invocable_v<Pred, Init, std::projected<Iter, Proj>>
        constexpr Init operator() (Iter iter, Sent sent, Init init, Pred pred, Proj proj = {}) const noexcept {
            for (; iter != sent; ++iter)
                init = std::invoke(proj, std::move(init), std::invoke(proj, *iter));
            return init;
        }

        template <std::ranges::input_range Range, Addable Init, typename Proj = std::identity>
            requires std::convertible_to<typename std::iterator_traits<std::ranges::iterator_t<Range>>::value_type, Init>
        constexpr Init operator() (Range&& range, Init init, Proj proj = {}) const noexcept {
            return (*this)(std::ranges::begin(range), std::ranges::end(range), std::move(init), std::move(proj));
        }

        template <std::ranges::input_range Range, typename Init, typename Proj = std::identity,
                std::indirect_binary_predicate<Init, std::projected<std::ranges::iterator_t<Range>, Proj>> Pred>
            requires std::convertible_to<typename std::iterator_traits<std::ranges::iterator_t<Range>>::value_type, Init>
                && std::is_nothrow_invocable_v<Pred, Init, std::projected<std::ranges::iterator_t<Range>, Proj>>
        constexpr Init operator() (Range&& range, Init init, Pred pred, Proj proj = {}) const noexcept {
            return (*this)(std::ranges::begin(range), std::ranges::end(range), std::move(init), std::move(pred), std::move(proj));
        }

    };

    inline constexpr AccumulateFn accumulate{};

    template <typename T>
    struct FindFirstAndLastResult {
        [[no_unique_address]] T first;
        [[no_unique_address]] T last;

        template <typename U>
            requires std::convertible_to<const T&, U>
        constexpr operator FindFirstAndLastResult<U>() const & { return {first, last}; }

        template <typename U>
            requires std::convertible_to<T, U>
        constexpr operator FindFirstAndLastResult<U>() && { return {std::move(first), std::move(last)}; }
    };

    struct FindFirstAndLastIfFn {

        template <std::forward_iterator Iter, std::sentinel_for<Iter> Sent, typename Proj = std::identity,
            std::indirect_unary_predicate<std::projected<Iter, Proj>> Pred>
        constexpr FindFirstAndLastResult<Iter> operator() (Iter iter, Sent sent, Pred pred, Proj proj = {}) const noexcept {
            auto first = iter, last = sent;
            if (iter == sent)
                return {first, last};
            for (; !std::invoke(pred, std::invoke(proj, *iter)) && iter != sent; ++iter);
            first = iter;
            if (iter == sent)
                return {first, first};
            --sent;
            for (; !std::invoke(pred, std::invoke(proj, *sent)) && iter != sent; --sent);
            last = iter != sent ? sent : first;
            return {first, last};
        }

        template <std::ranges::forward_range Range, typename Proj = std::identity,
            std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<Range>, Proj>> Pred>
        constexpr FindFirstAndLastResult<std::ranges::borrowed_iterator_t<Range>> operator()
            (Range&& range, Pred pred, Proj proj = {}) const noexcept
        {
            return (*this)(std::ranges::begin(range), std::ranges::end(range), std::move(pred), std::move(proj));
        }

    };

    inline constexpr FindFirstAndLastIfFn findFirstAndLastIf;

    struct CopyToFn {

        template <std::incrementable Iter, std::sentinel_for<Iter> Sent,
            std::weakly_incrementable TargetIter, typename Proj = std::identity>
        constexpr void operator() (Iter iter, Sent sent, TargetIter target, Proj proj = {}) const noexcept {
            for (; iter != sent; ++iter, ++target)
                *target = std::move(std::invoke(proj, *iter));
        }

        template <std::ranges::input_range Range, std::incrementable TargetIter, typename Proj = std::identity>
        constexpr void operator() (Range&& range, TargetIter iter, Proj proj = {}) const noexcept {
            (*this)(std::ranges::begin(range), std::ranges::end(range), std::move(iter), std::move(proj));
        }

    };

    inline constexpr CopyToFn copyTo;

    struct ReverseFn {

        template <std::bidirectional_iterator Iter, std::sentinel_for<Iter> Sent>
        constexpr void operator() (Iter iter, Sent sent) const noexcept {
            if (iter == sent)
                return;
            --sent;
            for (; iter < sent; ++iter, --sent)
                std::iter_swap(iter, sent);
        }

        template <std::ranges::bidirectional_range Range>
        constexpr void operator() (Range&& range) const noexcept {
            (*this)(std::ranges::begin(range), std::ranges::end(range));
        }

    };

    inline constexpr ReverseFn reverse;

}
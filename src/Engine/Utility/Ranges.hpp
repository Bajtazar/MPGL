#pragma once

#include <ranges>
#include <iterator>
#include <functional>

namespace ge {

    template <typename T>
    concept Addable = requires (T a) { a + a; };

    struct accumulateFn {

        template <std::weakly_incrementable Iter, std::sentinel_for<Iter> Sent, Addable Init, typename Proj = std::identity>
            requires std::convertible_to<typename std::iterator_traits<Iter>::value_type, Init>
        constexpr Init operator() (Iter iter, Sent sent, Init init, Proj proj = {}) const noexcept {
            for (; iter != sent; ++iter)
                init = std::move(init) + std::invoke(proj, *iter);
            return init;
        }

        template <std::weakly_incrementable Iter, std::sentinel_for<Iter> Sent, typename Init, typename Proj = std::identity,
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

    inline constexpr accumulateFn accumulate{};

}
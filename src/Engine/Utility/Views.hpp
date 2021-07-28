#pragma once

#include <ranges>
#include <iterator>

namespace ge {

    template <std::ranges::forward_range Range>
        requires std::ranges::view<Range>
    class SkippableView : public std::ranges::view_interface<SkippableView<Range>> {
    public:
        typedef std::ranges::range_value_t<Range>    value_type;

        constexpr explicit SkippableView(void) noexcept = default;
        constexpr explicit SkippableView(Range range, value_type skippable) noexcept : range{range}, skippable{skippable} {}

        struct sentinel {
            constexpr sentinel(void) noexcept = default;
        };

        class iterator : public std::iterator<std::forward_iterator_tag, value_type> {
        public:
            typedef std::ranges::iterator_t<Range>  Iter;
            typedef SkippableView::value_type       value_type;
            typedef value_type&                     reference;
            typedef std::forward_iterator_tag       iterator_category;
            typedef std::ptrdiff_t                  difference_type;

            explicit iterator(void) noexcept = default;
            explicit iterator(const Iter& iter, const Iter& end, value_type skippable) noexcept : iter{iter}, end{end}, skippable{skippable} {}

            iterator& operator++(void) noexcept {
                ++iter;
                while (*iter == skippable && iter != end)
                    ++iter;
                return *this;
            }
            iterator operator++(int) noexcept { auto tmp = *this; ++(*this); return tmp; }
            iterator& operator--(void) noexcept {
                --iter;
                while (*iter == skippable && iter != end)
                    --iter;
                return *this;
            }
            iterator operator--(int) noexcept { auto tmp = *this; --(*this); return tmp; }
            friend bool operator==(const iterator& left, [[maybe_unused]] const sentinel& right) noexcept { return left.iter == left.end; }

            auto& operator*(void) const noexcept {return *iter; }
        private:
            Iter iter;
            Iter end;
            value_type skippable;
        };

        constexpr const Range& base(void) const { return range; }
        constexpr Range&& base(void) { return std::move(range); }
        constexpr value_type getSkippable(void) const noexcept { return skippable; }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(range), std::ranges::end(range), skippable }; }
        constexpr sentinel end(void) const { return sentinel{}; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const Range> { return std::ranges::size(range); }

    private:
        Range range;
        value_type skippable;
    };

    template <class Range>
    SkippableView(Range&& range) -> SkippableView<std::views::all_t<Range>>;

    namespace details {

        template <typename Tp>
        struct SkippableViewRangeAdaptorClosure {
            Tp skippable;

            constexpr SkippableViewRangeAdaptorClosure(Tp skippable) noexcept : skippable{skippable} {}

            template <std::ranges::viewable_range Range>
            constexpr auto operator() (Range&& range) const noexcept {
                return SkippableView{ std::forward<Range>(range), skippable };
            }

        };


        struct SkippableViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
            constexpr auto operator() (Range&& range, std::ranges::range_value_t<Range> skippable) const noexcept {
                return SkippableView{ std::forward<Range>(range), skippable };
            }

            template <typename Tp>
            constexpr auto operator() (Tp skippable) const noexcept {
                return SkippableViewRangeAdaptorClosure<Tp>{ skippable };
            }
        };

        template <std::ranges::viewable_range Range, typename Tp>
            requires std::is_convertible_v<Tp, std::ranges::range_value_t<Range>>
        constexpr auto operator | (Range&& range, const SkippableViewRangeAdaptorClosure<Tp>& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const SkippableViewRangeAdaptor& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

    }

    namespace views {
        inline constexpr details::SkippableViewRangeAdaptor skippableView;
    }

}

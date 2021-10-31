#pragma once

#include <iterator>
#include <ranges>

namespace ge {

    template <class T>
    using position_t = decltype(std::declval<std::ranges::range_value_t<T>>().position);

    template <class T>
    using color_t = decltype(std::declval<std::ranges::range_value_t<T>>().color);

    template <class T>
    using coords_t = decltype(std::declval<std::ranges::range_value_t<T>>().textureCoords);

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range> &&
            requires (std::ranges::range_value_t<Range> vertex) { vertex.position; })
    class PositionView : public std::ranges::view_interface<PositionView<Range>> {
    public:
        constexpr explicit PositionView(void) noexcept = default;
        constexpr explicit PositionView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, position_t<Range>> {
        public:
            using inner_iter =                  std::ranges::iterator_t<Range>;
            using value_type =                  position_t<Range>;
            using reference =                   value_type&;
            using iterator_category =           std::input_iterator_tag;
            using difference_type =             std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(inner_iter const& iter) noexcept
                : iter{iter} {}

            iterator& operator++(void) noexcept
                { ++iter; return *this; }
            iterator operator++(int) noexcept
                { auto tmp = *this; ++iter; return tmp; }
            friend bool operator== (iterator const& left, iterator const& right) noexcept
                { return left.iter == right.iter; }

            auto& operator*(void) const noexcept
                {return iter->position; }
        private:
            inner_iter                          iter;
        };

        constexpr Range const& base(void) const
            { return base_; }
        constexpr Range&& base(void)
            { return std::move(base_); }

        constexpr iterator begin(void) const
            { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const
            { return iterator{ std::ranges::end(base_) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<Range const>
            { return std::ranges::size(base_); }
    private:
        Range                                   base_;
    };

    template <class Range>
    PositionView(Range&& base) -> PositionView<std::views::all_t<Range>>;

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range>
            && requires (std::ranges::range_value_t<Range> vertex) { vertex.color; })
    class ColorView : public std::ranges::view_interface<ColorView<Range>> {
    public:
        constexpr explicit ColorView(void) noexcept = default;
        constexpr explicit ColorView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, color_t<Range>> {
        public:
            using inner_iter =                  std::ranges::iterator_t<Range>;
            using value_type =                  color_t<Range>;
            using reference =                   value_type&;
            using iterator_category =           std::input_iterator_tag;
            using difference_type =             std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(inner_iter const& iter) noexcept : iter{iter} {}

            iterator& operator++(void) noexcept
                { ++iter; return *this; }
            iterator operator++(int) noexcept
                { auto tmp = *this; ++iter; return tmp; }
            friend bool operator==(iterator const& left, iterator const& right) noexcept
                { return left.iter == right.iter; }

            auto& operator*(void) const noexcept
                {return iter->color; }
        private:
            inner_iter                          iter;
        };

        constexpr const Range& base(void) const
            { return base_; }
        constexpr Range&& base(void)
            { return std::move(base_); }

        constexpr iterator begin(void) const
            { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const
            { return iterator{ std::ranges::end(base_) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<Range const>
            { return std::ranges::size(base_); }
    private:
        Range                                   base_;
    };

    template <class Range>
    ColorView(Range&& base) -> ColorView<std::views::all_t<Range>>;

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range>
            && requires (std::ranges::range_value_t<Range> vertex) { vertex.textureCoords; })
    class TextureCoordsView : public std::ranges::view_interface<TextureCoordsView<Range>> {
    public:
        constexpr explicit TextureCoordsView(void) noexcept = default;
        constexpr explicit TextureCoordsView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, coords_t<Range>> {
            using inner_iter =                  std::ranges::iterator_t<Range>;
            using value_type =                  coords_t<Range>;
            using reference =                   value_type&;
            using iterator_category =           std::input_iterator_tag;
            using difference_type =             std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(inner_iter const& iter) noexcept : iter{iter} {}

            iterator& operator++(void) noexcept
                { ++iter; return *this; }
            iterator operator++(int) noexcept
                { auto tmp = *this; ++iter; return *this; }

            friend bool operator== (iterator const& left, iterator const& right) noexcept
                { return left.iter == right.iter; }

            auto& operator*(void) const noexcept
                { return iter->textureCoords; }
        private:
            inner_iter                          iter;
        };

        constexpr const Range& base(void) const
            { return base_; }
        constexpr Range&& base(void)
            { return std::move(base_); }

        constexpr iterator begin(void) const
            { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const
            { return iterator{ std::ranges::end(base_) }; }

        constexpr auto size(void) const noexcept requires std::ranges::sized_range<Range const>
            { return std::ranges::size(base_); }
    private:
        Range                                   base_;
    };

    template <class Range>
    TextureCoordsView(Range&& base) -> TextureCoordsView<std::views::all_t<Range>>;

    namespace details {

        struct PositionViewRangeAdaptorClosure {
            constexpr PositionViewRangeAdaptorClosure(void) noexcept = default;

            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.position; }
            constexpr auto operator() (Range&& range) const noexcept {
                return PositionView{ std::forward<Range>(range) };
            }
        };

        struct PositionViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.position; }
            constexpr auto operator() (Range&& range) const noexcept {
                return PositionView{ std::forward<Range>(range) };
            }

            constexpr auto operator() (void) const noexcept {
                return PositionViewRangeAdaptorClosure{};
            }
        };

        struct ColorViewRangeAdaptorClosure {
            constexpr ColorViewRangeAdaptorClosure(void) noexcept = default;

            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.color; }
            constexpr auto operator() (Range&& range) const noexcept {
                return ColorView{ std::forward<Range>(range) };
            }
        };

        struct ColorViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.color; }
            constexpr auto operator() (Range&& range) const noexcept {
                return ColorView{ std::forward<Range>(range) };
            }

            constexpr auto operator() (void) const noexcept {
                return ColorViewRangeAdaptorClosure{};
            }
        };

        struct TextureCoordsViewRangeAdaptorClosure {
            constexpr TextureCoordsViewRangeAdaptorClosure(void) noexcept = default;

            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.textureCoords; }
            constexpr auto operator() (Range&& range) const noexcept {
                return TextureCoordsView{ std::forward<Range>(range) };
            }
        };

        struct TextureCoordsViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (std::ranges::range_value_t<Range> vertex) { vertex.textureCoords; }
            constexpr auto operator() (Range&& range) const noexcept {
                return TextureCoordsView{ std::forward<Range>(range) };
            }

            constexpr auto operator() (void) const noexcept {
                return TextureCoordsViewRangeAdaptorClosure{};
            }
        };

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, PositionViewRangeAdaptorClosure const& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, PositionViewRangeAdaptor const& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, ColorViewRangeAdaptorClosure const& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, ColorViewRangeAdaptor const& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, TextureCoordsViewRangeAdaptorClosure const& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, TextureCoordsViewRangeAdaptor const& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

    }

    namespace views {
        inline constexpr details::PositionViewRangeAdaptor      position;
        inline constexpr details::ColorViewRangeAdaptor         color;
        inline constexpr details::TextureCoordsViewRangeAdaptor textureCoords;
    }

}
#pragma once

#include <iterator>
#include <ranges>

namespace ge {

    template <class T>
    using position_t = decltype(std::declval<T>().position);

    template <class T>
    using color_t = decltype(std::declval<T>().color);

    template <class T>
    using coords_t = decltype(std::declval<T>().textureCoords);

    template <std::ranges::input_range Range>
    using iterator_value_type_t = std::iter_value_t<std::ranges::iterator_t<Range>>;

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range> && requires (iterator_value_type_t<Range> vertex) { vertex.position; })
    class PositionView : public std::ranges::view_interface<PositionView<Range>> {
    public:
        constexpr explicit PositionView(void) noexcept = default;
        constexpr explicit PositionView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, void, void, void, void> {
        public:
            using value_type = position_t<iterator_value_type_t<Range>>;
            using reference = value_type&;
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(const std::ranges::iterator_t<Range>& iter) noexcept : iter{iter} {}

            iterator& operator++(void) noexcept { ++iter; return *this; }
            iterator operator++(int) noexcept { auto tmp = *this; ++(*this); return tmp; }
            friend bool operator==(const iterator& left, const iterator& right) noexcept { return left.iter == right.iter; }

            auto& operator*(void) const noexcept {return iter->position; }
        private:
            std::ranges::iterator_t<Range> iter;
        };

        constexpr const Range& base(void) const { return base_; }
        constexpr Range&& base(void) { return std::move(base_); }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const { return iterator{ std::ranges::end(base_) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const Range> { return std::ranges::size(base_); }
    private:
        Range base_;
    };

    template <class Range>
    PositionView(Range&& base) -> PositionView<std::views::all_t<Range>>;

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range> && requires (iterator_value_type_t<Range> vertex) { vertex.color; })
    class ColorView : public std::ranges::view_interface<ColorView<Range>> {
    public:
        constexpr explicit ColorView(void) noexcept = default;
        constexpr explicit ColorView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, void, void, void, void> {
        public:
            using value_type = color_t<iterator_value_type_t<Range>>;
            using reference = value_type&;
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(const std::ranges::iterator_t<Range>& iter) noexcept : iter{iter} {}

            iterator& operator++(void) noexcept { ++iter; return *this; }
            iterator operator++(int) noexcept { auto tmp = *this; ++(*this); return tmp; }
            friend bool operator==(const iterator& left, const iterator& right) noexcept { return left.iter == right.iter; }

            auto& operator*(void) const noexcept {return iter->color; }
        private:
            std::ranges::iterator_t<Range> iter;
        };

        constexpr const Range& base(void) const { return base_; }
        constexpr Range&& base(void) { return std::move(base_); }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const { return iterator{ std::ranges::end(base_) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const Range> { return std::ranges::size(base_); }
    private:
        Range base_;
    };

    template <class Range>
    ColorView(Range&& base) -> ColorView<std::views::all_t<Range>>;

    template <std::ranges::input_range Range>
        requires (std::ranges::view<Range> && requires (iterator_value_type_t<Range> vertex) { vertex.textureCoords; })
    class TextureCoordsView : public std::ranges::view_interface<TextureCoordsView<Range>> {
    public:
        constexpr explicit TextureCoordsView(void) noexcept = default;
        constexpr explicit TextureCoordsView(Range base_) noexcept : base_{base_} {}

        class iterator : public std::iterator<std::input_iterator_tag, void, void, void, void> {
            using value_type = coords_t<iterator_value_type_t<Range>>;
            using reference = value_type&;
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(const std::ranges::iterator_t<Range>& iter) noexcept : iter{iter} {}

            iterator& operator++(void) noexcept { ++iter; return *this; }
            iterator operator++(int) noexcept { auto tmp = *this; ++iter; return *this; }

            friend bool operator== (const iterator& left, const iterator& right) noexcept { return left.iter == right.iter; }

            auto& operator*(void) const noexcept { return iter->textureCoords; }
        private:
            std::ranges::iterator_t<Range> iter;
        };

        constexpr const Range& base(void) const { return base_; }
        constexpr Range&& base(void) { return std::move(base_); }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(base_) }; }
        constexpr iterator end(void) const { return iterator{ std::ranges::end(base_) }; }

        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const Range> { return std::ranges::size(base_); }
    private:
        Range base_;
    };

    template <class Range>
    TextureCoordsView(Range&& base) -> TextureCoordsView<std::views::all_t<Range>>;

    namespace details {

        struct PositionViewRangeAdaptorClosure {
            constexpr PositionViewRangeAdaptorClosure(void) noexcept = default;

            template <std::ranges::viewable_range Range>
                requires requires (iterator_value_type_t<Range> vertex) { vertex.position; }
            constexpr auto operator() (Range&& range) const noexcept {
                return PositionView{ std::forward<Range>(range) };
            }
        };

        struct PositionViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (iterator_value_type_t<Range> vertex) { vertex.position; }
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
                requires requires (iterator_value_type_t<Range> vertex) { vertex.color; }
            constexpr auto operator() (Range&& range) const noexcept {
                return ColorView{ std::forward<Range>(range) };
            }
        };

        struct ColorViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (iterator_value_type_t<Range> vertex) { vertex.color; }
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
                requires requires (iterator_value_type_t<Range> vertex) { vertex.textureCoords; }
            constexpr auto operator() (Range&& range) const noexcept {
                return TextureCoordsView{ std::forward<Range>(range) };
            }
        };

        struct TextureCoordsViewRangeAdaptor {
            template <std::ranges::viewable_range Range>
                requires requires (iterator_value_type_t<Range> vertex) { vertex.textureCoords; }
            constexpr auto operator() (Range&& range) const noexcept {
                return TextureCoordsView{ std::forward<Range>(range) };
            }

            constexpr auto operator() (void) const noexcept {
                return TextureCoordsViewRangeAdaptorClosure{};
            }
        };

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const PositionViewRangeAdaptorClosure& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const PositionViewRangeAdaptor& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const ColorViewRangeAdaptorClosure& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const ColorViewRangeAdaptor& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const TextureCoordsViewRangeAdaptorClosure& closure) noexcept {
            return closure(std::forward<Range>(range));
        }

        template <std::ranges::viewable_range Range>
        constexpr auto operator | (Range&& range, const TextureCoordsViewRangeAdaptor& adaptor) noexcept {
            return adaptor(std::forward<Range>(range));
        }

    }

    namespace views {
        inline constexpr details::PositionViewRangeAdaptor position;
        inline constexpr details::ColorViewRangeAdaptor color;
        inline constexpr details::TextureCoordsViewRangeAdaptor textureCoords;
    }

}
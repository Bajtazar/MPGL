#pragma once

#include <iterator>
#include <ranges>

namespace ge {

    template <class T>
    concept isVertex = requires(T vertex) { vertex.position, vertex.color; };

    template <class T>
    using position_t = decltype(std::declval<T&>().position);

    template <class T>
    using color_t = decltype(std::declval<T&>().color);

    template <std::ranges::input_range _Range>
    using iterator_value_type_t = typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type;

    template <std::ranges::input_range _Range>
        requires std::ranges::view<_Range> && isVertex<iterator_value_type_t<_Range>>
    class position_view : public std::ranges::view_interface<position_view<_Range>> {
    public:
        constexpr explicit position_view(void) noexcept = default;
        constexpr explicit position_view(_Range __base) noexcept : __base(__base) {}

        class iterator : public std::iterator<std::input_iterator_tag, void, void, void, void> {
        public:
            using value_type = position_t<iterator_value_type_t<_Range>>;
            using reference = value_type&;
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(const std::ranges::iterator_t<_Range>& __iter) noexcept : __iter(__iter) {}

            iterator& operator++(void) noexcept { ++__iter; return *this; }
            iterator operator++(int) noexcept { auto tmp = *this; ++(*this); return tmp; }
            friend bool operator==(const iterator& left, const iterator& right) noexcept { return left.__iter == right.__iter; }
            friend bool operator!=(const iterator& left, const iterator& right) noexcept { return left.__iter != right.__iter; }

            auto& operator*(void) const noexcept {return __iter->position; }
        private:
            std::ranges::iterator_t<_Range> __iter;
        };

        constexpr const _Range& base(void) const { return __base; }
        constexpr _Range&& base(void) { return std::move(__base); }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(__base) }; }
        constexpr iterator end(void) const { return iterator{ std::ranges::end(__base) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const _Range> { return std::ranges::size(__base); }
    private:
        _Range __base;
    };

    template <class _Range>
    position_view(_Range&& __base) -> position_view<std::views::all_t<_Range>>;

    template <std::ranges::input_range _Range>
        requires std::ranges::view<_Range> && isVertex<iterator_value_type_t<_Range>>
    class color_view : public std::ranges::view_interface<color_view<_Range>> {
    public:
        constexpr explicit color_view(void) noexcept = default;
        constexpr explicit color_view(_Range __base) noexcept : __base(__base) {}

        class iterator : public std::iterator<std::input_iterator_tag, void, void, void, void> {
        public:
            using value_type = color_t<iterator_value_type_t<_Range>>;
            using reference = value_type&;
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;

            explicit iterator(void) noexcept = default;
            explicit iterator(const std::ranges::iterator_t<_Range>& __iter) noexcept : __iter(__iter) {}

            iterator& operator++(void) noexcept { ++__iter; return *this; }
            iterator operator++(int) noexcept { auto tmp = *this; ++(*this); return tmp; }
            friend bool operator==(const iterator& left, const iterator& right) noexcept { return left.__iter == right.__iter; }
            friend bool operator!=(const iterator& left, const iterator& right) noexcept { return left.__iter != right.__iter; }

            auto& operator*(void) const noexcept {return __iter->color; }
        private:
            std::ranges::iterator_t<_Range> __iter;
        };

        constexpr const _Range& base(void) const { return __base; }
        constexpr _Range&& base(void) { return std::move(__base); }

        constexpr iterator begin(void) const { return iterator{ std::ranges::begin(__base) }; }
        constexpr iterator end(void) const { return iterator{ std::ranges::end(__base) }; }
        constexpr auto size(void) const noexcept requires std::ranges::sized_range<const _Range> { return std::ranges::size(__base); }
    private:
        _Range __base;
    };

    template <class _Range>
    color_view(_Range&& __base) -> color_view<std::views::all_t<_Range>>;

    namespace details {

        struct position_view_range_adaptor_closure {
            constexpr position_view_range_adaptor_closure(void) noexcept = default;

            template <std::ranges::viewable_range _Range>
            constexpr auto operator() (_Range&& __range) const noexcept {
                return position_view(std::forward<_Range>(__range));
            }
        };

        struct position_view_range_adaptor {
            template <std::ranges::viewable_range _Range>
            constexpr auto operator() (_Range&& __range) const noexcept {
                return position_view(std::forward<_Range>(__range));
            }

            constexpr auto operator() (void) const noexcept {
                return position_view_range_adaptor_closure();
            }
        };

        struct color_view_range_adaptor_closure {
            constexpr color_view_range_adaptor_closure(void) noexcept = default;

            template <std::ranges::viewable_range _Range>
            constexpr auto operator() (_Range&& __range) const noexcept {
                return color_view(std::forward<_Range>(__range));
            }
        };

        struct color_view_range_adaptor {
            template <std::ranges::viewable_range _Range>
            constexpr auto operator() (_Range&& __range) const noexcept {
                return color_view(std::forward<_Range>(__range));
            }

            template <std::ranges::viewable_range _Range>
            constexpr auto operator() (void) const noexcept {
                return color_view_range_adaptor_closure();
            }
        };

        template <std::ranges::viewable_range _Range>
        constexpr auto operator | (_Range&& __range, const position_view_range_adaptor_closure& __closure) noexcept {
            return __closure(std::forward<_Range>(__range));
        }

        template <std::ranges::viewable_range _Range>
        constexpr auto operator | (_Range&& __range, const position_view_range_adaptor& __adaptor) noexcept {
            return __adaptor(std::forward<_Range>(__range));
        }

        template <std::ranges::viewable_range _Range>
        constexpr auto operator | (_Range&& __range, const color_view_range_adaptor_closure& __closure) noexcept {
            return __closure(std::forward<_Range>(__range));
        }

        template <std::ranges::viewable_range _Range>
        constexpr auto operator | (_Range&& __range, const color_view_range_adaptor& __adaptor) noexcept {
            return __adaptor(std::forward<_Range>(__range));
        }

    }

    namespace views {
        inline constexpr details::position_view_range_adaptor position;
        inline constexpr details::color_view_range_adaptor color;
    }

}
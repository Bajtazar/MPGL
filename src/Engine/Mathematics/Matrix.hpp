#pragma once

#include "Vector.hpp"

namespace ge {

    template <typename T, std::size_t Rows>
    constexpr decltype(auto) tupleConstructor(void) noexcept {
        if constexpr (Rows > 1)
            return std::tuple_cat(tupleConstructor<T, Rows-1>(), std::tuple<T>{});
        else
            return std::tuple<T>{};
    }

    template <AllSame... Args>
    constexpr Vector<Args...> vectorConverter(const std::tuple<Args...>& tuple) noexcept;

    template <Arithmetic T, std::size_t Rows>
    using MatrixTuple = decltype(tupleConstructor<decltype(vectorConverter(tupleConstructor<T, Rows>())), Rows>());

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    class Matrix : public MatrixTuple<T, Rows> {
    public:
        using value_type = decltype(vectorConverter(tupleConstructor<T, Rows>()));

        template <AllSame<value_type>... Args>
            requires (sizeof...(Args) == Rows)
        constexpr Matrix(const Args&... args) noexcept : MatrixTuple<T, Rows>{ args... } { std::reverse(begin(), end()); }
        constexpr Matrix(void) noexcept = default;

        // returns columns not rows !
        template <std::size_t Index>
            requires (Index < Rows)
        constexpr value_type& get(void) noexcept { return std::get<Rows - 1 - Index>(static_cast<MatrixTuple<T, Rows>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Rows)
        constexpr const value_type& get(void) const noexcept { return std::get<Rows - 1 - Index>(static_cast<const MatrixTuple<T, Rows>&>(*this)); }

        template <std::size_t Index>
            requires (Index < Rows)
        constexpr value_type&& get(void) noexcept { return std::get<Rows - 1 - Index>(static_cast<MatrixTuple<T, Rows>&&>(*this)); }

        constexpr std::size_t size(void) const noexcept { return Rows; }

        template <Arithmetic U>
        constexpr operator Matrix<U, Rows>() const noexcept;

        template <std::size_t URows>
            requires (URows > Rows)
        constexpr operator Matrix<T, URows>() const noexcept;

        template <typename value_type>
        class Iterator : public std::iterator<std::contiguous_iterator_tag, value_type, std::ptrdiff_t, value_type*, value_type&> {
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using pointer = value_type*;

            constexpr explicit Iterator(value_type* iter) noexcept : iter(iter) {}
            constexpr explicit Iterator(void) noexcept = default;

            constexpr Iterator& operator++(void) noexcept { ++iter; return *this; }
            constexpr Iterator& operator++(int) noexcept { auto tmp = *this; ++iter; return tmp; }

            constexpr Iterator& operator--(void) noexcept { --iter; return *this; }
            constexpr Iterator& operator--(int) noexcept { auto tmp = *this; --iter; return tmp; }

            constexpr reference operator*(void) const noexcept { return *iter; }
            constexpr pointer operator->(void) noexcept { return iter; }

            constexpr Iterator& operator+=(std::size_t offset) noexcept { iter += offset; return *this; }
            constexpr Iterator& operator-=(std::size_t offset) noexcept { iter -= offset; return *this; }
            constexpr Iterator operator[] (std::size_t offset) noexcept { auto tmp = *this; tmp += offset; return tmp; }

            friend constexpr bool operator== (const Iterator& right, const Iterator& left) noexcept { return right.iter == left.iter; }
            friend constexpr bool operator!= (const Iterator& right, const Iterator& left) noexcept { return right.iter != left.iter; }

            friend constexpr Iterator operator+ (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
            friend constexpr Iterator operator+ (std::size_t right, const Iterator& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
            friend constexpr Iterator operator- (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
            friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.iter - left.iter; }

            friend constexpr bool operator> (const Iterator& right, const Iterator& left) noexcept { return right.iter > left.iter; }
            friend constexpr bool operator< (const Iterator& right, const Iterator& left) noexcept { return right.iter < left.iter; }
            friend constexpr bool operator>= (const Iterator& right, const Iterator& left) noexcept { return right.iter >= left.iter; }
            friend constexpr bool operator<= (const Iterator& right, const Iterator& left) noexcept { return right.iter <= left.iter; }
        private:
            value_type* iter;
        };

        using iterator = Iterator<value_type>;
        using const_iterator = Iterator<const value_type>;

        constexpr iterator begin(void) noexcept { return iterator{ reinterpret_cast<value_type*>(this) }; }
        constexpr iterator end(void) noexcept { return iterator{ reinterpret_cast<value_type*>(this) + Rows }; }

        constexpr const_iterator begin(void) const noexcept { return const_iterator{ reinterpret_cast<const value_type*>(this) }; }
        constexpr const_iterator end(void) const noexcept { return const_iterator{ reinterpret_cast<const value_type*>(this) + Rows }; }

        constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ reinterpret_cast<const value_type*>(this) }; }
        constexpr const_iterator cend(void) const noexcept { return const_iterator{ reinterpret_cast<const value_type*>(this) + Rows }; }

        constexpr auto rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr auto rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr auto crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr auto crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr value_type& operator[] (std::size_t index) noexcept { return *(reinterpret_cast<value_type*>(this) + index); }
        constexpr const value_type& operator[] (std::size_t index) const noexcept { return *(reinterpret_cast<const value_type*>(this) + index); }

    };

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <Arithmetic U>
    constexpr Matrix<T, Rows>::operator Matrix<U, Rows>() const noexcept {
        Matrix<U, Rows> base;
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <std::size_t URows>
        requires (URows > Rows)
    constexpr Matrix<T, Rows>::operator Matrix<T, URows>() const noexcept {
        Matrix<T, URows> base;
        std::ranges::copy(*this, base);
        return base;
    }

}

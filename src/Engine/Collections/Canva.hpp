#pragma once

#include <inttypes.h>
#include <iterator>
#include <concepts>
#include <memory>
#include <vector>

#include "../Mathematics/Vector.hpp"
#include "../Traits/Concepts.hpp"

#if __cplusplus > 202002L
#define VEC_CONSTEXPR constexpr
#else
#define VEC_CONSTEXPR
#endif

namespace ge {

    typedef TwoVector<std::size_t>          Vector2ul;

    template <typename Base>
        requires (NotReference<Base> && std::is_default_constructible_v<Base>)
    class Canva {
    public:
        VEC_CONSTEXPR explicit Canva(std::size_t width, std::size_t height) noexcept;
        VEC_CONSTEXPR explicit Canva(Vector2ul const& dimmensions) noexcept : Canva{dimmensions[0], dimmensions[1]} {}
        VEC_CONSTEXPR explicit Canva(void) noexcept = default;

        template <class BaseType>
        class Row {
        public:
            using value_type = std::conditional_t<std::is_const_v<BaseType>, const Base, Base>;

            template <typename IterValue>
            class Iterator : public std::iterator<std::random_access_iterator_tag, IterValue, std::ptrdiff_t, IterValue*, IterValue&> {
            public:
                using value_type = IterValue;
                using pointer = value_type*;
                using reference = value_type&;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::random_access_iterator_tag;

                constexpr explicit Iterator(pointer iter) noexcept : iter{iter} {}
                constexpr explicit Iterator(void) noexcept = default;

                constexpr Iterator& operator++ (void) noexcept { ++iter; return *this; }
                constexpr Iterator  operator++ (int)  noexcept { auto tmp = *this; ++iter; return *tmp; }

                constexpr Iterator& operator-- (void) noexcept { --iter; return *this; }
                constexpr Iterator  operator-- (int)  noexcept { auto tmp = *this; --iter; return *tmp; }

                constexpr reference operator*(void) const noexcept { return *iter; }
                constexpr pointer operator->(void)  const noexcept { return iter; }

                constexpr Iterator& operator+= (std::ptrdiff_t offset) noexcept { iter += offset; return *this; }
                constexpr Iterator& operator-= (std::ptrdiff_t offset) noexcept { iter -= offset; return *this; }
                constexpr reference operator[] (std::ptrdiff_t offset) const noexcept { return *(iter + offset); }

                friend constexpr bool operator== (const Iterator& left, const Iterator& right) noexcept { return left.iter == right.iter; }
                friend constexpr bool operator!= (const Iterator& left, const Iterator& right) noexcept { return left.iter != right.iter; }
                friend constexpr bool operator>  (const Iterator& left, const Iterator& right) noexcept { return left.iter > right.iter; }
                friend constexpr bool operator<  (const Iterator& left, const Iterator& right) noexcept { return left.iter < right.iter; }
                friend constexpr bool operator>= (const Iterator& left, const Iterator& right) noexcept { return left.iter >= right.iter; }
                friend constexpr bool operator<= (const Iterator& left, const Iterator& right) noexcept { return left.iter <= right.iter; }

                friend constexpr Iterator operator+ (const Iterator& right, std::ptrdiff_t left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
                friend constexpr Iterator operator+ (std::ptrdiff_t right, const Iterator& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
                friend constexpr Iterator operator- (const Iterator& right, std::ptrdiff_t left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
                friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.iter - left.iter; }
            private:
                pointer iter;
            };

            using iterator = Iterator<Base>;
            using const_iterator = Iterator<const Base>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            constexpr std::size_t size(void) const noexcept { return reference.get().width; }

            template <NotConstant Type = BaseType>
            constexpr Base& operator[] (std::size_t index) noexcept { return reference.get().image[reference.get().width * rowID + index]; }
            constexpr const Base& operator[] (std::size_t index) const noexcept { return reference.get().image[reference.get().width * rowID + index]; }

            template <NotConstant Type = BaseType>
            constexpr iterator begin(void) noexcept { return iterator{ &(reference.get().image[reference.get().width * rowID]) };  }
            constexpr const_iterator begin(void) const noexcept { return const_iterator{ &(reference.get().image[reference.get().width * rowID]) }; }

            template <NotConstant Type = BaseType>
            constexpr iterator end(void) noexcept { return iterator{ &(reference.get().image[reference.get().width * (rowID + 1)]) }; }
            constexpr const_iterator end(void) const noexcept { return const_iterator{ &(reference.get().image[reference.get().width * (rowID + 1)]) }; }

            constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ &(reference.get().image[reference.get().width * rowID]) }; }
            constexpr const_iterator cend(void) const noexcept { return const_iterator{ &(reference.get().image[reference.get().width * (rowID + 1)]) }; }

            template <NotConstant Type = BaseType>
            constexpr reverse_iterator rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
            constexpr const_reverse_iterator rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }

            template <NotConstant Type = BaseType>
            constexpr reverse_iterator rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }
            constexpr const_reverse_iterator rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

            constexpr const_reverse_iterator crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
            constexpr const_reverse_iterator crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

            friend class Canva;
        private:
            constexpr explicit Row(const std::reference_wrapper<BaseType>& reference, std::size_t rowID) noexcept : rowID{rowID}, reference{reference} {}

            std::size_t rowID;
            std::reference_wrapper<BaseType> reference;
        };

        VEC_CONSTEXPR void resize(std::size_t width, std::size_t height);
        constexpr Vector2ul size(void) const noexcept { return Vector2ul{width, height}; }

        constexpr std::size_t getWidth(void) const noexcept { return width; }
        constexpr std::size_t getHeight(void) const noexcept { return height; }

        VEC_CONSTEXPR const void* getMemoryPtr(void) const noexcept { return image.data(); }
        VEC_CONSTEXPR void* getMemoryPtr(void) noexcept { return image.data(); }

        constexpr Row<Canva> operator[] (std::size_t index) noexcept { return Row<Canva>{std::ref(*this), index}; }
        constexpr Row<const Canva> operator[] (std::size_t index) const noexcept { return Row<const Canva>{std::cref(*this), index}; }

        template <std::integral T>
        constexpr Base& operator[](Vector<T, 2> const& dimmensions) noexcept { return image[dimmensions[1] * width + dimmensions[0]]; }

        template <std::integral T>
        constexpr Base const& operator[](Vector<T, 2> const& dimmensions) const noexcept { return image[dimmensions[1] * width + dimmensions[0]]; }

        template <class BaseType>
        class Iterator {
        public:
            using value_type = Row<BaseType>;
            using reference = value_type;
            using difference_type = std::size_t;

            constexpr explicit Iterator(std::reference_wrapper<BaseType> ref, std::size_t row = 0) noexcept : row{row}, ref{ref} {}

            constexpr Iterator& operator++(void) noexcept { ++row; return *this; }
            constexpr Iterator& operator++(int) noexcept { auto tmp = *this; ++row; return tmp; }

            constexpr Iterator& operator--(void) noexcept { --row; return *this; }
            constexpr Iterator& operator--(int) noexcept { auto tmp = *this; --row; return tmp; }

            constexpr reference operator*(void) const noexcept { return value_type{ref, row}; }

            constexpr Iterator& operator+= (difference_type offset) noexcept { row += offset; return *this; }
            constexpr Iterator& operator-= (difference_type offset) noexcept { row -= offset; return *this; }
            constexpr reference operator[] (std::size_t offset) const noexcept { return Row{ref, row + offset}; }

            friend constexpr bool operator== (const Iterator& left, const Iterator& right) noexcept { return left.row == right.row; }
            friend constexpr bool operator!= (const Iterator& left, const Iterator& right) noexcept { return left.row != right.row; }
            friend constexpr bool operator>  (const Iterator& left, const Iterator& right) noexcept { return left.row > right.row; }
            friend constexpr bool operator<  (const Iterator& left, const Iterator& right) noexcept { return left.row < right.row; }
            friend constexpr bool operator>= (const Iterator& left, const Iterator& right) noexcept { return left.row >= right.row; }
            friend constexpr bool operator<= (const Iterator& left, const Iterator& right) noexcept { return left.row <= right.row; }

            friend constexpr Iterator operator+ (const Iterator& right, difference_type left) noexcept { auto tmp = right; tmp.row += left; return tmp; }
            friend constexpr Iterator operator+ (difference_type right, const Iterator& left) noexcept { auto tmp = left; tmp.row += right; return tmp; }
            friend constexpr Iterator operator- (const Iterator& right, difference_type left) noexcept { auto tmp = right; tmp.row -= left; return tmp; }
            friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.row - left.row; }
        private:
            std::size_t row;
            std::reference_wrapper<BaseType> ref;
        };

        using iterator = Iterator<Canva>;
        using const_iterator = Iterator<const Canva>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr iterator begin(void) noexcept { return iterator{ std::ref(*this) }; }
        constexpr iterator end(void) noexcept { return iterator{ std::ref(*this), height}; }

        constexpr const_iterator begin(void) const noexcept { return const_iterator{ std::cref(*this) }; }
        constexpr const_iterator end(void) const noexcept { return const_iterator{ std::cref(*this), height}; }

        constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ std::cref(*this) }; }
        constexpr const_iterator cend(void) const noexcept { return const_iterator{ std::cref(*this), height}; }

        constexpr reverse_iterator rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr reverse_iterator rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

    private:
        std::vector<Base> image;
        std::size_t width;
        std::size_t height;
    };

    template <typename Base>
        requires (NotReference<Base> && std::is_default_constructible_v<Base>)
    VEC_CONSTEXPR Canva<Base>::Canva(std::size_t width, std::size_t height) noexcept
        : image(width * height, Base{}), width{width}, height{height} {}

    template <typename Base>
        requires (NotReference<Base> && std::is_default_constructible_v<Base>)
    VEC_CONSTEXPR void Canva<Base>::resize(std::size_t width, std::size_t height) {
        this->width = width;
        this->height = height;
        image.resize(width * height);
    }

}

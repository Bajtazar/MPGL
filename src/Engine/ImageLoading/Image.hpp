#pragma once

#include <inttypes.h>
#include <iterator>
#include <concepts>
#include <istream>
#include <ostream>
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

    class Image {
    public:
        VEC_CONSTEXPR explicit Image(std::size_t width, std::size_t height) noexcept;
        VEC_CONSTEXPR explicit Image(Vector2ul const& dimmensions) noexcept : Image{dimmensions[0], dimmensions[1]} {}
        VEC_CONSTEXPR explicit Image(void) noexcept = default;

        #pragma pack(push, 1)

        struct Pixel {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
            constexpr uint8_t& operator[] (std::size_t index) noexcept;
            constexpr const uint8_t& operator[] (std::size_t index) const noexcept;
            constexpr Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept : red{red}, green{green}, blue{blue}, alpha{alpha} {}
            constexpr Pixel(void) noexcept : red{0}, green{0}, blue{0}, alpha{255} {}
        };

        #pragma pack(pop)

        struct Manip {
            static std::istream& RGB(std::istream& is, Pixel& pixel) noexcept;
            static std::ostream& RGB(std::ostream& os, const Pixel& pixel) noexcept;

            template <std::input_iterator Iter>
            static void RGB(Iter& iter, Pixel& pixel) noexcept;
        };

        template <class BaseType>
        class Row {
        public:
            using value_type = std::conditional_t<std::is_const_v<BaseType>, const Pixel, Pixel>;

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

            using iterator = Iterator<Pixel>;
            using const_iterator = Iterator<const Pixel>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            template <NotConstant Type = BaseType>
            constexpr Pixel& operator[] (std::size_t index) noexcept { return reference.get().image[reference.get().width * rowID + index]; }
            constexpr const Pixel& operator[] (std::size_t index) const noexcept { return reference.get().image[reference.get().width * rowID + index]; }

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

            friend class Image;
        private:
            constexpr explicit Row(const std::reference_wrapper<BaseType>& reference, std::size_t rowID) noexcept : rowID{rowID}, reference{reference} {}

            std::size_t rowID;
            std::reference_wrapper<BaseType> reference;
        };

        VEC_CONSTEXPR void resize(std::size_t width, std::size_t height);
        constexpr Vector2ul size(void) const noexcept { return Vector2ul{width, height}; }

        constexpr std::size_t getWidth(void) const noexcept { return width; }
        constexpr std::size_t getHeight(void) const noexcept { return height; }

        VEC_CONSTEXPR const uint8_t* getMemoryPtr(void) const noexcept { return &image.front().red; }
        VEC_CONSTEXPR uint8_t* getMemoryPtr(void) noexcept { return &image.front().red; }

        constexpr Row<Image> operator[] (std::size_t index) noexcept { return Row<Image>{std::ref(*this), index}; }
        constexpr Row<const Image> operator[] (std::size_t index) const noexcept { return Row<const Image>{std::cref(*this), index}; }

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

        using iterator = Iterator<Image>;
        using const_iterator = Iterator<const Image>;
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
        std::vector<Pixel> image;
        std::size_t width;
        std::size_t height;
    };

    constexpr uint8_t& Image::Pixel::operator[] (std::size_t index) noexcept {
        switch (index) {
            case 0:
                return red;
            case 1:
                return green;
            case 2:
                return blue;
            case 3:
                return alpha;
        }
        return red;
    }

    constexpr const uint8_t& Image::Pixel::operator[] (std::size_t index) const noexcept {
        switch (index) {
            case 0:
                return red;
            case 1:
                return green;
            case 2:
                return blue;
            case 3:
                return alpha;
        }
        return red;
    }

    template <std::input_iterator Iter>
    void Image::Manip::RGB(Iter& iter, Image::Pixel& pixel) noexcept {
        pixel.blue = *iter++;
        pixel.green = *iter++;
        pixel.red = *iter++;
    }

}

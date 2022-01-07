#pragma once

#include "Canva.hpp"

#include <istream>
#include <ostream>

namespace ge {

    #pragma pack(push, 1)

    struct Pixel {
        typedef uint8                           Subpixel;

        constexpr Pixel(Subpixel red, Subpixel green,
            Subpixel blue, Subpixel alpha) noexcept
                : red{red}, green{green},
                blue{blue}, alpha{alpha} {}
        constexpr Pixel(void) noexcept
            : red{0x00}, green{0x00}, blue{0x00},
            alpha{0xFF} {}

        Subpixel                                red;
        Subpixel                                green;
        Subpixel                                blue;
        Subpixel                                alpha;

        constexpr Subpixel& operator[] (std::size_t index) noexcept;
        constexpr Subpixel const& operator[] (
            std::size_t index) const noexcept;

        struct Manip {
            static std::istream& RGB(std::istream& is,
                Pixel& pixel) noexcept;
            static std::ostream& RGB(std::ostream& os,
                Pixel const& pixel) noexcept;

            template <std::input_iterator Iter>
            static void RGB(Iter& iter, Pixel& pixel) noexcept;
        };
    };

    #pragma pack(pop)

    template class Canva<Pixel>;

    typedef Canva<Pixel>                        Image;

    constexpr Pixel::Subpixel&
        Pixel::operator[] (std::size_t index) noexcept
    {
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

    constexpr const Pixel::Subpixel&
        Pixel::operator[] (std::size_t index) const noexcept
    {
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
    void Pixel::Manip::RGB(Iter& iter, Pixel& pixel) noexcept {
        pixel.blue = *iter++;
        pixel.green = *iter++;
        pixel.red = *iter++;
    }

}

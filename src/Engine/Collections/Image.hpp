#pragma once

#include "Canva.hpp"

#include <istream>
#include <ostream>

namespace ge {

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

        struct Manip {
            static std::istream& RGB(std::istream& is, Pixel& pixel) noexcept;
            static std::ostream& RGB(std::ostream& os, const Pixel& pixel) noexcept;

            template <std::input_iterator Iter>
            static void RGB(Iter& iter, Pixel& pixel) noexcept;
        };
    };

    #pragma pack(pop)

    template class Canva<Pixel>;

    typedef Canva<Pixel>        Image;

    constexpr uint8_t& Pixel::operator[] (std::size_t index) noexcept {
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

    constexpr const uint8_t& Pixel::operator[] (std::size_t index) const noexcept {
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
#include "Image.hpp"

namespace ge {

    VEC_CONSTEXPR Image::Image(std::size_t width, std::size_t height) noexcept : image{width * height, Pixel{}}, width{width}, height{height} {}

    VEC_CONSTEXPR void Image::resize(std::size_t width, std::size_t height) {
        this->width = width;
        this->height = height;
        image.resize(width * height);
    }

    std::ostream& Image::Manip::RGB(std::ostream& os, const Pixel& pixel) noexcept {
        os.put(reinterpret_cast<const char&>(pixel.blue));
        os.put(reinterpret_cast<const char&>(pixel.green));
        os.put(reinterpret_cast<const char&>(pixel.red));
        return os;
    }

    std::istream& Image::Manip::RGB(std::istream& is, Pixel& pixel) noexcept {
        is.get(reinterpret_cast<char&>(pixel.blue));
        is.get(reinterpret_cast<char&>(pixel.green));
        is.get(reinterpret_cast<char&>(pixel.red));
        return is;
    }

}

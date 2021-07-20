#include "Image.hpp"

namespace ge {

    VEC_CONSTEXPR Image::Image(std::size_t width, std::size_t height) noexcept : image{width * height, Pixel{}}, width{width}, height{height} {}

    VEC_CONSTEXPR void Image::resize(std::size_t width, std::size_t height) {
        this->width = width;
        this->height = height;
        image.resize(width * height);
    }

    std::istream& operator>> (std::istream& is, Image::Pixel& pixel) noexcept {
        is.get(reinterpret_cast<char&>(pixel.red));
        is.get(reinterpret_cast<char&>(pixel.green));
        is.get(reinterpret_cast<char&>(pixel.blue));
        return is;
    }

    std::ostream& operator<< (std::ostream& os, Image::Pixel& pixel) noexcept {
        os.put(reinterpret_cast<char&>(pixel.red));
        os.put(reinterpret_cast<char&>(pixel.green));
        os.put(reinterpret_cast<char&>(pixel.blue));
        return os;
    }

}

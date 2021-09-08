#include "Image.hpp"

namespace ge {

    std::ostream& Pixel::Manip::RGB(std::ostream& os, const Pixel& pixel) noexcept {
        os.put(reinterpret_cast<const char&>(pixel.blue));
        os.put(reinterpret_cast<const char&>(pixel.green));
        os.put(reinterpret_cast<const char&>(pixel.red));
        return os;
    }

    std::istream& Pixel::Manip::RGB(std::istream& is, Pixel& pixel) noexcept {
        is.get(reinterpret_cast<char&>(pixel.blue));
        is.get(reinterpret_cast<char&>(pixel.green));
        is.get(reinterpret_cast<char&>(pixel.red));
        return is;
    }

}

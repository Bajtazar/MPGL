#include "Image.hpp"

namespace ge {

    std::ostream& Pixel::Manip::RGB(std::ostream& os,
        Pixel const& pixel) noexcept
    {
        os.put(reinterpret_cast<char const&>(pixel.blue));
        os.put(reinterpret_cast<char const&>(pixel.green));
        os.put(reinterpret_cast<char const&>(pixel.red));
        return os;
    }

    std::istream& Pixel::Manip::RGB(std::istream& is,
        Pixel& pixel) noexcept
    {
        is.get(reinterpret_cast<char&>(pixel.blue));
        is.get(reinterpret_cast<char&>(pixel.green));
        is.get(reinterpret_cast<char&>(pixel.red));
        return is;
    }

}

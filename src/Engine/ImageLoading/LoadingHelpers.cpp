#include "LoadingHelpers.hpp"

namespace ge {

    std::string readNChars(std::size_t length, std::istream& file) noexcept {
        std::string data(length, ' ');
        std::ranges::for_each(data, [&file](auto& c){ file.get(c); });
        return data;
    }

    void ignoreNBytes(std::size_t length, std::istream& is) noexcept {
        char ingoredByte;
        for (std::size_t i = 0;i != length; ++i)
            is.get(ingoredByte);
    }

}

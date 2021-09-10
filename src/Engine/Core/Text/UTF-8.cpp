#include "UTF-8.hpp"

namespace ge {

    uint32_t fromUTF8(std::string unicodeString) {
        uint8_t bitmask = unicodeString.size() != 1 ? (1 << (7 - unicodeString.size())) - 1 : 0x7F;
        uint32_t front = (unicodeString.front() & bitmask) << (6 * (unicodeString.size() - 1));
        unicodeString.erase(unicodeString.begin());
        return front + decodeTail(unicodeString);
    }

    uint32_t decodeTail(std::string const& unicode) noexcept {
        uint32_t sum = 0;
        for (char const& link : unicode)
            sum = (sum << 6) + (link & 0x3F);
        return sum;
    }

}
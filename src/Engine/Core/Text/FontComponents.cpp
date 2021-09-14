#include "FontComponents.hpp"

namespace ge {

    void KernTable::setAxis(uint16_t const& coverage) noexcept {
        bool horizontal = coverage & 0x0001;
        bool cross = coverage & 0x0002;
        axis = cross == horizontal;
    }

}
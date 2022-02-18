#pragma once

#include "HuffmanTreeException.hpp"

namespace mpgl {

    class HuffmanTreeEmptyMapException : public HuffmanTreeException {
    public:
        explicit HuffmanTreeEmptyMapException(void) noexcept = default;
        const char* what (void) const noexcept{ return "CharacterMap given to HuffmanTree is empty."; }
    };

}

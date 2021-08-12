#pragma once

#include "HuffmanTreeException.hpp"

namespace ge {

    class HuffmanTreeEmptyMapException : public HuffmanTreeException {
    public:
        explicit HuffmanTreeEmptyMapException(void) noexcept = default;
        const char* what (void) const noexcept{ return "CharacterMap given to HuffmanTree is empty."; }
    };

}

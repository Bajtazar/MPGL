#pragma once

#include "HuffmanTreeException.hpp"

namespace mpgl {

    class HuffmanTreeDecoderException : public HuffmanTreeException {
    public:
        explicit HuffmanTreeDecoderException(void) noexcept = default;
        const char* what (void) const noexcept{ return "Token out of alphabet has been found."; }
    };

}

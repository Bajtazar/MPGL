#pragma once

#include <exception>

namespace ge {

    class HuffmanTreeDecoderException : public std::exception {
    public:
        explicit HuffmanTreeDecoderException(void) noexcept = default;
        const char* what (void) const noexcept{ return "Token out of alphabet has been found."; }
    };

}

#pragma once

#include <exception>
#include <string>

namespace ge {

    class HuffmanTreeEmptyMapException : public std::exception {
    public:
        explicit HuffmanTreeEmptyMapException(void) noexcept {}
        const char* what (void) const noexcept{ return "CharacterMap given to HuffmanTree is empty."; }
    };

}

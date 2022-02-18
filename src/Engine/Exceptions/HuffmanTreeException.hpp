#pragma once

#include <exception>

namespace mpgl {

    struct HuffmanTreeException : public std::exception {
        virtual const char* what(void) const noexcept { return std::exception::what(); }
        virtual ~HuffmanTreeException(void) noexcept = default;
    };

}

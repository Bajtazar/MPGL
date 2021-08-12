#pragma once

#include <exception>

namespace ge {

    struct DeflateDecoderException : public std::exception {
        virtual const char* what(void) const noexcept { return std::exception::what(); }
        virtual ~DeflateDecoderException(void) noexcept = default;
    };

}

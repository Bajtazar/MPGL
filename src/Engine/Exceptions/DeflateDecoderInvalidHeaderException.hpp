#pragma once

#include <exception>

namespace ge {

    struct DeflateDecoderInvalidHeaderException : public std::exception {
        explicit DeflateDecoderInvalidHeaderException(void) noexcept = default;
        const char* what (void) const noexcept { return "Invalid header has been detected during stream parsing."; }
    };

}
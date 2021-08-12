#pragma once

#include "DeflateDecoderException.hpp"

namespace ge {

    struct DeflateDecoderInvalidHeaderException : public DeflateDecoderException {
        explicit DeflateDecoderInvalidHeaderException(void) noexcept = default;
        const char* what (void) const noexcept { return "Invalid header has been detected during stream parsing."; }
    };

}
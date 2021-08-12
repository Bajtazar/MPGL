#pragma once

#include "DeflateDecoderException.hpp"

namespace ge {

    struct DeflateDecoderDataCorruptionException : public DeflateDecoderException {
        explicit DeflateDecoderDataCorruptionException(void) = default;
        const char* what (void) const noexcept { return "Data corruption has been detected during stream parsing."; }
    };

}

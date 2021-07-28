#pragma once

#include <exception>

namespace ge {

    struct DeflateDecoderDataCorruptionException : public std::exception {
        explicit DeflateDecoderDataCorruptionException(void) = default;
        const char* what (void) const noexcept { return "Data corruption has been detected during stream parsing."; }
    };

}

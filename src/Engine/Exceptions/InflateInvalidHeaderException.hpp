#pragma once

#include "InflateException.hpp"

namespace mpgl {

    struct InflateInvalidHeaderException : public InflateException {
        explicit InflateInvalidHeaderException(void) noexcept = default;
        const char* what (void) const noexcept { return "Invalid header has been detected during stream parsing."; }
    };

}
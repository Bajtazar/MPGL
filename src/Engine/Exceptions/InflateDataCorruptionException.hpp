#pragma once

#include "InflateException.hpp"

namespace mpgl {

    struct InflateDataCorruptionException : public InflateException {
        explicit InflateDataCorruptionException(void) = default;
        const char* what (void) const noexcept { return "Data corruption has been detected during stream parsing."; }
    };

}

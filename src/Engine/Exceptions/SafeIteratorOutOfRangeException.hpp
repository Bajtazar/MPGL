#pragma once

#include <stdexcept>

namespace mpgl {

    struct SafeIteratorOutOfRangeException : public std::out_of_range {
        explicit SafeIteratorOutOfRangeException(void) noexcept : std::out_of_range{"SafeIterator out of range access attempt."} {}
    };

}

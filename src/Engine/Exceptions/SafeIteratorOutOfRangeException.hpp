#pragma once

#include <stdexcept>

namespace ge {

    struct SafeIteratorOutOfRangeException : public std::out_of_range {
        explicit SafeIteratorOutOfRangeException(void) noexcept = default;
        const char* what (void) const noexcept { return "SafeIterator out of range access attempt."; }
    };

}

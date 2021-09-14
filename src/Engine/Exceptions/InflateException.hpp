#pragma once

#include <exception>

namespace ge {

    struct InflateException : public std::exception {
        virtual const char* what(void) const noexcept { return std::exception::what(); }
        virtual ~InflateException(void) noexcept = default;
    };

}

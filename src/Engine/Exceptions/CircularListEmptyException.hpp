#pragma once

#include <exception>

namespace mpgl {

    class CircularListEmptyException
        : public std::exception
    {
    public:
        explicit CircularListEmptyException(
            void) noexcept = default;

        virtual const char* what(void) const noexcept final
            { return "Cannot remove element - list is already empty"; }
    };

}

#pragma once

#include <exception>

namespace mpgl {

    class SecurityUnknownPolicyException : public std::exception {
    public:
        explicit SecurityUnknownPolicyException(void) noexcept = default;
        virtual const char* what (void) const noexcept final { return "Unknown policy has been given"; }
    };

}

#pragma once

#include <exception>

namespace mpgl {

    class ExecusionUnknownPolicyException : public std::exception {
    public:
        explicit ExecusionUnknownPolicyException(void) noexcept = default;
        virtual const char* what (void) const noexcept final { return "Unknown policy has been given"; }
    };

}

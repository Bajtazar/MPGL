#pragma once

#include <exception>
#include <string>

namespace ge {

    class OptionsNoAntiAliasingSamplesException : public std::exception {
    public:
        explicit OptionsNoAntiAliasingSamplesException(void) noexcept :
            message{"Anti-Aliasing requires more samples than 0."} {}
        char const* what (void) const noexcept
            { return message.c_str(); }
        std::string const& getMessage(void) const noexcept
            { return message; }
    private:
        std::string message;
    };

}

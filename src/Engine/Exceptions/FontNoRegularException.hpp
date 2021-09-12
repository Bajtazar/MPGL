#pragma once

#include <exception>
#include <string>

namespace ge {

    class FontNoRegualrException : public std::exception {
    public:
        explicit FontNoRegualrException(std::string const& fontName) noexcept : std::exception{},
            message{fontName + " - font has not contain regular version"} {}
        virtual const char* what (void) const noexcept final { return message.c_str(); }
    private:
        std::string message;
    };

}
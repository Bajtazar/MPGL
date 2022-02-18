#pragma once

#include <exception>
#include <string>

namespace mpgl {

    class ImageLoadingException : public std::exception {
    public:
        explicit ImageLoadingException(const std::string& fileName, const std::string& message) noexcept : fileName{fileName}, message{message} {}
        virtual const char* what (void) const noexcept final { return message.c_str(); }
    private:
        std::string fileName;
        std::string message;
    };

}
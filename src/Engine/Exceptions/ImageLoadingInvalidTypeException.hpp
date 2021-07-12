#pragma once

#include <exception>
#include <string>

namespace ge {

    class ImageLoadingInvalidTypeException : public std::exception {
    public:
        explicit ImageLoadingInvalidTypeException(const std::string& fileName) noexcept : fileName{fileName}, message{"File " + fileName + " contains invalid data"} {}
        const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
        const std::string& getFileName(void) const noexcept { return fileName; }
    protected:
        std::string fileName;
        std::string message;
    };

}
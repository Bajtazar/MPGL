#pragma once

#include <exception>
#include <string>

namespace ge {

    class NotSupportedException : public std::exception {
    public:
        explicit NotSupportedException(const std::string& info) noexcept : message{"An exception related to the lack of feature support has occured - " + info + "\n"} {}
        const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
    protected:
        std::string message;
    };

}

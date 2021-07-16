#pragma once

#include <exception>
#include <vector>
#include <string>

namespace ge {

    class ShaderProgramLinkingException : public std::exception {
    public:
        explicit ShaderProgramLinkingException(const std::string& infoLog) noexcept : message{"The shader program linker has occured an unexpected error. Additional data: " + infoLog} {}
        const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
    private:
        std::string message;
    };

}

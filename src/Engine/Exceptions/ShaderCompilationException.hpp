#pragma once

#include <exception>
#include <string>

namespace ge {

    class ShaderCompilationException : public std::exception {
    public:
        explicit ShaderCompilationException(const std::string& shaderLog) noexcept : message{"The shader compiler has occured an error in shader compilation. Additional data: " + shaderLog} {}
        const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
    private:
        std::string message;
    };

}

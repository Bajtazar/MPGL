#pragma once

#include <exception>
#include <string>

namespace mpgl {

    class ShaderCompilationException : public std::exception {
    public:
        explicit ShaderCompilationException(std::string const& shaderLog) noexcept
            : message{"The shader compiler has occured an error in shader compilation. Additional data: " + shaderLog} {}
        const char* what (void) const noexcept{ return message.c_str(); }
        std::string const& getMessage(void) const noexcept { return message; }
    private:
        std::string                                 message;
    };

}

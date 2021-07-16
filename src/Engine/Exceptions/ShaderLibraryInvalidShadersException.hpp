#pragma once

#include <exception>
#include <vector>
#include <string>

namespace ge {

    class ShaderLibraryInvalidShadersException {
    public:
        explicit ShaderLibraryInvalidShadersException(std::vector<std::string> vertex, std::vector<std::string> fragment) noexcept;
        const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
    private:
        std::string message;
    };

}

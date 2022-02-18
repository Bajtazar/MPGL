#pragma once

#include "../Utility/Ranges.hpp"

#include <concepts>
#include <sstream>
#include <vector>

#include <glad/glad.h>

namespace mpgl {

    class Logger {
    public:
        template <std::derived_from<std::exception> Exception>
        static void checkCompilationStatus(uint32 bufferID, uint32 operation, const std::string& title);
        static std::string loggingString(std::size_t size, char fill) noexcept;
        static void saveOpenGl(const std::string& data, const std::string& title) noexcept;
        static void logInvalidFont(const std::string& fontName, const std::string& subtype);
    private:
        static std::stringstream getTimeInString(void) noexcept;
        static std::string getCurrentDay(void) noexcept;
    };

    template <std::derived_from<std::exception> Exception>
    void Logger::checkCompilationStatus(uint32 bufferID, uint32 operation, const std::string& title) {
        int32 status = 0;
        glGetProgramiv(bufferID, operation, &status);
        if (!status) {
            std::string info = loggingString(512, 0);
            glGetProgramInfoLog(bufferID, 512, nullptr, info.data());
            if (!accumulate(info, 0u))
                return;
            saveOpenGl(info, title);
            throw Exception{info};
        }
    }

}

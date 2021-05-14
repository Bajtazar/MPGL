#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

namespace ge {

    class Logger {
    public:
        static void saveOpenGl(GLchar* infoLog, size_t size) noexcept;
    private:
        static std::stringstream getTimeInString(void) noexcept;
        static std::string getCurrentDay(void) noexcept;
    };

}

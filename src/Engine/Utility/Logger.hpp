#pragma once

#include <sstream>
#include <vector>

namespace ge {

    class Logger {
    public:
        static void saveOpenGl(const std::string& data, const std::string& title) noexcept;
        static std::string loggingString(std::size_t size, char fill) noexcept;
    private:
        static std::stringstream getTimeInString(void) noexcept;
        static std::string getCurrentDay(void) noexcept;
    };

}

#include "Logger.hpp"
#include "FileIO.hpp"
#include "Ranges.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace ge {

    void Logger::saveOpenGl(const std::string& infoLog, const std::string& title) noexcept {
        if (!accumulate(infoLog, 0u))
            return;
        std::stringstream ss = getTimeInString();
        ss << '[' << title << ']' << ' ' << infoLog << '\n';
        std::cout << ss.rdbuf();
        FileIO::saveFile("logs/" + getCurrentDay() + ".log", std::move(ss), std::ios_base::app);
    }

    std::string Logger::loggingString(std::size_t size, char fill) noexcept {
        std::string info;
        info.resize(size);
        std::fill(info.begin(), info.end(), fill);
        return info;
    }

    std::stringstream Logger::getTimeInString(void) noexcept {
        auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << '[' << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << ']';
        return ss;
    }

    std::string Logger::getCurrentDay(void) noexcept {
        auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
        return ss.str();
    }

}

#include "Logger.hpp"
#include "FileIO.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace ge {

    void Logger::saveOpenGl(GLchar* infoLog, size_t size) noexcept {
        std::stringstream ss = getTimeInString();
        for (char* i = infoLog; i != &infoLog[size]; ++i) ss << i;
        ss << '\n';
        std::cout << ss.rdbuf();
        FileIO::saveFile("logs/" + getCurrentDay() + ".log", std::move(ss), std::ios_base::app);
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

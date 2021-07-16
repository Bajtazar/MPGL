#pragma once

#include <optional>
#include <sstream>
#include <fstream>
#include <vector>

namespace ge {

    struct FileIO {
        using stream = std::stringstream;

        static std::vector<std::string> getAllDirectoryFiles(const std::string& dirName) noexcept;
        static std::optional<stream> readFile(const std::string& fileName) noexcept;
        static void saveFile(const std::string& fileName, stream&& dataStream, std::ios_base::openmode mode = std::ios_base::trunc) noexcept;
    };

}

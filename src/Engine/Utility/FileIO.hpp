#pragma once

#include <optional>
#include <sstream>
#include <fstream>

namespace ge {

    struct FileIO {
        using stream = std::stringstream;

        static std::optional<stream> readFile(const std::string& fileName) noexcept;
        static void saveFile(const std::string& fileName, stream&& dataStream, std::ios_base::openmode mode = std::ios_base::trunc) noexcept;
    };

}

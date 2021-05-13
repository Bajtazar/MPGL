#pragma once

#include <optional>
#include <sstream>
#include <fstream>

namespace ge {

    struct FileIO {
        using stream = std::stringstream;

        static std::optional<stream> readFile(const std::string& fileName) noexcept;
    };

}

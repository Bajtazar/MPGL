#include "FileIO.hpp"

#include <algorithm>
#include <iterator>

namespace ge {

    std::optional<FileIO::stream> FileIO::readFile(const std::string& fileName) noexcept {
        std::ifstream file(fileName.c_str());
        if (file.is_open() && file.good()) {
            stream dataStream;
            std::copy(std::istreambuf_iterator<char>(file), {}, std::ostreambuf_iterator<char>(dataStream));
            return dataStream;
        }
        return {};
    }

}

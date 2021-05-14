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

    void FileIO::saveFile(const std::string& fileName, stream&& dataStream, std::ios_base::openmode mode) noexcept {
        std::ofstream file(fileName.c_str(), std::move(mode));
        if (file.is_open() && file.good())
            file << dataStream.rdbuf();
    }

}

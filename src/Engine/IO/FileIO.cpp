#include "FileIO.hpp"

#include <filesystem>
#include <algorithm>
#include <iterator>

namespace mpgl {

    std::vector<std::string> FileIO::getAllDirectoryFiles(const std::string& dirName) noexcept {
        std::vector<std::string> files;
        std::ranges::transform(std::filesystem::directory_iterator(dirName), std::back_inserter(files), [](auto& path){ return path.path(); });
        return files;
    }

    std::vector<std::string> FileIO::getRecursiveDirFiles(const std::string& dirName) noexcept {
        std::vector<std::string> files;
        std::ranges::transform(std::filesystem::recursive_directory_iterator(dirName), std::back_inserter(files), [](auto& path){ return path.path(); });
        return files;
    }

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

/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include "FileIO.hpp"

#include <filesystem>
#include <algorithm>
#include <iterator>
#include <ranges>

namespace mpgl {

    [[nodiscard]] std::vector<FileIO::Path>
        FileIO::getAllDirectoryFiles(Path const& dirPath)
    {
        std::vector<std::string> files;
        std::ranges::transform(
            std::filesystem::directory_iterator(dirPath),
            std::back_inserter(files),
            [](auto& path){ return path.path(); });
        return files;
    }

    [[nodiscard]] std::vector<FileIO::Path>
        FileIO::getRecursiveDirFiles(Path const& dirPath)
    {
        namespace fs = std::filesystem;

        std::vector<std::string> files;
        for (auto& entry : fs::recursive_directory_iterator(dirPath))
            if (!fs::is_directory(entry))
                files.emplace_back(entry.path());
        return files;
    }

    [[nodiscard]] std::optional<FileIO::Stream>
        FileIO::readFile(Path const& filePath)
    {
        std::ifstream file(filePath.c_str());
        if (file.is_open() && file.good()) {
            Stream dataStream;
            std::copy(std::istreambuf_iterator<char>(file), {},
                std::ostreambuf_iterator<char>(dataStream));
            return dataStream;
        }
        return {};
    }

    [[nodiscard]] std::size_t FileIO::fileSize(
        std::istream& is) noexcept
    {
        std::streampos fileSize;
        is.seekg(0, std::ios::end);
        fileSize = is.tellg();
        is.seekg(0, std::ios::beg);
        return fileSize;
    }

    [[nodiscard]] std::optional<FileIO::Buffer>
        FileIO::readFileToVec(Path const& filePath)
    {
        std::ifstream file{filePath.c_str(), std::ios::binary};
        file.unsetf(std::ios::skipws);
        if (file.is_open() && file.good()) {
            Buffer dataStream;
            dataStream.reserve(fileSize(file));
            dataStream.insert(dataStream.begin(),
                std::istream_iterator<char>(file), {});
            return { dataStream };
        }
        return {};
    }

    bool FileIO::saveFile(
        Path const& filePath,
        Stream&& dataStream,
        Ios::openmode mode) noexcept
    {
        std::ofstream file(filePath.c_str(), std::move(mode));
        if (!file.is_open() || !file.good())
            return false;
        file << dataStream.rdbuf();
        return true;
    }

}

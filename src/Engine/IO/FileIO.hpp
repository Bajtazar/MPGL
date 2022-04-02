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
#pragma once

#include <optional>
#include <sstream>
#include <fstream>
#include <vector>

namespace mpgl {

    /**
     * Helps with IO operations on files
     */
    struct FileIO {
        typedef std::stringstream                   Stream;
        typedef std::ios_base                       Ios;
        typedef std::string                         Path;

        /**
         * Reads all directory files into the vector
         *
         * @param dirPath the path to the directory
         * @return the vector with paths
         */
        [[nodiscard]] static std::vector<Path> getAllDirectoryFiles(
            Path const& dirPath);

        /**
         * Recursively reads all directory files into the vector
         *
         * @param dirPath the path to the directory
         * @return the vector with paths
         */
        [[nodiscard]] static std::vector<Path> getRecursiveDirFiles(
            Path const& dirPath);
        /**
         * Reads the file concent into an optional string stream.
         * If file cannot be open then returns an empty optional
         *
         * @param filePath the path to the file
         * @return the optional with the string stream
         */
        [[nodiscard]] static std::optional<Stream> readFile(
            Path const& filePath);

        /**
         * Saves the string stream into the given file with the
         * given mode
         *
         * @param filePath the path to the file
         * @param dataStream the rvalue reference to the string
         * stream object
         * @param mode the file opening mode
         * @return if file was saved
         */
        static bool saveFile(
            Path const& filePath,
            Stream&& dataStream,
            Ios::openmode mode = Ios::trunc) noexcept;
    };

}

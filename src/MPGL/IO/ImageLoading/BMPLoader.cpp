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
#include <MPGL/Exceptions/ImageLoading/ImageLoadingFileCorruptionException.hpp>
#include <MPGL/Exceptions/ImageLoading/ImageLoadingInvalidTypeException.hpp>
#include <MPGL/Exceptions/ImageLoading/ImageLoadingFileOpenException.hpp>
#include <MPGL/Exceptions/SecurityUnknownPolicyException.hpp>
#include <MPGL/IO/ImageLoading/BMPLoader.hpp>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::Path const BMPLoader<Policy>::Tag{"bmp"};

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::BMPLoader(
        [[maybe_unused]] Policy policy,
        Path const& filePath)
            : LoaderInterface{filePath}
    {
        std::ifstream file{this->filePath.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->filePath};
        FileIter iter = setPolicy(file);
        try {
            readHeader(iter);
            readImage(iter);
        } catch (std::out_of_range&) {
            throw ImageLoadingFileCorruptionException{this->filePath};
        }
    }

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::BMPLoader(Path const& filePath)
        : BMPLoader{Policy{}, filePath} {}

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::FileIter BMPLoader<Policy>::setPolicy(
        std::istream& file)
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return FileIter{StreamBuf{file}, StreamBuf{}};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return FileIter{file};
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void BMPLoader<Policy>::readHeader(FileIter& file) {
        if (readType<uint16>(file) != 0x4D42)
            throw ImageLoadingInvalidTypeException{filePath};
        std::advance(file, 8); // file size and two reserved fields
        uint32 offset = readType<uint32>(file)
            - 6 * sizeof(uint32) - sizeof(uint16);
        std::advance(file, 4);   // DIB header
        pixels.resize(readType<uint32>(file), readType<uint32>(file));
        std::advance(file, offset);
    }

    template <security::SecurityPolicy Policy>
    void BMPLoader<Policy>::readImage(FileIter& file) {
        for (auto& row : pixels) {
            for (auto& pixel : row)
                Pixel::Manip::RGB(file, pixel);
            std::advance(file, (4 - pixels.getWidth() % 4) % 4);
        }
    }

}

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
#include <MPGL/Exceptions/NotSupportedException.hpp>
#include <MPGL/Exceptions/InflateException.hpp>
#include <MPGL/Compression/Checksums/CRC32.hpp>
#include <MPGL/IO/ImageLoading/PNGLoader.hpp>
#include <MPGL/Compression/ZlibDecoder.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <iterator>
#include <numeric>
#include <ranges>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::Path const PNGLoader<Policy>::Tag{"png"};

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::PNGLoader(
        Policy policy,
        Path const& filePath)
            : LoaderInterface{filePath}
    {
        if (auto file = FileIO::readFileToVec(this->filePath)) {
            try {
                setPolicy(*file, policy);
            } catch (std::out_of_range&) {
                throw ImageLoadingFileCorruptionException{this->filePath};
            } catch (InflateException&) {
                throw ImageLoadingFileCorruptionException{this->filePath};
            }
        } else
            throw ImageLoadingFileOpenException{this->filePath};
    }

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::PNGLoader(Path const& filePath)
        : PNGLoader{Policy{}, filePath} {}

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setPolicy(
        DataBuffer const& file,
        Policy policy)
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return readImage(policy, FileIter{file.begin(),
               file.end()});
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return readImage(policy, FileIter{file.begin()});
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::checkCRCCode(
        FileIter begin,
        size_type length)
    {
        auto const end = std::next(begin, length + 4);
        uint32 crc = crc32(std::ranges::subrange{begin, end});
        if (peekType<uint32, true>(end) != crc)
            throw ImageLoadingFileCorruptionException{filePath};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::readImage(Policy policy, FileIter file) {
        if (readType<uint64>(file) != MagicNumber)
            throw ImageLoadingInvalidTypeException{filePath};
        while (auto length = readType<uint32, true>(file))
            parseChunk(file, length);
        if (readType<uint64>(file) != IENDNumber)
            throw ImageLoadingFileCorruptionException{filePath};
        chooseInterlance(policy);
    }

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::FileIter PNGLoader<Policy>::decompresedIter(
        DataBuffer const& buffer) const noexcept
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return FileIter{buffer.begin(), buffer.end()};
        else
            return FileIter{buffer.begin()};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::chooseInterlance(Policy policy) {
        auto decoded = ZlibDecoder{std::move(rawFileData), policy}();
        auto iter = decompresedIter(decoded);
        if (headerData.interlance)
            return interlance(policy, iter);
        Filters{pixels, *this}(iter);
    }

    template <security::SecurityPolicy Policy>
    Vector2<typename PNGLoader<Policy>::size_type>
        PNGLoader<Policy>::subimageDimensions(
            uint32 startX,
            uint32 startY,
            uint32 incrementX,
            uint32 incrementY) const noexcept
    {
        size_type width = std::ceil(
            (float64) (pixels.getWidth() - startX) / incrementX);
        size_type height = std::ceil(
            (float64) (pixels.getWidth() - startY) / incrementY);
        return { width, height };
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::interlance(
        Policy policy,
        FileIter& iter)
    {
        for (auto const& [inY, inX, startY, startX] : InterlanceCoeff) {
            Image image{subimageDimensions(startX, startY, inX, inY)};
            Filters{image, *this}(iter);
            for (size_type y = startY, i = 0; y < pixels.getHeight();
                y += inY, ++i)
            {
                for (size_type x = startX, j = 0;
                    x < pixels.getWidth(); x += inX, ++j)
                        pixels[y][x] = image[i][j];
            }
        }
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::parseChunk(
        FileIter& file,
        size_type length)
    {
        checkCRCCode(file, length);
        if (auto iter = chunkParsers.find(readNChars(4, file));
            iter != chunkParsers.end())
                (*(iter->second)(std::ref(*this)))(length, file);
        else
            std::advance(file, length);
        std::advance(file, 4); // CRC is already checked
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::operator() (
        [[maybe_unused]] size_type length,
        FileIter& data)
    {
        uint32 width = readType<uint32, true>(data);
        this->loader.pixels.resize(width, readType<uint32, true>(data));
        parseBitDepth(readType<uint8>(data));
        parseColorType(readType<uint8>(data));
        std::advance(data, 2); // compression method and filter method
        parseInterlance(readType<uint8>(data));
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseBitDepth(uint8 bitDepth) {
        if (bitDepth != 0x08)
            throw NotSupportedException{
                "Not 8-bit pixel fortmats are not supported"};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseColorType(
        uint8 colorType)
    {
        auto iter = colorSetters.find(colorType);
        if (iter == colorSetters.end())
            throw NotSupportedException{
                "Following PNG image type is not supported"};
        this->loader.headerData.setter = iter->second;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseInterlance(
        uint8 interlance)
    {
        this->loader.headerData.interlance = interlance > 0;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IDATChunk::operator() (
        size_type length,
        FileIter& data)
    {
        this->loader.rawFileData.resize(
            this->loader.rawFileData.size() + length);
        copyTo(data, std::next(data, length),
            this->loader.rawFileData.end() - length);
        std::advance(data, length);
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::paethPredictor(
        uint8 a,
        uint8 b,
        uint8 c) const noexcept
    {
        uint16 paethA = b > c ? b - c : c - b;
        uint16 paethB = a > c ? a - c : c - a;
        uint16 paethC = ((uint16) a + b) > ((uint16) 2 * c) ?
            (uint16) a + b - 2 * c : (uint16) 2 * c - a - b;
        if (paethA <= paethB && paethA <= paethC)
            return a;
        return (paethB <= paethC) ? b : c;
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::reconstructA(
        size_type row,
        size_type column,
        uint8 pixel) const noexcept
    {
        return column ? image[row][column - 1][pixel] : 0;
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::reconstructB(
        size_type row,
        size_type column,
        uint8 pixel) const noexcept
    {
        return (row < image.getHeight() - 1) ?
            image[row + 1][column][pixel] : 0;
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::reconstructC(
        size_type row,
        size_type column,
        uint8 pixel) const noexcept
    {
        return (row < image.getHeight() - 1) && column ?
            image[row + 1][column - 1][pixel] : 0;
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::subpixelFilterA(
        size_type row,
        size_type column,
        uint8 subpixelID,
        uint8 subpixel) const noexcept
    {
        return subpixel + reconstructA(row, column, subpixelID);
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::subpixelFilterB(
        size_type row,
        size_type column,
        uint8 subpixelID,
        uint8 subpixel) const noexcept
    {
        return subpixel + reconstructB(row, column, subpixelID);
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::subpixelFilterC(
        size_type row,
        size_type column,
        uint8 subpixelID,
        uint8 subpixel) const noexcept
    {
        return subpixel
            + ((uint16)reconstructA(row, column, subpixelID)
            + reconstructB(row, column, subpixelID)) / 2;
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::subpixelFilterD(
        size_type row,
        size_type column,
        uint8 subpixelID,
        uint8 subpixel) const noexcept
    {
        return subpixel + paethPredictor(
            reconstructA(row, column, subpixelID),
            reconstructB(row, column, subpixelID),
            reconstructC(row, column, subpixelID));
    }

    template <security::SecurityPolicy Policy>
    uint8 PNGLoader<Policy>::Filters::filterSubpixel(
        size_type row,
        size_type column,
        uint8 filter,
        uint8 subpixelID,
        FileIter& iter) noexcept
    {
        switch (filter) {
            case 1:
                return subpixelFilterA(row, column, subpixelID, *iter++);
            case 2:
                return subpixelFilterB(row, column, subpixelID, *iter++);
            case 3:
                return subpixelFilterC(row, column, subpixelID, *iter++);
            case 4:
                return subpixelFilterD(row, column, subpixelID, *iter++);
        }
        return *iter++;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::Filters::setRGBAPixels(
        size_type row,
        size_type column,
        uint8 filter,
        FileIter& iter) noexcept
    {
        for (uint8 sub = 0; sub < 4; ++sub)
            image[row][column][sub]
                = filterSubpixel(row, column, filter, sub, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::Filters::setRGBPixels(
        size_type row,
        size_type column,
        uint8 filter,
        FileIter& iter) noexcept
    {
        for (uint8 sub = 0; sub < 3; ++sub)
            image[row][column][sub]
                = filterSubpixel(row, column, filter, sub, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::Filters::setGrayPixels(
        size_type row,
        size_type column,
        uint8 filter,
        FileIter& iter) noexcept
    {
        uint8 subpixel = filterSubpixel(row, column, filter, 0, iter);
        for (uint8 sub = 0; sub < 3; ++sub)
            image[row][column][sub] = subpixel;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::Filters::setGrayAlphaPixels(
        size_type row,
        size_type column,
        uint8 filter,
        FileIter& iter) noexcept
    {
        setGrayPixels(row, column, filter, iter);
        image[row][column].alpha
            = filterSubpixel(row, column, filter, 3, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::Filters::operator()(
        FileIter& iter) noexcept
    {
        for (std::size_t i = image.getHeight() - 1;
            i < image.getHeight(); --i)
        {
            uint8 filter = *iter++;
            for (std::size_t j = 0;j < image.getWidth(); ++j)
                (this->*loader.headerData.setter)(i, j, filter, iter);
        }
    }

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::ColorSetters const
        PNGLoader<Policy>::IHDRChunk::colorSetters
    {
        {0, &PNGLoader::Filters::setGrayPixels},
        {2, &PNGLoader::Filters::setRGBPixels},
        {4, &PNGLoader::Filters::setGrayAlphaPixels},
        {6, &PNGLoader::Filters::setRGBAPixels}
    };

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::ChunkMap const PNGLoader<Policy>::chunkParsers {
        {"IHDR", DeferredConstructor<PNGLoader::IHDRChunk, PNGLoader::ChunkInterface>{}},
        {"IDAT", DeferredConstructor<PNGLoader::IDATChunk, PNGLoader::ChunkInterface>{}}
    };

}

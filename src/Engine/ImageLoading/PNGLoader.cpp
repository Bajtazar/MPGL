#include "PNGLoader.hpp"

#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/NotSupportedException.hpp"
#include "../Compression/DeflateDecoder.hpp"

#include <numeric>
#include <ranges>

namespace ge {

    const std::string PNGLoader::Tag{"png"};
    const std::array<uint32_t, 256> PNGLoader::crcTable{ PNGLoader::createCRCTable() };

    PNGLoader::PNGLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        readImage(file);
    }

    void PNGLoader::readImage(std::ifstream& file) {
        if (readType<uint64_t>(file) != 0x0A1A0A0D474E5089)
            throw ImageLoadingInvalidTypeException{fileName};
        while (auto length = readType<uint32_t, true>(file)) {
            if (auto iter = chunkParsers.find(readNChars(4, file)); iter != chunkParsers.end())
                std::invoke(*std::invoke(iter->second, std::ref(*this)), length, file);
            else
                ignoreNBytes(length + 4, file);
        }
        if (readType<uint64_t>(file) != 0x826042AE444E4549)
            throw ImageLoadingFileCorruptionException{fileName};
        DeflateDecoder decoder{rawFileData};
        filterPixels(decoder.decompress());
    }

    std::array<uint32_t, 256> PNGLoader::createCRCTable(void) noexcept {
        std::array<uint32_t, 256> table;
        std::ranges::for_each(table, [i = 0](auto& value) mutable -> void {
            value = i++;
            for ([[maybe_unused]] auto j : std::views::iota(0, 8)) {
                if (value & 1)
                    value = 0xEDB88320 ^ (value >> 1);
                else
                    value >>= 1;
            }
        });
        return table;
    }

    void PNGLoader::ChunkInterface::checkCRCCode(const uint32_t& crc) const {
        if ((crcCode ^ 0xFFFFFFFF) != crc)
            throw ImageLoadingFileCorruptionException{loader.fileName};
    }

    void PNGLoader::IHDRChunk::operator() ([[maybe_unused]] std::size_t length, std::istream& data) {
        loader.pixels.resize(readType<uint32_t, true>(data), readType<uint32_t, true>(data));
        readCRCCode(crcCode, static_cast<uint32_t>(loader.pixels.getWidth()));
        readCRCCode(crcCode, static_cast<uint32_t>(loader.pixels.getHeight()));
        parseBitDepth(readType<uint8_t>(data));
        parseColorType(readType<uint8_t>(data));
        readCRCCode(crcCode, readType<uint16_t, true>(data)); // compresion method and filter method
        parseInterlance(readType<uint8_t>(data));
        checkCRCCode(readType<uint32_t, true>(data));
    }

    void PNGLoader::IHDRChunk::parseBitDepth(const uint8_t& bitDepth) {
        if (bitDepth != 0x08)
            throw NotSupportedException{"not 8-bit pixel fortmats are not supported"};
        readCRCCode(crcCode, bitDepth);
    }

    void PNGLoader::IHDRChunk::parseColorType(const uint8_t& colorType) {
        readCRCCode(crcCode, colorType);
        switch (colorType) {
            case 0:
                loader.headerData.type = PNGLoader::HeaderData::Types::GRAY;
                return;
            case 2:
                loader.headerData.type = PNGLoader::HeaderData::Types::RGB;
                return;
            case 4:
                loader.headerData.type = PNGLoader::HeaderData::Types::GRAYALPHA;
                return;
            case 6:
                loader.headerData.type = PNGLoader::HeaderData::Types::RGBA;
                return;
            default:
                throw NotSupportedException{"following PNG image type is not supported"};
        }
    }

    void PNGLoader::IHDRChunk::parseInterlance(const uint8_t& interlance) {
        if (interlance)
            throw NotSupportedException{"Adam7 interlance in PNG files is not supported"};
        readCRCCode(crcCode, interlance);
    }

    void PNGLoader::IDATChunk::operator() (std::size_t length, std::istream& data) {
        loader.rawFileData.resize(length);
        for (char& byte : loader.rawFileData) {
            data.get(byte);
            crcCode = crcTable[(crcCode ^ byte) & 0xFF] ^ (crcCode >> 8);
        }
        checkCRCCode(readType<uint32_t, true>(data));
    }

    uint8_t PNGLoader::paethPredictor(uint8_t a, uint8_t b, uint8_t c) const noexcept {
        uint16_t paethA = b > c ? b - c : c - b;
        uint16_t paethB = a > c ? a - c : c - a;
        uint16_t paethC = ((uint16_t) a + b) > ((uint16_t) 2 * c) ?
            (uint16_t) a + b - 2 * c : (uint16_t) 2 * c - a - b;
        if (paethA <= paethB && paethA <= paethC)
            return a;
        return (paethB <= paethC) ? b : c;
    }

    uint8_t PNGLoader::reconstructA(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return column ? pixels[row][column - 1][pixel] : 0;
    }
    uint8_t PNGLoader::reconstructB(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return (row < pixels.getHeight() - 1) ? pixels[row + 1][column][pixel] : 0;
    }
    uint8_t PNGLoader::reconstructC(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return (row < pixels.getHeight() - 1) && column < pixels.getHeight() - 1 ? pixels[row + 1][column - 1][pixel] : 0;
    }

    void PNGLoader::setRGBAPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        for (uint8_t k = 0; k < 4; ++k, ++iter) {
            auto& subpixel = pixels[row][column][k];
            subpixel = *iter;
            if (filter == 1)
                subpixel += reconstructA(row, column, k);
            else if (filter == 2)
                subpixel += reconstructB(row, column, k);
            else if (filter == 3)
                subpixel += ((uint16_t) reconstructA(row, column, k) + reconstructB(row, column, k)) / 2;
            else if (filter == 4)
                subpixel += paethPredictor(reconstructA(row, column, k),
                    reconstructB(row, column, k), reconstructC(row, column, k));
        }
    }
    void PNGLoader::setRGBPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        for (uint8_t k = 0; k < 3; ++k, ++iter) {
            auto& subpixel = pixels[row][column][k];
            subpixel = *iter;
            if (filter == 1)
                subpixel += reconstructA(row, column, k);
            else if (filter == 2)
                subpixel += reconstructB(row, column, k);
            else if (filter == 3)
                subpixel += ((uint16_t) reconstructA(row, column, k) + reconstructB(row, column, k)) / 2;
            else if (filter == 4)
                subpixel += paethPredictor(reconstructA(row, column, k),
                    reconstructB(row, column, k), reconstructC(row, column, k));
        }
    }

    void PNGLoader::setGrayPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        uint8_t subpixel = *iter++;
        if (filter == 1)
            subpixel += reconstructA(row, column, 0);
        else if (filter == 2)
            subpixel += reconstructB(row, column, 0);
        else if (filter == 3)
            subpixel += ((uint16_t) reconstructA(row, column, 0) + reconstructB(row, column, 0)) / 2;
        else if (filter == 4)
            subpixel += paethPredictor(reconstructA(row, column, 0),
                reconstructB(row, column, 0), reconstructC(row, column, 0));
        for (uint8_t sub = 0; sub < 3; ++sub)
            pixels[row][column][sub] = subpixel;
    }

    void PNGLoader::setGrayAlphaPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        setGrayPixels(row, column, filter, iter);
        uint8_t& subpixel = pixels[row][column].alpha;
        subpixel = *iter++;
        if (filter == 1)
            subpixel += reconstructA(row, column, 3);
        else if (filter == 2)
            subpixel += reconstructB(row, column, 3);
        else if (filter == 3)
            subpixel += ((uint16_t) reconstructA(row, column, 3) + reconstructB(row, column, 3)) / 2;
        else if (filter == 4)
            subpixel += paethPredictor(reconstructA(row, column, 3),
                reconstructB(row, column, 3), reconstructC(row, column, 3));
    }

    PNGLoader::PixelsSetter PNGLoader::getPixelsSetter(void) const noexcept {
        switch (headerData.type) {
            case HeaderData::Types::RGBA:
                return &PNGLoader::setRGBAPixels;
            case HeaderData::Types::RGB:
                return &PNGLoader::setRGBPixels;
            case HeaderData::Types::GRAY:
                return &PNGLoader::setGrayPixels;
            case HeaderData::Types::GRAYALPHA:
                return &PNGLoader::setGrayAlphaPixels;
        }
        return &PNGLoader::setRGBAPixels;
    }

    void PNGLoader::filterPixels(const std::vector<char>& data) noexcept {
        auto iter = data.begin();
        PixelsSetter setter = getPixelsSetter();
        for (std::size_t i = pixels.getHeight() - 1;i < pixels.getHeight(); --i) {
            uint8_t filter = *iter++;
            for (std::size_t j = 0;j < pixels.getWidth(); ++j)
                (this->*setter)(i, j, filter, iter);
        }
    }

    const std::map<std::string, std::function<std::unique_ptr<PNGLoader::ChunkInterface> (PNGLoader&)>> PNGLoader::chunkParsers {
        {"IHDR", FunctionalWrapper<PNGLoader::IHDRChunk, PNGLoader::ChunkInterface>{}},
        {"IDAT", FunctionalWrapper<PNGLoader::IDATChunk, PNGLoader::ChunkInterface>{}}
    };

}

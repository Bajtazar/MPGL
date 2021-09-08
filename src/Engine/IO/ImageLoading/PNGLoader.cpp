#include "PNGLoader.hpp"

#include "../../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../../Exceptions/DeflateDecoderException.hpp"
#include "../../Exceptions/NotSupportedException.hpp"
#include "../../Compression/DeflateDecoder.hpp"
#include <iostream>

#include <numeric>
#include <ranges>

namespace ge {

    template <security::SecurityPolicy Policy>
    const std::string PNGLoader<Policy>::Tag{"png"};

    template <security::SecurityPolicy Policy>
    const std::array<uint32_t, 256> PNGLoader<Policy>::crcTable{ PNGLoader<Policy>::createCRCTable() };

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::PNGLoader(Policy policy, const std::string& fileName)
        : LoaderInterface{std::move(fileName)}
    {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        try {
            setPolicy(file, policy);
        } catch (std::out_of_range&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        } catch (DeflateDecoderException&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        }
    }

    template <security::SecurityPolicy Policy>
    PNGLoader<Policy>::PNGLoader(const std::string& fileName) : PNGLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setPolicy(std::istream& file, Policy policy) {
        if constexpr (security::isSecurePolicy<Policy>)
            return readImage(policy, FileIter{StreamBuf{file}, StreamBuf{}});
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return readImage(policy, FileIter{file});
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::readImage(Policy policy, FileIter file) {
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
        DeflateDecoder decoder{policy, rawFileData};
        filterPixels(decoder.decompress());
    }

    template <security::SecurityPolicy Policy>
    std::array<uint32_t, 256> PNGLoader<Policy>::createCRCTable(void) noexcept {
        std::array<uint32_t, 256> table;
        std::ranges::for_each(table, [i = 0](auto& value) mutable -> void {
            value = i++;
            for ([[maybe_unused]] auto _ : std::views::iota(0, 8)) {
                if (value & 1)
                    value = 0xEDB88320 ^ (value >> 1);
                else
                    value >>= 1;
            }
        });
        return table;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::ChunkInterface::checkCRCCode(const uint32_t& crc) const {
        if ((crcCode ^ 0xFFFFFFFF) != crc)
            throw ImageLoadingFileCorruptionException{loader.fileName};
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::operator() ([[maybe_unused]] std::size_t length, FileIter& data) {
        uint32_t width = readType<uint32_t, true>(data);
        this->loader.pixels.resize(width, readType<uint32_t, true>(data));
        readCRCCode(this->crcCode, static_cast<uint32_t>(this->loader.pixels.getWidth()));
        readCRCCode(this->crcCode, static_cast<uint32_t>(this->loader.pixels.getHeight()));
        parseBitDepth(readType<uint8_t>(data));
        parseColorType(readType<uint8_t>(data));
        readCRCCode(this->crcCode, readType<uint16_t, true>(data)); // compression method and filter method
        parseInterlance(readType<uint8_t>(data));
        this->checkCRCCode(readType<uint32_t, true>(data));
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseBitDepth(const uint8_t& bitDepth) {
        if (bitDepth != 0x08)
            throw NotSupportedException{"Not 8-bit pixel fortmats are not supported"};
        readCRCCode(this->crcCode, bitDepth);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseColorType(const uint8_t& colorType) {
        readCRCCode(this->crcCode, colorType);
        switch (colorType) {
            case 0:
                this->loader.headerData.setter = &PNGLoader::setGrayPixels;
                return;
            case 2:
                this->loader.headerData.setter = &PNGLoader::setRGBPixels;
                return;
            case 4:
                this->loader.headerData.setter = &PNGLoader::setGrayAlphaPixels;
                return;
            case 6:
                this->loader.headerData.setter = &PNGLoader::setRGBAPixels;
                return;
            default:
                throw NotSupportedException{"Following PNG image type is not supported"};
        }
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IHDRChunk::parseInterlance(const uint8_t& interlance) {
        if (interlance)
            throw NotSupportedException{"Adam7 interlance in PNG files is not supported"};
        readCRCCode(this->crcCode, interlance);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::IDATChunk::operator() (std::size_t length, FileIter& data) {
        this->loader.rawFileData.resize(length);
        for (char& byte : this->loader.rawFileData) {
            byte = *data++;
            this->crcCode = PNGLoader<Policy>::crcTable[(this->crcCode ^ byte) & 0xFF] ^ (this->crcCode >> 8);
        }
        this->checkCRCCode(readType<uint32_t, true>(data));
    }

    template <security::SecurityPolicy Policy>
    uint8_t PNGLoader<Policy>::paethPredictor(uint8_t a, uint8_t b, uint8_t c) const noexcept {
        uint16_t paethA = b > c ? b - c : c - b;
        uint16_t paethB = a > c ? a - c : c - a;
        uint16_t paethC = ((uint16_t) a + b) > ((uint16_t) 2 * c) ?
            (uint16_t) a + b - 2 * c : (uint16_t) 2 * c - a - b;
        if (paethA <= paethB && paethA <= paethC)
            return a;
        return (paethB <= paethC) ? b : c;
    }

    template <security::SecurityPolicy Policy>
    uint8_t PNGLoader<Policy>::reconstructA(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return column ? pixels[row][column - 1][pixel] : 0;
    }

    template <security::SecurityPolicy Policy>
    uint8_t PNGLoader<Policy>::reconstructB(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return (row < pixels.getHeight() - 1) ? pixels[row + 1][column][pixel] : 0;
    }

    template <security::SecurityPolicy Policy>
    uint8_t PNGLoader<Policy>::reconstructC(std::size_t row, std::size_t column, uint8_t pixel) const noexcept {
        return (row < pixels.getHeight() - 1) && column < pixels.getHeight() - 1 ? pixels[row + 1][column - 1][pixel] : 0;

    }

    template <security::SecurityPolicy Policy>
    uint8_t PNGLoader<Policy>::filterSubpixel(std::size_t row, std::size_t column, uint8_t filter, uint8_t subpixelID, CharIter& iter) noexcept {
        uint8_t subpixel = *iter++;
        if (filter == 1)
            subpixel += reconstructA(row, column, subpixelID);
        else if (filter == 2)
            subpixel += reconstructB(row, column, subpixelID);
        else if (filter == 3)
            subpixel += ((uint16_t) reconstructA(row, column, subpixelID) + reconstructB(row, column, subpixelID)) / 2;
        else if (filter == 4)
            subpixel += paethPredictor(reconstructA(row, column, subpixelID),
                reconstructB(row, column, subpixelID), reconstructC(row, column, subpixelID));
        return subpixel;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setRGBAPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        for (uint8_t sub = 0; sub < 4; ++sub)
            pixels[row][column][sub] = filterSubpixel(row, column, filter, sub, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setRGBPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        for (uint8_t sub = 0; sub < 3; ++sub)
            pixels[row][column][sub] = filterSubpixel(row, column, filter, sub, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setGrayPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        uint8_t subpixel = filterSubpixel(row, column, filter, 0, iter);
        for (uint8_t sub = 0; sub < 3; ++sub)
            pixels[row][column][sub] = subpixel;
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::setGrayAlphaPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept {
        setGrayPixels(row, column, filter, iter);
        pixels[row][column].alpha = filterSubpixel(row, column, filter, 3, iter);
    }

    template <security::SecurityPolicy Policy>
    void PNGLoader<Policy>::filterPixels(const std::vector<char>& data) noexcept {
        auto iter = data.begin();
        for (std::size_t i = pixels.getHeight() - 1;i < pixels.getHeight(); --i) {
            uint8_t filter = *iter++;
            for (std::size_t j = 0;j < pixels.getWidth(); ++j)
                (this->*headerData.setter)(i, j, filter, iter);
        }
    }

    template <security::SecurityPolicy Policy>
    const std::map<std::string, std::function<std::unique_ptr<typename PNGLoader<Policy>::ChunkInterface> (PNGLoader<Policy>&)>>
        PNGLoader<Policy>::chunkParsers
    {
        {"IHDR", FunctionalWrapper<PNGLoader::IHDRChunk, PNGLoader::ChunkInterface>{}},
        {"IDAT", FunctionalWrapper<PNGLoader::IDATChunk, PNGLoader::ChunkInterface>{}}
    };

}

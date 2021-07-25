#include "PNGLoader.hpp"

#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/NotSupportedException.hpp"

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
                std::invoke(*std::invoke(iter->second), length, file, *this);
            else
                ignoreNBytes(length + 4, file);
        }
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

    void PNGLoader::ChunkInterface::checkCRCCode(const uint32_t& crc, const PNGLoader& loader) {
        if ((crcCode ^ 0xFFFFFFFF) != crc)
            throw ImageLoadingFileCorruptionException{loader.fileName};
    }

    void PNGLoader::IHDRChunk::operator() ([[maybe_unused]] std::size_t length, std::istream& data, PNGLoader& loader) {
        loader.pixels.resize(readType<uint32_t, true>(data), readType<uint32_t, true>(data));
        readCRCCode(crcCode, static_cast<uint32_t>(loader.pixels.getWidth()));
        readCRCCode(crcCode, static_cast<uint32_t>(loader.pixels.getHeight()));
        parseBitDepth(readType<uint8_t>(data), loader);
        parseColorType(readType<uint8_t>(data), loader);
        readCRCCode(crcCode, readType<uint16_t, true>(data)); // compresion method and filter method
        parseInterlance(readType<uint8_t>(data), loader);
        checkCRCCode(readType<uint32_t, true>(data), loader);
    }

    void PNGLoader::IHDRChunk::parseBitDepth(const uint8_t& bitDepth, [[maybe_unused]] PNGLoader& loader) {
        if (bitDepth != 0x08)
            throw NotSupportedException{"not 8-bit pixel fortmats are not supported"};
        readCRCCode(crcCode, bitDepth);
    }

    void PNGLoader::IHDRChunk::parseColorType(const uint8_t& colorType, PNGLoader& loader) {
        readCRCCode(crcCode, colorType);
        switch (colorType) {
            case 2:
                loader.headerData.type = PNGLoader::HeaderData::Types::RGB;
                return;
            case 6:
                loader.headerData.type = PNGLoader::HeaderData::Types::RGBA;
                return;
            default:
                throw NotSupportedException{"following PNG image type is not supported"};
        }
    }

    void PNGLoader::IHDRChunk::parseInterlance(const uint8_t& interlance, [[maybe_unused]] PNGLoader& loader) {
        if (interlance)
            throw NotSupportedException{"Adam7 interlance in PNG files is not supported"};
        readCRCCode(crcCode, interlance);
    }

    const std::map<std::string, std::function<std::unique_ptr<PNGLoader::ChunkInterface> (void)>> PNGLoader::chunkParsers {
        {"IHDR", FunctionalWrapper<PNGLoader::IHDRChunk, PNGLoader::ChunkInterface>{}}
    };

}

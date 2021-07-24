#include "PNGLoader.hpp"

#include "../Exceptions/ImageLoadingFileException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"

namespace ge {

    const std::string PNGLoader::Tag{"png"};

    PNGLoader::PNGLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileException{this->fileName};
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

    void PNGLoader::IHDRChunk::operator() ([[maybe_unused]] std::size_t length, std::istream& data, PNGLoader& loader) {
        loader.pixels.resize(readType<uint32_t, true>(data), readType<uint32_t, true>(data));
        auto bitDepth = readType<uint8_t>(data);
        auto colorType = readType<uint8_t>(data);
        readType<uint16_t>(data); // compresion method and filter method
        auto interlance = readType<uint8_t>(data);
        auto crc = readType<uint32_t>(data);
    }

    std::map<std::string, std::function<std::unique_ptr<PNGLoader::ChunkInterface> (void)>> PNGLoader::chunkParsers {
        {"IHDR", FunctionalWrapper<PNGLoader::IHDRChunk, PNGLoader::ChunkInterface>{}}
    };

}

#include "BMPLoader.hpp"

#include "../Exceptions/ImageLoadingFileException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"

namespace ge {

    const std::string BMPLoader::Tag{"bmp"};

    BMPLoader::BMPLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileException{fileName};
        readHeader(file);
        pixels.reserve(3 * width * height);
        readImage(file);
    }

    void BMPLoader::readHeader(std::ifstream& file) {
        if (readType<uint16_t>(file) != 0x4D42)
            throw ImageLoadingInvalidTypeException{fileName};
        uint32_t fileSize = readType<uint32_t>(file);
        uint32_t reserved = readType<uint32_t>(file);
        uint32_t offset = readType<uint32_t>(file);
        uint32_t header = readType<uint32_t>(file);
        width = readType<uint32_t>(file);
        height = readType<uint32_t>(file);
        offset -= 3 * sizeof(uint32_t);
        uint8_t x;
        for (uint32_t i = 0;i < offset; ++i)
            file >> x;
    }

    void BMPLoader::readImage(std::ifstream& file) noexcept {
        for (uint32_t i = 0;i < height; ++i) {
            for (uint32_t j = 0;j < width; ++j)
                pixels.push_back(readType<uint8_t>(file));
            for (uint32_t j = 0; j < (4 - (width % 4)) % 4; ++j)
                readType<uint8_t>(file);
        }
    }
}

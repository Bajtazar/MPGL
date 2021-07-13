#include "BMPLoader.hpp"

#include "../Exceptions/ImageLoadingFileException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"

namespace ge {

    const std::string BMPLoader::Tag{"bmp"};

    BMPLoader::BMPLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{fileName.c_str()};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileException{fileName};
        readHeader(file);
        pixels.reserve(3 * width * height);
        readImage(file);
    }

    void BMPLoader::readHeader(std::ifstream& file) {
        uint16_t fileTag;
        if (file >> fileTag; fileTag != 0x424D)
            throw ImageLoadingInvalidTypeException{fileName};
        uint32_t fileSize = decode<uint32_t>(file);
        uint32_t reserved = decode<uint32_t>(file);
        uint32_t offset = decode<uint32_t>(file);
        uint32_t header = decode<uint32_t>(file);
        width = decode<uint32_t>(file);
        height = decode<uint32_t>(file);
        offset -= 3 * sizeof(uint32_t);
        uint8_t x;
        for (uint32_t i = 0;i < offset; ++i)
            file >> x;
    }

    void BMPLoader::readImage(std::ifstream& file) noexcept {
        char subpixel;
        for (uint32_t i = 0;i < height; ++i) {
            for (uint32_t j = 0;j < width; ++j) {
                file >> subpixel;
                pixels.push_back(subpixel);
            }
            for (uint32_t j = 0; j < (4 - (width % 4)) % 4; ++j)
                file >> subpixel;
        }
    }
}

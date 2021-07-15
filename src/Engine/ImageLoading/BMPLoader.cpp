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
        readType<uint32_t>(file);   // file size
        readType<uint32_t>(file);   // two reserved fields
        uint32_t offset = readType<uint32_t>(file) - 12;
        readType<uint32_t>(file);   // DIB header
        width = readType<uint32_t>(file);
        height = readType<uint32_t>(file);
        for (uint32_t i = 0;i < offset; ++i)
            readType<uint8_t>(file);
    }

    void BMPLoader::readImage(std::ifstream& file) noexcept {
        for (uint32_t i = 0;i < height; ++i) {
            for (uint32_t j = 0;j < 3 * width; ++j)
                pixels.push_back(readType<uint8_t>(file));
            for (uint32_t j = 0; j < (4 - (width % 4)) % 4; ++j)
                readType<uint8_t>(file);
        }
    }
}

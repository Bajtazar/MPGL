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
        readImage(file);
    }

    void BMPLoader::readHeader(std::ifstream& file) {
        if (readType<uint16_t>(file) != 0x4D42)
            throw ImageLoadingInvalidTypeException{fileName};
        readType<uint32_t>(file);   // file size
        readType<uint32_t>(file);   // two reserved fields
        uint32_t offset = readType<uint32_t>(file) - 6 * sizeof(uint32_t) - sizeof(uint16_t);
        readType<uint32_t>(file);   // DIB header
        pixels.resize(readType<uint32_t>(file), readType<uint32_t>(file));
        for (uint32_t i = 0;i < offset; ++i)
            readType<std::byte>(file);
    }

    void BMPLoader::readImage(std::ifstream& file) noexcept {
        for (auto row : pixels) {
            for (auto& pixel : row)
                file >> pixel;
            for (uint32_t j = 0; j < (4 - (pixels.getWidth() % 4)) % 4; ++j)
                readType<std::byte>(file);
        }
    }
}

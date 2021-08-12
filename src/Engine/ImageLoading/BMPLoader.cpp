#include "BMPLoader.hpp"

#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"

namespace ge {

    const std::string BMPLoader::Tag{"bmp"};

    BMPLoader::BMPLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        FileIter iter{StreamBuf{file}, StreamBuf{}};
        try {
            readHeader(iter);
            readImage(iter);
        } catch (std::out_of_range&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        }
    }

    void BMPLoader::readHeader(FileIter& file) {
        if (readType<uint16_t>(file) != 0x4D42)
            throw ImageLoadingInvalidTypeException{fileName};
        readType<uint64_t>(file);   // file size and two reserved fields
        uint32_t offset = readType<uint32_t>(file) - 6 * sizeof(uint32_t) - sizeof(uint16_t);
        readType<uint32_t>(file);   // DIB header
        pixels.resize(readType<uint32_t>(file), readType<uint32_t>(file));
        for (uint32_t i = 0;i < offset; ++i)
            readType<std::byte>(file);
    }

    void BMPLoader::readImage(FileIter& file) {
        for (auto row : pixels) {
            for (auto& pixel : row)
                Image::Manip::RGB(file, pixel);
            ignoreNBytes((4 - pixels.getWidth() % 4) % 4, file);
        }
    }
}

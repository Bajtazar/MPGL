#pragma once

#include <fstream>
#include <vector>

namespace ge {

    template <typename Decoder = std::identity>
    class LoaderInterface {
    public:
        explicit LoaderInterface(const std::string& fileName, Decoder decoder = {});

        const uint8_t* memoryPointer(void) const noexcept { return pixels.data(); }

        std::size_t getWidth(void) const noexcept { return width; }
        std::size_t getHeight(void) const noexcept { return height; }

        virtual ~LoaderInterface(void) noexcept = default;
    protected:
        std::vector<uint8_t> pixels;
        std::size_t width;
        std::size_t height;

        virtual void readHeader(std::ifstream& file, Decoder& decode) = 0;
        virtual void readImage(std::ifstream& file, Decoder& decode) noexcept = 0;
    };

    template <typename Decoder>
    LoaderInterface<Decoder>::LoaderInterface(const std::string& fileName, Decoder decoder) {
        std::ifstream file{fileName.c_str()};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileException{fileName};
        uint32_t offset = readHeader(file, decoder);
        readHeader(file, decoder);
        pixels.reserve(3 * width * height);
        readImage(file, decoder);
    }

}

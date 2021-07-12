#pragma once

#include "LoaderInterface.hpp"
#include "../Exceptions/ImageLoadingFileException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"

#include <functional>

namespace ge {

    template <typename Decoder = std::identity>
    class BMPLoader : public LoaderInterface<Decoder> {
    public:
        explicit BMPLoader(const std::string& fileName, Decoder decoder = {}) : LoaderInterface<Decoder>(std::move(fileName), std::move(decoder)) {}
        ~BMPLoader(void) noexcept = default;
    private:
        template <typename T>
        T decode(std::ifstream& file, Decoder& decoder) const noexcept;

        virtual void readHeader(std::ifstream& file, Decoder& decode) final;
        virtual void readImage(std::ifstream& file, Decoder& decode) noexcept final;
    };

    template <typename Decoder>
    template <typename T>
    T BMPLoader<Decoder>::decode(std::ifstream& file, Decoder& decoder) const noexcept {
        T data;
        file >> data;
        std::invoke(decoder, data);
        return data;
    }

    template <typename Decoder>
    void BMPLoader<Decoder>::readHeader(std::ifstream& file, Decoder& decoder) {
        uint16_t fileTag;
        if (file >> fileTag; std::invoke(decoder, fileTag) != 0x424D)
            throw ImageLoadingInvalidTypeException{fileName};
        uint32_t fileSize = decode<uint32_t, Decoder>(file, decoder);
        uint32_t reserved = decode<uint32_t, Decoder>(file, decoder);
        uint32_t offset = decode<uint32_t, Decoder>(file, decoder);
        uint32_t header = decode<uint32_t, Decoder>(file, decoder);
        width = decode<uint32_t, Decoder>(file, decoder);
        height = decode<uint32_t, Decoder>(file, decoder);
        offset -= 3 * sizeof(uint32_t);
        uint8_t x;
        for (uint32_t i = 0;i < offset; ++i)
            file >> x;
    }

    template <typename Decoder>
    void BMPLoader<Decoder>::readImage(std::ifstream& file, Decoder& decode) noexcept {
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

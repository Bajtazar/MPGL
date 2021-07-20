#pragma once

#include <fstream>

#include "Image.hpp"

namespace ge {

    class LoaderInterface {
    public:
        const Image& getImage(void) const noexcept { return pixels; }

        std::size_t getWidth(void) const noexcept { return pixels.getWidth(); }
        std::size_t getHeight(void) const noexcept { return pixels.getHeight(); }

        virtual ~LoaderInterface(void) noexcept = default;
    protected:
        Image pixels;
        std::string fileName;

        explicit LoaderInterface(const std::string& fileName) : fileName{std::move(fileName)} {}

        virtual void readHeader(std::ifstream& file) = 0;
        virtual void readImage(std::ifstream& file) noexcept = 0;
    };

    template <typename T>
        requires std::is_trivially_constructible_v<T>
    T readType(std::ifstream& file) noexcept {
        T data;
        char* raw = reinterpret_cast<char*>(&data);
        for (uint16_t i = 0;i < sizeof(T); ++i, ++raw)
            file.get(*raw);
        return data;
    }

    template <typename T>
    concept ImageLoaderType = std::derived_from<T, LoaderInterface> && std::is_same_v<const std::string, typename T::Tag>;

}

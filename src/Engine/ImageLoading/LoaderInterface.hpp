#pragma once

#include <fstream>
#include <vector>
#include <type_traits>

namespace ge {

    class LoaderInterface {
    public:
        const uint8_t* memoryPointer(void) const noexcept { return pixels.data(); }

        std::size_t getWidth(void) const noexcept { return width; }
        std::size_t getHeight(void) const noexcept { return height; }

        virtual ~LoaderInterface(void) noexcept = default;
    protected:
        std::vector<uint8_t> pixels;
        std::string fileName;
        std::size_t width;
        std::size_t height;

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

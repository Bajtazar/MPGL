#pragma once

#include <fstream>

#include "../../Collections/Image.hpp"
#include "../Helper.hpp"

namespace ge {

    class LoaderInterface {
    public:
        explicit LoaderInterface(void) noexcept = delete;

        const Image& getImage(void) const noexcept { return pixels; }

        std::size_t getWidth(void) const noexcept { return pixels.getWidth(); }
        std::size_t getHeight(void) const noexcept { return pixels.getHeight(); }

        virtual ~LoaderInterface(void) noexcept = default;
    protected:
        Image pixels;
        std::string fileName;

        explicit LoaderInterface(const std::string& fileName) : fileName{std::move(fileName)} {}
    };

    template <typename T>
    concept ImageLoaderType = std::derived_from<T, LoaderInterface> && std::is_same_v<const std::string, typename T::Tag>;

}

#pragma once

#include <map>
#include <memory>
#include <functional>

#include "LoaderInterface.hpp"
#include "../Utility/FunctionalWrapper.hpp"

namespace ge {

    class ImageLoader {
    public:
        explicit ImageLoader(const std::string& fileName);

        const Image& getImage(void) const noexcept { return opener->getImage(); }
        const uint8_t* memoryPointer(void) const noexcept { return opener->getImage().getMemoryPtr(); }

        std::size_t getWidth(void) const noexcept { return opener->getWidth(); }
        std::size_t getHeight(void) const noexcept { return opener->getHeight(); }

        template <ImageLoaderType T>
        static void addFunctional(void) noexcept { loaders[T::Tag] = std::function<std::unique_ptr<LoaderInterface>(const std::string&)>{ FunctionalWrapper<T, LoaderInterface>{} }; }
    private:
        static std::map<std::string, std::function<std::unique_ptr<LoaderInterface> (const std::string&)>> loaders;
        static std::unique_ptr<LoaderInterface> getLoader(const std::string& fileName);
        static std::string extractTag(const std::string& fileName) noexcept;

        std::unique_ptr<LoaderInterface> opener;
    };

}

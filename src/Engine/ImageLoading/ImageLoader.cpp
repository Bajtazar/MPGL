#include "ImageLoader.hpp"

#include "../Exceptions/ImageLoadingUnsuportedFileType.hpp"
#include "JPEGLoader.hpp"
#include "BMPLoader.hpp"
#include "PNGLoader.hpp"

#include <algorithm>

namespace ge {

    ImageLoader::ImageLoader(const std::string& fileName) : opener{std::move(getLoader(fileName))} {}

    std::string ImageLoader::extractTag(const std::string& fileName) noexcept {
        std::size_t dot = fileName.find_last_of('.');
        std::string tag;
        std::transform(fileName.begin() + dot + 1, fileName.end(), std::back_inserter(tag), [](const char& x) { return std::tolower(x); });
        return tag;
    }

    std::unique_ptr<LoaderInterface> ImageLoader::getLoader(const std::string& fileName) {
        auto iter = loaders.find(extractTag(fileName));
        if (iter == loaders.end())
            throw ImageLoadingUnsuportedFileType{fileName};
        return std::invoke(iter->second, fileName);
    }

    std::map<std::string, std::function<std::unique_ptr<LoaderInterface> (const std::string&)>>
    ImageLoader::loaders {
        {"bmp", {FunctionalWrapper<BMPLoader, LoaderInterface>{}}},
        {"png", {FunctionalWrapper<PNGLoader, LoaderInterface>{}}},
        {"jpg", {FunctionalWrapper<JPEGLoader, LoaderInterface>{}}},
        {"jpe", {FunctionalWrapper<JPEGLoader, LoaderInterface>{}}},
        {"jpeg", {FunctionalWrapper<JPEGLoader, LoaderInterface>{}}}
    };

}

#include "ImageLoader.hpp"

#include "../../Exceptions/ImageLoadingUnsuportedFileType.hpp"
#include "JPEGLoader.hpp"
#include "BMPLoader.hpp"
#include "PNGLoader.hpp"

#include <algorithm>

namespace ge {

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::ImageLoader(Policy policy, const std::string& fileName)
        : opener{std::move(getLoader(policy, fileName))} {}

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::ImageLoader(const std::string& fileName)
        : ImageLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    std::string ImageLoader<Policy>::extractTag(const std::string& fileName) noexcept {
        std::size_t dot = fileName.find_last_of('.');
        std::string tag;
        std::transform(fileName.begin() + dot + 1, fileName.end(), std::back_inserter(tag), [](const char& x) { return std::tolower(x); });
        return tag;
    }

    template <security::SecurityPolicy Policy>
    std::unique_ptr<LoaderInterface> ImageLoader<Policy>::getLoader(Policy policy, const std::string& fileName) {
        auto iter = loaders.find(extractTag(fileName));
        if (iter == loaders.end())
            throw ImageLoadingUnsuportedFileType{fileName};
        return std::invoke(iter->second, policy, fileName);
    }

    template <security::SecurityPolicy Policy>
    std::map<std::string, std::function<std::unique_ptr<LoaderInterface> (Policy, const std::string&)>>
    ImageLoader<Policy>::loaders {
        {"bmp", {FunctionalWrapper<BMPLoader<Policy>, LoaderInterface>{}}},
        {"png", {FunctionalWrapper<PNGLoader<Policy>, LoaderInterface>{}}},
        {"jpg", {FunctionalWrapper<JPEGLoader<Policy>, LoaderInterface>{}}},
        {"jpe", {FunctionalWrapper<JPEGLoader<Policy>, LoaderInterface>{}}},
        {"jpeg", {FunctionalWrapper<JPEGLoader<Policy>, LoaderInterface>{}}}
    };

}

/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include <MPGL/Exceptions/ImageLoadingUnsuportedFileType.hpp>

#include <MPGL/IO/ImageLoading/ImageLoader.hpp>
#include <MPGL/IO/ImageLoading/JPEGLoader.hpp>
#include <MPGL/IO/ImageLoading/BMPLoader.hpp>
#include <MPGL/IO/ImageLoading/PNGLoader.hpp>

#include <algorithm>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::ImageLoader(
        Policy policy,
        Path const& filePath)
            : opener{getLoader(policy, filePath)} {}

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::ImageLoader(Path const& filePath)
        : ImageLoader{Policy{}, filePath} {}

    template <security::SecurityPolicy Policy>
    std::string ImageLoader<Policy>::extractTag(
        Path const& filePath) noexcept
    {
        size_type dot = filePath.find_last_of('.');
        Path tag;
        std::transform(filePath.begin() + dot + 1, filePath.end(),
            std::back_inserter(tag), [](const char& x)
                { return std::tolower(x); });
        return tag;
    }

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::LoaderPtr ImageLoader<Policy>::getLoader(
        Policy policy,
        Path const& filePath)
    {
        auto iter = loaders.find(extractTag(filePath));
        if (iter == loaders.end())
            throw ImageLoadingUnsuportedFileType{filePath};
        return std::invoke(iter->second, policy, filePath);
    }

    template <security::SecurityPolicy Policy>
    ImageLoader<Policy>::Loaders ImageLoader<Policy>::loaders {
        {"bmp", {DeferredConstructor<BMPLoader<Policy>, LoaderInterface>{}}},
        {"png", {DeferredConstructor<PNGLoader<Policy>, LoaderInterface>{}}},
        {"jpg", {DeferredConstructor<JPEGLoader<Policy>, LoaderInterface>{}}},
        {"jpe", {DeferredConstructor<JPEGLoader<Policy>, LoaderInterface>{}}},
        {"jpeg", {DeferredConstructor<JPEGLoader<Policy>, LoaderInterface>{}}}
    };

}

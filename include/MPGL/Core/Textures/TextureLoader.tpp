/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#pragma once

namespace mpgl {

    template <security::SecurityPolicy SP>
    TextureLoaderParallel<SP>::TextureLoaderParallel(
        std::string const& directory,
        async::Threadpool& threadpool)
            : TextureLoaderBase{directory.size() + 1},
                threadpool{threadpool}
    {
        auto files = FileIO::getRecursiveDirFiles(directory);
        allTextures = files.size();
        pushTasks(files);
    }

    template <security::SecurityPolicy SP>
    void TextureLoaderParallel<SP>::pushTasks(Paths const& paths) {
        for (auto const& path : paths)
            imageQueue.emplace_back(path,
                threadpool.appendTask([path, this]() -> Image {
                    return ImageLoader{securityToken, path}.getImage();
                }));
    }

    template <security::SecurityPolicy SP>
    void TextureLoaderParallel<SP>::load(void) {
        auto pred = [&](auto& pair) -> bool {
            using std::literals::operator""ns;
            auto& [path, future] = pair;
            if (std::future_status::ready == future.wait_for(0ns)) {
                getFuture(future, path, exceptions);
                return true;
            }
            return false;
        };
        auto&& [begin, end] = std::ranges::remove_if(imageQueue, pred);
        imageQueue.erase(begin, end);
    }

    template <security::SecurityPolicy SP>
    void TextureLoaderParallel<SP>::getFuture(
        ImageFuture& future,
        std::string const& path,
        Exceptions& exceptions)
    {
        try {
            textures.emplace_back(path.substr(prefix),
                Texture{future.get()});
        } catch (...) {
            exceptions.push_back(std::current_exception());
        }
        ++counter;
    }

    template <security::SecurityPolicy SP>
    TextureLoaderSequenced<SP>::TextureLoaderSequenced(
        std::string const& directory)
            : TextureLoaderBase{directory.size() + 1},
                pathsQueue{FileIO::getRecursiveDirFiles(directory)}
    {
        allTextures = pathsQueue.size();
    }

    template <security::SecurityPolicy SP>
    void TextureLoaderSequenced<SP>::load(void) {
        try {
            loadTexture();
        } catch (...) {
            exceptions.push_back(std::current_exception());
        }
    }

    template <security::SecurityPolicy SP>
    void TextureLoaderSequenced<SP>::loadTexture(void) {
        if (pathsQueue.empty()) return;
        ++counter;
        auto path = std::move(pathsQueue.back());
        pathsQueue.pop_back();
        textures.emplace_back(path.substr(prefix),
            Texture{ImageLoader{securityToken, path}.getImage()});
    }

    template <security::SecurityPolicy SP>
    TextureLoaderParallelInd<SP>::TextureLoaderParallelInd(
        std::string const& directory)
            : TextureLoaderThreadpool{},
              TextureLoaderParallel<SP>{directory, threadpool} {}

    template <security::SecurityPolicy SP,
        execution::ExecutionPolicy EP, bool Independent>
    TextureLoader<SP, EP, Independent>::TextureLoader(
        std::string const& directory,
        [[maybe_unused]] SP securityToken,
        [[maybe_unused]] EP executionToken)
            : LoaderBase{directory} {}

    template <security::SecurityPolicy SP,
        execution::ExecutionPolicy EP, bool Independent>
    TextureLoader<SP, EP, Independent>::TextureLoader(
        std::string const& directory,
        [[maybe_unused]] SP securityToken,
        async::Threadpool& threadpool
        ) requires (!Independent)
            : LoaderBase{directory, threadpool} {}

}

#pragma once

#include "Texture.hpp"
#include "TexturePack.hpp"
#include "../Utility/FileIO.hpp"
#include "../Collections/ThreadsafeQueue.hpp"
#include "../Exceptions/ImageLoadingException.hpp"

#include <stdexcept>
#include <thread>
#include <vector>
#include <ranges>
#include <atomic>

namespace ge {

    template <Allocator Alloc = std::allocator<uint32_t>>
    class TextureLoader {
    public:
        explicit TextureLoader(std::string const& directory);

        TextureLoader(TextureLoader const&) = delete;
        TextureLoader(TextureLoader&&) = delete;

        TextureLoader& operator=(TextureLoader const&) = delete;
        TextureLoader& operator=(TextureLoader&&) = delete;

        double loadingStatus(void) const noexcept;
        void load(void);
        void tryLoad(void);
        void loadAll(void);

        auto getTextures(void) noexcept { return TexturePack<>{std::move(textures)}; }
        std::vector<std::string>& getInvalidPaths(void) const noexcept;

        ~TextureLoader(void) = default;
    private:
        typedef std::vector<std::pair<std::string, Texture<Alloc>>> TextureVector;
        typedef ThreadsafeQueue<std::pair<std::string, Image>>      LoaderQueue;
        typedef ThreadsafeQueue<std::string>                        ImageQueue;

        ImageQueue imagePaths;
        LoaderQueue loadedImages;
        TextureVector textures;

        struct {
            std::vector<std::string> paths;
            mutable std::mutex mutex;
        } invalidPaths;

        std::vector<std::jthread> threadpool;
        std::atomic<std::size_t> counter;
        std::exception_ptr exception;

        std::size_t const allTextures;
        std::size_t const prefix;

        class ParallelLoader {
        public:
            explicit ParallelLoader(TextureLoader<Alloc>& loader) noexcept : loader{loader} {}
            void operator() (void) noexcept;
        private:
            TextureLoader<Alloc>& loader;
        };

        void startParallelLoading(void);
    };

    template <Allocator Alloc>
    TextureLoader<Alloc>::TextureLoader(std::string const& directory)
        : counter{0}, allTextures{0}, prefix{directory.size() + 1}
    {
        auto files = FileIO::getRecursiveDirFiles(directory);
        const_cast<std::size_t&>(allTextures) = files.size();
        imagePaths = ThreadsafeQueue<std::string>{files};
        startParallelLoading();
    }

    template <Allocator Alloc>
    double TextureLoader<Alloc>::loadingStatus(void) const noexcept {
        return (double) textures.size() * counter.load(std::memory_order_relaxed)
            / (allTextures * allTextures);
    }

    template <Allocator Alloc>
    void TextureLoader<Alloc>::tryLoad(void) {
        if (loadingStatus() != 1.)
            load();
    }

    template <Allocator Alloc>
    void TextureLoader<Alloc>::loadAll(void) {
        while (loadingStatus() != 1.)
            load();
    }

    template <Allocator Alloc>
    void TextureLoader<Alloc>::load(void) {
        if (exception)
            std::rethrow_exception(exception);
        if (auto ptr = loadedImages.pop()) {
            auto&& [name, image] = *ptr;
            textures.emplace_back(name, Texture<Alloc>{image});
        }
    }

    template <Allocator Alloc>
    void TextureLoader<Alloc>::ParallelLoader::operator() (void) noexcept {
        while (auto path = loader.imagePaths.pop()) {
            try {
                loader.loadedImages.emplace(
                    std::string(*path).substr(loader.prefix), ImageLoader{*path}.getImage());
            } catch(ImageLoadingException const&) {
                std::lock_guard<std::mutex> lock{loader.invalidPaths.mutex};
                loader.invalidPaths.paths.push_back(*path);
            } catch(...) {
                loader.exception = std::current_exception();
                break;
            }
            loader.counter.fetch_add(1, std::memory_order_relaxed);
        }
    }

    template <Allocator Alloc>
    void TextureLoader<Alloc>::startParallelLoading(void) {
        threadpool.emplace_back(ParallelLoader{*this});
    }

    template <Allocator Alloc>
    std::vector<std::string>& TextureLoader<Alloc>::getInvalidPaths(void) const noexcept {
        std::lock_guard<std::mutex> lock{invalidPaths.mutex};
        return invalidPaths.paths;
    }

}

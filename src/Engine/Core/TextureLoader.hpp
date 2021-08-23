#pragma once

#include "Texture.hpp"
#include "TexturePack.hpp"
#include "../Utility/FileIO.hpp"
#include "../Utility/Security.hpp"
#include "../Collections/ThreadsafeQueue.hpp"
#include "../Exceptions/ImageLoadingException.hpp"
#include "../Exceptions/TextureLoaderMovedTexturesException.hpp"

#include <stdexcept>
#include <thread>
#include <vector>
#include <ranges>
#include <atomic>

namespace ge {

    template <execution::ExecutionPolicy ExecutionPolicy = Sequenced,
        security::SecurityPolicy SecurityPolicy = Secured,
        Allocator Alloc = std::allocator<uint32_t>>
    class TextureLoader {
    public:
        explicit TextureLoader(std::string const& directory);
        explicit TextureLoader(ExecutionPolicy executionPolicy,
            std::string const& directory);
        explicit TextureLoader(SecurityPolicy securityPolicy,
            std::string const& directory);
        explicit TextureLoader(ExecutionPolicy executionPolicy,
            SecurityPolicy securityPolicy, std::string const& directory);

        TextureLoader(TextureLoader const&) = delete;
        TextureLoader(TextureLoader&&) = delete;

        TextureLoader& operator=(TextureLoader const&) = delete;
        TextureLoader& operator=(TextureLoader&&) = delete;

        double loadingStatus(void) const noexcept;
        void load(void);
        void tryLoad(void);
        void loadAll(void);

        auto getTextures(void);
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

        bool available;

        [[no_unique_address]] ExecutionPolicy executionPolicy;
        [[no_unique_address]] SecurityPolicy securityPolicy;

        class ParallelLoader {
        public:
            explicit ParallelLoader(TextureLoader& loader) noexcept : loader{loader} {}
            void operator() (void) noexcept;
        private:
            TextureLoader& loader;
        };

        void startParallelLoading(std::size_t threadpoolSize);
        std::size_t threadpoolSize(void);
    };

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::TextureLoader(std::string const& directory)
        : TextureLoader{ExecutionPolicy{}, SecurityPolicy{}, directory} {}

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::TextureLoader(ExecutionPolicy policy,
        std::string const& directory) : TextureLoader{policy, SecurityPolicy{}, directory} {}

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::TextureLoader(SecurityPolicy policy,
        std::string const& directory) : TextureLoader{ExecutionPolicy{}, policy, directory} {}

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::TextureLoader(ExecutionPolicy executionPolicy,
        SecurityPolicy securityPolicy, std::string const& directory)
            : counter{0}, allTextures{0}, prefix{directory.size() + 1}, available{true},
            executionPolicy{executionPolicy}, securityPolicy{securityPolicy}
    {
        auto files = FileIO::getRecursiveDirFiles(directory);
        const_cast<std::size_t&>(allTextures) = files.size();
        imagePaths = ThreadsafeQueue<std::string>{files};
        startParallelLoading(threadpoolSize());
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    std::size_t TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::threadpoolSize(void) {
        if constexpr (execution::isParallelPolicy<ExecutionPolicy>) {
            std::size_t availableThreads = std::thread::hardware_concurrency();
            return availableThreads > allTextures ? allTextures : availableThreads > 0 ? availableThreads : 1;
        } else if constexpr (execution::isSequencedPolicy<ExecutionPolicy>)
            return allTextures ? 1 : 0;
        else
            throw ExecusionUnknownPolicyException{};
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    double TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::loadingStatus(void) const noexcept {
        return (double) textures.size() * counter.load(std::memory_order_relaxed)
            / (allTextures * allTextures);
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    void TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::tryLoad(void) {
        if (loadingStatus() != 1.)
            load();
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    void TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::loadAll(void) {
        while (loadingStatus() != 1.)
            load();
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    void TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::load(void) {
        if (exception)
            std::rethrow_exception(exception);
        if (auto ptr = loadedImages.pop()) {
            auto&& [name, image] = *ptr;
            textures.emplace_back(name, Texture<Alloc>{image});
        }
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    void TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::ParallelLoader::operator() (void) noexcept {
        while (auto path = loader.imagePaths.pop()) {
            try {
                loader.loadedImages.emplace(std::string(*path).substr(loader.prefix),
                    ImageLoader{loader.securityPolicy, *path}.getImage());
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

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    void TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::startParallelLoading(std::size_t threadpoolSize) {
        threadpool.reserve(threadpoolSize);
        for (auto i : std::views::iota(std::size_t(0), threadpoolSize))
            threadpool.emplace_back(ParallelLoader{*this});
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    std::vector<std::string>& TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::getInvalidPaths(void) const noexcept {
        std::lock_guard<std::mutex> lock{invalidPaths.mutex};
        return invalidPaths.paths;
    }

    template <execution::ExecutionPolicy ExecutionPolicy,
        security::SecurityPolicy SecurityPolicy, Allocator Alloc>
    auto TextureLoader<ExecutionPolicy, SecurityPolicy, Alloc>::getTextures(void) {
        if (!available)
            throw TextureLoaderMovedTexturesException{};
        available = false;
        return TexturePack<>{executionPolicy, std::move(textures)};
    }

}

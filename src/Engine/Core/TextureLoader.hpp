#pragma once

#include "Texture.hpp"
#include "TexturePack.hpp"
#include "../IO/FileIO.hpp"
#include "../Utility/Security.hpp"
#include "../Utility/Execution.hpp"
#include "../Collections/ThreadsafeQueue.hpp"
#include "../Exceptions/ImageLoadingException.hpp"

#include <stdexcept>
#include <thread>
#include <vector>
#include <ranges>
#include <atomic>

namespace ge {

    template <security::SecurityPolicy SecurityPolicy = Secured,
        Allocator Alloc = std::allocator<uint32_t>>
    class TextureLoader {
    public:
        explicit TextureLoader(std::string const& directory);
        template <execution::ExecutionPolicy ExecutionPolicy>
        explicit TextureLoader(ExecutionPolicy executionPolicy,
            std::string const& directory);
        explicit TextureLoader(SecurityPolicy securityPolicy,
            std::string const& directory);
        template <execution::ExecutionPolicy ExecutionPolicy>
        explicit TextureLoader(ExecutionPolicy executionPolicy,
            SecurityPolicy securityPolicy,
            std::string const& directory);

        TextureLoader(TextureLoader const&) = delete;
        TextureLoader(TextureLoader&&) = delete;

        TextureLoader& operator=(TextureLoader const&) = delete;
        TextureLoader& operator=(TextureLoader&&) = delete;

        double loadingStatus(void) const noexcept;
        void load(void);
        void tryLoad(void);
        void loadAll(void);

        TexturePack<Alloc> getTextures(void) const
            { return TexturePack<Alloc>{textures}; }
        std::vector<std::string> const& getInvalidPaths(
            void) const noexcept;

        ~TextureLoader(void);
    private:
        typedef std::vector<std::pair<std::string const,
            Texture<Alloc>>>                        TextureVector;
        typedef ThreadsafeQueue<std::pair<
            std::string, Image>>                    LoaderQueue;
        typedef ThreadsafeQueue<std::string>        ImageQueue;
        typedef std::vector<std::jthread>           Threadpool;
        typedef std::size_t                         size_type;
        typedef std::atomic<size_type>              AtomicCounter;

        ImageQueue                                  imagePaths;
        LoaderQueue                                 loadedImages;
        TextureVector                               textures;

        struct {
            std::vector<std::string>                paths;
            mutable std::mutex                      mutex;
        }                                           invalidPaths;

        Threadpool                                  threadpool;
        AtomicCounter                               counter;
        std::exception_ptr                          exception;

        size_type const                             allTextures;
        size_type const                             prefix;

        [[no_unique_address]] SecurityPolicy        securityPolicy;

        class ParallelLoader {
        public:
            explicit ParallelLoader(TextureLoader& loader)
                noexcept : loader{loader} {}
            void operator() (void) noexcept;
        private:
            TextureLoader&                          loader;

            void loadImage(std::string const& path);
            void saveInvalidPath(std::string const& path);
        };

        void startParallelLoading(size_type threadpoolSize);

        template <execution::ExecutionPolicy ExecutionPolicy>
        size_type threadpoolSize(void);
    };

    template <security::SecurityPolicy Sp, Allocator Alloc>
    TextureLoader<Sp, Alloc>::TextureLoader(
        std::string const& directory)
            : TextureLoader{Sequenced{}, Sp{}, directory} {}

    template <security::SecurityPolicy Sp, Allocator Alloc>
    template <execution::ExecutionPolicy Ep>
    TextureLoader<Sp, Alloc>::TextureLoader(
        Ep policy, std::string const& directory)
            : TextureLoader{policy, Sp{}, directory} {}

    template <security::SecurityPolicy Sp, Allocator Alloc>
    TextureLoader<Sp, Alloc>::TextureLoader(Sp policy,
        std::string const& directory)
            : TextureLoader{Sequenced{}, policy, directory} {}

    template <security::SecurityPolicy Sp, Allocator Alloc>
    template <execution::ExecutionPolicy Ep>
    TextureLoader<Sp, Alloc>::TextureLoader(
        Ep executionPolicy, Sp securityPolicy,
        std::string const& directory)
            : counter{0}, allTextures{0}, prefix{directory.size() + 1},
            securityPolicy{securityPolicy}
    {
        auto files = FileIO::getRecursiveDirFiles(directory);
        const_cast<size_type&>(allTextures) = files.size();
        imagePaths = ImageQueue{files};
        startParallelLoading(threadpoolSize<Ep>());
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    template <execution::ExecutionPolicy Ep>
    TextureLoader<Sp, Alloc>::size_type
        TextureLoader<Sp, Alloc>::threadpoolSize(void)
    {
        if constexpr (execution::isParallelPolicy<Ep>) {
            size_type availableThreads
                = std::thread::hardware_concurrency();
            return availableThreads > allTextures ? allTextures
                : availableThreads > 0 ? availableThreads : 1;
        } else if constexpr (execution::isSequencedPolicy<Ep>)
            return allTextures ? 1 : 0;
        else
            throw ExecusionUnknownPolicyException{};
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    double TextureLoader<Sp, Alloc>::loadingStatus(
        void) const noexcept
    {
        return (double) textures.size() * counter.load(
            std::memory_order_relaxed) / (allTextures * allTextures);
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::tryLoad(void) {
        if (loadingStatus() != 1.)
            load();
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::loadAll(void) {
        while (loadingStatus() != 1.)
            load();
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::load(void) {
        if (exception)
            std::rethrow_exception(exception);
        if (auto ptr = loadedImages.pop()) {
            auto&& [name, image] = *ptr;
            textures.emplace_back(name, Texture<Alloc>{image});
        }
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::ParallelLoader::loadImage(
        std::string const& path)
    {
        loader.loadedImages.emplace(path.substr(loader.prefix),
            ImageLoader{loader.securityPolicy, path}.getImage());
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::ParallelLoader::saveInvalidPath(
        std::string const& path)
    {
        std::lock_guard<std::mutex> lock{loader.invalidPaths.mutex};
        loader.invalidPaths.paths.push_back(path);
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::ParallelLoader::operator()(
        void) noexcept
    {
        while (auto path = loader.imagePaths.pop()) {
            try {
                loadImage(*path);
            } catch(ImageLoadingException const&) {
                saveInvalidPath(*path);
            } catch(...) {
                loader.exception = std::current_exception();
                loader.imagePaths = ImageQueue{}; // stops other threads
                return;
            }
            loader.counter.fetch_add(1u, std::memory_order_relaxed);
        }
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    void TextureLoader<Sp, Alloc>::startParallelLoading(
        size_type threadpoolSize)
    {
        threadpool.reserve(threadpoolSize);
        for (size_type i = 0; i < threadpoolSize; ++i)
            threadpool.emplace_back(ParallelLoader{*this});
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    std::vector<std::string> const&
        TextureLoader<Sp, Alloc>::getInvalidPaths(void)
            const noexcept
    {
        std::lock_guard<std::mutex> lock{invalidPaths.mutex};
        return invalidPaths.paths;
    }

    template <security::SecurityPolicy Sp, Allocator Alloc>
    TextureLoader<Sp, Alloc>::~TextureLoader(void) {
        imagePaths = ImageQueue{};
    }

}

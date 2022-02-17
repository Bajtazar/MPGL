#pragma once

#include "TexturePack.hpp"
#include "../IO/FileIO.hpp"
#include "../Utility/Security.hpp"
#include "../Utility/Execution.hpp"
#include "../Concurrency/Threadpool.hpp"
#include "../Exceptions/ImageLoadingException.hpp"

#include <algorithm>
#include <atomic>
#include <vector>
#include <list>

namespace ge {

    class TextureLoaderBase {
    public:
        explicit TextureLoaderBase(std::size_t prefix) noexcept
            : allTextures{0}, counter{0}, prefix{prefix} {}

        typedef std::vector<std::string>            Paths;

        [[nodiscard]] float64 loadingStatus(void) const
            { return counter / allTextures; }

        virtual void load(void) = 0;

        void tryLoad(void);
        void loadAll(void);

        // throw an exception
        TexturePack getTextures(void) const
            { return TexturePack{textures}; }

        virtual ~TextureLoaderBase(void) noexcept = default;
    protected:
        typedef std::vector<std::pair<std::string const,
            Texture>>                               TextureVector;
        typedef std::list<std::exception_ptr>       Exceptions;

        TextureVector                               textures;
        Exceptions                                  exceptions;
        std::size_t                                 allTextures;
        std::size_t                                 counter;
        std::size_t const                           prefix;
    };

    template <security::SecurityPolicy SecurityPolicy = Secured>
    class TextureLoaderParallel : public TextureLoaderBase {
    public:
        explicit TextureLoaderParallel(
            std::string const& directory,
            Threadpool& threadpool);

        TextureLoaderParallel(TextureLoaderParallel const&) = delete;
        TextureLoaderParallel(TextureLoaderParallel&&) = delete;

        TextureLoaderParallel& operator=(
            TextureLoaderParallel const&) = delete;
        TextureLoaderParallel& operator=(
            TextureLoaderParallel&&) = delete;

        void load(void) final;

        ~TextureLoaderParallel(void) noexcept = default;
    private:
        typedef std::future<Image>                  ImageFuture;
        typedef std::list<std::pair<
            std::string, ImageFuture>>              ImageQueue;

        ImageQueue                                  imageQueue;
        Threadpool&                                 threadpool;
        [[no_unique_address]] SecurityPolicy        securityToken;

        void pushTasks(Paths const& paths);
        void getFuture(ImageFuture& future,
            std::string const& path,
            Exceptions& exceptions);
    };

    template <security::SecurityPolicy SecurityPolicy = Secured,
        execution::ExecutionPolicy ExecutionPolicy = Parallel>
    class TextureLoader {
    public:
        typedef std::vector<std::string>            Paths;

        explicit TextureLoader(
            std::string const& directory,
            SecurityPolicy securityToken = {},
            ExecutionPolicy executionToken = {});

        explicit TextureLoader(
            std::string const& directory,
            SecurityPolicy securityToken = {},
            Threadpool& threadpool = {});

        TextureLoader(TextureLoader const&) = delete;
        TextureLoader(TextureLoader&&) = delete;

        TextureLoader& operator=(TextureLoader const&) = delete;
        TextureLoader& operator=(TextureLoader&&) = delete;

        ~TextureLoader(void) = default;
    };

    template <security::SecurityPolicy SP>
    TextureLoaderParallel<SP>::TextureLoaderParallel(
        std::string const& directory,
        Threadpool& threadpool)
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
                threadpool.append([path, this]() -> Image {
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


}

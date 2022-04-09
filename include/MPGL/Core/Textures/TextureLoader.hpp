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
#pragma once

#include <MPGL/Core/Textures/TexturePack.hpp>
#include <MPGL/Concurrency/Threadpool.hpp>
#include <MPGL/Utility/Execution.hpp>
#include <MPGL/Utility/Security.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <algorithm>
#include <vector>
#include <list>

namespace mpgl {

    /**
     * The base class of all texture loaders
     */
    class TextureLoaderBase {
    public:
        /**
         * Construct a new Texture Loader Base object
         *
         * @param prefix the length of avoided path prefix
         */
        explicit TextureLoaderBase(std::size_t prefix) noexcept
            : allTextures{0}, counter{0}, prefix{prefix} {}

        typedef std::vector<std::string>            Paths;

        /**
         * Returns the percentage of loaded textures in range
         * from 0.f to 1.f
         *
         * @return float64 the percentage of loaded textures
         */
        [[nodiscard]] float64 loadingStatus(void) const
            { return counter / allTextures; }

        /**
         * Pure virtual load function. It has to be implemented
         * by all deriving classes. It should implement a texture
         * loading mechanism
         */
        virtual void load(void) = 0;

        /**
         * Tryies to call loading function, whether there are
         * unloaded textures left
         */
        void tryLoad(void);

        /**
         * Loads all unloaded yet textures
         */
        void loadAll(void);

        /**
         * Returns the TexturePack object with all loaded textures.
         * Textures are associated with their path shortened by
         * the direction path
         *
         * @throw StackedExceptions exception object containing
         * pointers (std::exception_ptr) to all exceptions
         * throwed by texture loading process during loading phase.
         *
         * @return TexturePack object containing all textures
         * loaded so far
         */
        [[nodiscard]] TexturePack getTextures(void) const;

        /**
         * Virtual destructor of TextureLoaderBase
         */
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

    /**
     * Texture loading class which loads textures in a sequenced
     * manner (only on the given thread) in given security policy.
     *
     * @tparam SecurityPolicy security policy passed to image
     * loading classes
     */
    template <security::SecurityPolicy SecurityPolicy = Secured>
    class TextureLoaderSequenced : public TextureLoaderBase {
    public:
        /**
         * Construct a new Texture Loader Sequenced object.
         * Loads all files from the given directory path and uses
         * them to make textures
         *
         * @param directory the directory containing image files
         */
        explicit TextureLoaderSequenced(
            std::string const& directory);

        TextureLoaderSequenced(TextureLoaderSequenced const&) = delete;
        TextureLoaderSequenced(TextureLoaderSequenced&&) = delete;

        TextureLoaderSequenced& operator=(
            TextureLoaderSequenced const&) = delete;
        TextureLoaderSequenced& operator=(
            TextureLoaderSequenced&&) = delete;

        /**
         * Loads texture from one file from given directory
         */
        void load(void) final;

        /**
         * Virtual destructor of TextureLoaderSequenced
         */
        virtual ~TextureLoaderSequenced(void) noexcept = default;
    private:
        Paths                                       pathsQueue;
        [[no_unique_address]] SecurityPolicy        securityToken;

        void loadTexture(void);
    };

    /**
     * Texture loading class which loads textures in a parallel
     * manner (utilizes given threadpool) in given security policy.
     *
     * @tparam SecurityPolicy security policy passed to image
     * loading classes
     */
    template <security::SecurityPolicy SecurityPolicy = Secured>
    class TextureLoaderParallel : public TextureLoaderBase {
    public:
        /**
         * Construct a new Texture Loader Parallel object.
         * Uses given threadpool reference to load images
         * from given directory and makes textures from them.
         *
         * @param directory the directory containing image files
         * @param threadpool the threadpool used to load images
         */
        explicit TextureLoaderParallel(
            std::string const& directory,
            Threadpool& threadpool);

        TextureLoaderParallel(TextureLoaderParallel const&) = delete;
        TextureLoaderParallel(TextureLoaderParallel&&) = delete;

        TextureLoaderParallel& operator=(
            TextureLoaderParallel const&) = delete;
        TextureLoaderParallel& operator=(
            TextureLoaderParallel&&) = delete;

        /**
         * Loads all textures that have loaded images
         */
        void load(void) final;

        /**
         * Virtual destructor of TextureLoaderParallel
         */
        virtual ~TextureLoaderParallel(void) noexcept = default;
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

    /**
     * Class used by the indepentent parallel texture loader
     * to implement implicit threadpool (when threadpool
     * reference is not given).
     */
    class TextureLoaderThreadpool {
    protected:
        Threadpool                                  threadpool;
    };

    /**
     * Texture loading class which implements the parallel loader
     * with implicit threadpool.
     *
     * @tparam SP the security policy used in image loading process
     */
    template <security::SecurityPolicy SP = Secured>
    class TextureLoaderParallelInd :
        private TextureLoaderThreadpool,
        public TextureLoaderParallel<SP>
    {
    public:
        /**
         * Construct a new Texture Loader Parallel Ind object.
         * Uses own threadpool to load images from given directory
         * and makes textures from them.
         *
         * @param directory the directory containing image files
         */
        explicit TextureLoaderParallelInd(
            std::string const& directory);

        TextureLoaderParallelInd(
            TextureLoaderParallelInd const&) = delete;
        TextureLoaderParallelInd(
            TextureLoaderParallelInd&&) = delete;

        TextureLoaderParallelInd& operator=(
            TextureLoaderParallelInd const&) = delete;
        TextureLoaderParallelInd& operator=(
            TextureLoaderParallelInd&&) = delete;

        /**
         * Virtual destructor of TextureLoaderParallelInd
         */
        virtual ~TextureLoaderParallelInd(void) noexcept = default;
    };

    /**
     * The conditional base that chooses the right texure loader
     * for the given conditions
     *
     * @tparam SP the security policy of the texture loader
     * @tparam EP the execution policy of the texure loader
     * @tparam Independent the information whether implicit
     * threadpool is being used
     */
    template <security::SecurityPolicy SP,
        execution::ExecutionPolicy EP, bool Independent>
    using TextureLoaderBaseImpl =
        std::conditional_t<execution::isSequencedPolicy<EP>,
            TextureLoaderSequenced<SP>,
            std::conditional_t<Independent,
                TextureLoaderParallelInd<SP>,
                TextureLoaderParallel<SP>
            >
        >;

    /**
     * Texture loading class which chooses a suitable loader for
     * given conditions and utilizes it to load textures from a
     * given directory
     *
     * @tparam SecurityPolicy the security policy used by image
     * loading process
     * @tparam ExecutionPolicy the execution policy used to
     * determine in which manner the loading process should be
     * executed
     * @tparam Independent the information whether implicit
     * threadpool should be used (only for parallel execution
     * policy)
     */
    template <security::SecurityPolicy SecurityPolicy = Secured,
        execution::ExecutionPolicy ExecutionPolicy = Parallel,
        bool Independent = true>
    class TextureLoader
        : public TextureLoaderBaseImpl<SecurityPolicy,
            ExecutionPolicy, Independent>
    {
    private:
        typedef TextureLoaderBaseImpl<
            SecurityPolicy, ExecutionPolicy,
            Independent>                            LoaderBase;
    public:
        typedef std::vector<std::string>            Paths;

        /**
         * Construct a new Texture Loader object. Loads images
         * from given directory and turns them into textures in
         * given execution and security manner
         *
         * @param directory the directory containing image files
         * @param securityToken the security policy token
         * @param executionToken the execution policy token
         */
        explicit TextureLoader(
            std::string const& directory,
            SecurityPolicy securityToken = {},
            ExecutionPolicy executionToken = {});

        /**
         * Construct a new Texture Loader object. Utlizes given
         * threadpool to load images from given directory and
         * turns them into textures in given security manner.
         * Works only for parallel execution manner.
         *
         * @param directory the directory containing image files
         * @param securityToken the security policy token
         * @param threadpool the given threadpool reference
         */
        explicit TextureLoader(
            std::string const& directory,
            SecurityPolicy securityToken,
            Threadpool& threadpool
            ) requires (!Independent);

        TextureLoader(TextureLoader const&) = delete;
        TextureLoader(TextureLoader&&) = delete;

        TextureLoader& operator=(TextureLoader const&) = delete;
        TextureLoader& operator=(TextureLoader&&) = delete;

        /**
         * Destroy the Texture Loader object
         */
        ~TextureLoader(void) = default;
    };

    template <security::SecurityPolicy SecurityPolicy>
    TextureLoader(std::string const&,
        SecurityPolicy, Parallel) -> TextureLoader<SecurityPolicy,
            Parallel, true>;

    template <security::SecurityPolicy SecurityPolicy>
    TextureLoader(std::string const&,
        SecurityPolicy, Sequenced) -> TextureLoader<SecurityPolicy,
            Sequenced, false>;

    template <security::SecurityPolicy SecurityPolicy>
    TextureLoader(std::string const&,
        SecurityPolicy, Threadpool&) -> TextureLoader<SecurityPolicy,
            Parallel, false>;

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
        Threadpool& threadpool
        ) requires (!Independent)
            : LoaderBase{directory, threadpool} {}

}

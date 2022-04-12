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

#include <map>
#include <memory>
#include <functional>

#include <MPGL/IO/ImageLoading/LoaderInterface.hpp>
#include <MPGL/Utility/FunctionalWrapper.hpp>
#include <MPGL/Utility/Security.hpp>

namespace mpgl {

    /**
     * Loads the given image into the memory. If image has
     * unsupported format or it cannot be opened or parsed
     * then throws an exception
     *
     * @tparam Policy the secured policy token
     */
    template <security::SecurityPolicy Policy = Secured>
    class ImageLoader {
    public:
        typedef std::size_t                         size_type;
        typedef std::string                         Path;

        /**
         * Constructs a new Image Loader object from the given
         * image file path
         *
         * @throw ImageLoadingUnsuportedFileType when the given
         * image format is unsuported
         * @param filePath the path to the image file
         */
        explicit ImageLoader(Path const& filePath);

        /**
         * Constructs a new Image Loader object from the given
         * image file path
         *
         * @throw ImageLoadingUnsuportedFileType when the given
         * image format is unsuported
         * @param policy the policy token
         * @param filePath the path to the image file
         */
        explicit ImageLoader(Policy policy, Path const& filePath);

        /**
         * Returns the constant reference to the image
         *
         * @return the constant reference to the image
         */
        [[nodiscard]] Image const& getImage(void) const noexcept
            { return opener->getImage(); }

        /**
         * Returns the pointer to the image's memory
         *
         * @return the pointer to the image's memory
         */
        [[nodiscard]] void const* memoryPointer(void) const noexcept
            { return opener->getImage().data(); }

        /**
         * Returns the width of the image
         *
         * @return the width of the image
         */
        [[nodiscard]] size_type getWidth(void) const noexcept
            { return opener->getWidth(); }

        /**
         * Returns the height of the image
         *
         * @return the height of the image
         */
        [[nodiscard]] size_type getHeight(void) const noexcept
            { return opener->getHeight(); }

        /**
         * Adds the new image format loader into the image loaders map
         *
         * @tparam Tp the type of the new image format loader
         */
        template <std::derived_from<LoaderInterface> Tp>
            requires (std::same_as<typename Tp::Tag, Path const>
                && std::invocable<Tp, Policy, Path const>)
        static void addFormatLoader(void);
    private:
        typedef std::unique_ptr<LoaderInterface>    LoaderPtr;
        typedef std::function<LoaderPtr(Policy,
            Path const&)>                           LoadingFun;
        typedef std::map<std::string, LoadingFun>   Loaders;

        /**
         * Returns the loader for the given image format
         *
         * @throw ImageLoadingUnsuportedFileType when the given
         * image format is unsuported
         * @param policy the policy token
         * @param filePath the path to the image file
         * @return LoaderPtr
         */
        static LoaderPtr getLoader(
            Policy policy,
            Path const& filePath);

        /**
         * Returns the image file's tag from the given file path
         *
         * @param filePath the path to the image file
         * @return the image file's tag
         */
        static Path extractTag(Path const& filePath) noexcept;

        LoaderPtr                                   opener;

        static Loaders                              loaders;
    };

    template class ImageLoader<Secured>;
    template class ImageLoader<Unsecured>;

}

#include <MPGL/IO/ImageLoading/ImageLoader.tpp>

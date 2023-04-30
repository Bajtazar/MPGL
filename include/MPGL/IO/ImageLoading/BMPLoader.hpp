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

#include <MPGL/IO/ImageLoading/LoaderInterface.hpp>
#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Iterators/SafeIterator.hpp>

#include <fstream>

namespace mpgl {

    /**
     * Loads the BMP format image file
     *
     * @tparam Policy the secured policy token
     */
    template <security::SecurityPolicy Policy = Secured>
    class BMPLoader : public LoaderInterface {
    public:
        typedef std::string                         Path;

        /// The BMP tag
        static Path const                           Tag;

        /**
         * Constructs a new BMPLoader object. Loads the
         * BMP format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param filePath the path to the BMP file
         */
        explicit BMPLoader(Path const& filePath);

        /**
         * Constructs a new BMPLoader object. Loads the
         * BMP format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param policy the security policy tag
         * @param filePath the path to the BMP file
         */
        explicit BMPLoader(
            Policy policy,
            Path const& fileName);

        /**
         * Destroys the BMPLoader object
         */
        ~BMPLoader(void) noexcept = default;
    private:
        typedef std::istreambuf_iterator<char>      StreamBuf;
        typedef PolicyIterIT<Policy, StreamBuf>     FileIter;

        /**
         * Parses the BMP's header
         *
         * @throw ImageLoadingInvalidTypeException when the given
         * file is in invalid format
         * @param file the reference to the wrapped file's iterator
         */
        void readHeader(FileIter& file);

        /**
         * Parses the BMP's image
         *
         * @param file the reference to the wrapped file's iterator
         */
        void readImage(FileIter& file);
    };

    template class BMPLoader<Secured>;
    template class BMPLoader<Unsecured>;

}

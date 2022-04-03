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

#include "LoaderInterface.hpp"
#include "../../Utility/Security.hpp"
#include "../../Iterators/SafeIterator.hpp"
#include "../../Utility/FunctionalWrapper.hpp"

#include <map>
#include <array>
#include <fstream>
#include <iterator>
#include <functional>

namespace mpgl {

    /**
     * Loads the PNG format image file
     *
     * @tparam Policy the secured policy token
     */
    template <security::SecurityPolicy Policy = Secured>
    class PNGLoader : public LoaderInterface {
    public:
        typedef std::size_t                         size_type;
        typedef std::string                         Path;

        /// The PNG tag
        static Path const                           Tag;

        /**
         * Constructs a new PNGLoader object. Loads the
         * PNG format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param filePath the path to the PNG file
         */
        explicit PNGLoader(Path const& filePath);

        /**
         * Constructs a new PNGLoader object. Loads the
         * PNG format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param policy the security policy tag
         * @param filePath the path to the PNG file
         */
        explicit PNGLoader(
            Policy policy,
            Path const& filePath);

        /**
         * Destroys the PNGLoader object
         */
        ~PNGLoader(void) noexcept = default;
    private:
        typedef std::vector<char>                   DataBuffer;
        typedef DataBuffer::const_iterator          CharIter;

        typedef std::conditional_t<
        security::isSecurePolicy<Policy>,
            SafeIterator<CharIter>, CharIter>       FileIter;

        /**
         * Filters the given image
         */
        class Filters {
        public:
            /**
             * Constructs a new Filters object from the given
             * image reference
             *
             * @param image the reference to the image object
             * @param loader the reference to the loader object
             */
            explicit Filters(
                Image& image,
                PNGLoader& loader) noexcept
                    : image{image}, loader{loader} {}

            /**
             * Filter pixels from the given buffer
             *
             * @param iter the iterator to the data buffer
             */
            void operator()(FileIter& iter) noexcept;

            /**
             * Sets the RGBA pixels
             *
             * @param row the row size
             * @param column the column size
             * @param filter the filter type
             * @param iter the reference to the iterator
             */
            void setRGBAPixels(
                size_type row,
                size_type column,
                uint8 filter,
                FileIter& iter) noexcept;

            /**
             * Sets the RGB pixels
             *
             * @param row the row size
             * @param column the column size
             * @param filter the filter type
             * @param iter the reference to the iterator
             */
            void setRGBPixels(
                size_type row,
                size_type column,
                uint8 filter,
                FileIter& iter) noexcept;

            /**
             * Sets the gray pixels
             *
             * @param row the row size
             * @param column the column size
             * @param filter the filter type
             * @param iter the reference to the iterator
             */
            void setGrayPixels(
                size_type row,
                size_type column,
                uint8 filter,
                FileIter& iter) noexcept;

            /**
             * Sets the gray-alpha pixels
             *
             * @param row the row size
             * @param column the column size
             * @param filter the filter type
             * @param iter the reference to the iterator
             */
            void setGrayAlphaPixels(
                size_type row,
                size_type column,
                uint8 filter,
                FileIter& iter) noexcept;

            /**
             * Destroys the Filters object
             */
            ~Filters(void) noexcept = default;
        private:

            /**
             * The paeth predictor
             *
             * @param a the a pixel
             * @param b the b pixel
             * @param c the c pixel
             * @return the predicted pixel
             */
            uint8 paethPredictor(
                uint8 a,
                uint8 b,
                uint8 c) const noexcept;

            /**
             * The type-A reconstructor
             *
             * @param row the row size
             * @param column the column size
             * @param pixel the subpixel value
             * @return the reconstructed pixel
             */
            uint8 reconstructA(
                size_type row,
                size_type column,
                uint8 pixel) const noexcept;

            /**
             * The type-B reconstructor
             *
             * @param row the row size
             * @param column the column size
             * @param pixel the subpixel value
             * @return the reconstructed pixel
             */
            uint8 reconstructB(
                size_type row,
                size_type column,
                uint8 pixel) const noexcept;

            /**
             * The type-C reconstructor
             *
             * @param row the row size
             * @param column the column size
             * @param pixel the subpixel value
             * @return the reconstructed pixel
             */
            uint8 reconstructC(
                size_type row,
                size_type column,
                uint8 pixel) const noexcept;

            /**
             * Filter the subpixel
             *
             * @param row the row size
             * @param column the column size
             * @param filter the filter type
             * @param subpixelID the subpixel id
             * @param iter the reference to the iterator
             * @return the filtered subpixel
             */
            uint8 filterSubpixel(
                size_type row,
                size_type column,
                uint8 filter,
                uint8 subpixelID,
                FileIter& iter) noexcept;

            /**
             * The first subpixel filter
             *
             * @param row the row size
             * @param column the column size
             * @param subpixelID the subpixel id
             * @param subpixel the subpixel value
             * @return the filtered subpixel
             */
            uint8 subpixelFilterA(
                size_type row,
                size_type column,
                uint8 subpixelID,
                uint8 subpixel) const noexcept;

            /**
             * The second subpixel filter
             *
             * @param row the row size
             * @param column the column size
             * @param subpixelID the subpixel id
             * @param subpixel the subpixel value
             * @return the filtered subpixel
             */
            uint8 subpixelFilterB(
                size_type row,
                size_type column,
                uint8 subpixelID,
                uint8 subpixel) const noexcept;

            /**
             * The third subpixel filter
             *
             * @param row the row size
             * @param column the column size
             * @param subpixelID the subpixel id
             * @param subpixel the subpixel value
             * @return the filtered subpixel
             */
            uint8 subpixelFilterC(
                size_type row,
                size_type column,
                uint8 subpixelID,
                uint8 subpixel) const noexcept;

            /**
             * The fourth subpixel filter
             *
             * @param row the row size
             * @param column the column size
             * @param subpixelID the subpixel id
             * @param subpixel the subpixel value
             * @return the filtered subpixel
             */
            uint8 subpixelFilterD(
                size_type row,
                size_type column,
                uint8 subpixelID,
                uint8 subpixel) const noexcept;

            Image&                                  image;
            PNGLoader&                              loader;
        };

        typedef void(PNGLoader::Filters::*PixelsSetter)
            (std::size_t, std::size_t, uint8, FileIter&);

        typedef std::map<uint8, PixelsSetter>       ColorSetters;

        /**
         * Interface for all types of the PNG data chunks
         */
        class ChunkInterface {
        public:
            /**
             * Constructs a new Chunk Interface object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit ChunkInterface(PNGLoader& loader) noexcept
                : loader{loader} {}

            /**
             * Pure virtual operator. Has to be overloaded.
             * Parses the chunk data
             *
             * @param length the length of the chunk
             * @param data the reference to the file's iterator
             */
            virtual void operator() (
                size_type length,
                FileIter& data) = 0;

            /**
             * Virtual destructor. Destroys the Chunk Interface object
             */
            virtual ~ChunkInterface(void) = default;
        protected:
            PNGLoader&                              loader;
        };

        /**
         * Parses the PNG's IHDR chunk
         */
        class IHDRChunk : public ChunkInterface {
        public:
            /**
             * Constructs a new IHDRChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit IHDRChunk(PNGLoader& loader) noexcept
                : ChunkInterface{ loader } {}

            /**
             * Parses the IDHR chunk data
             *
             * @param length the length of the chunk
             * @param data the reference to the file's iterator
             */
            virtual void operator() (
                size_type length,
                FileIter& data) final;

            /**
             *  Destroys the IHDRChunk object
             */
            ~IHDRChunk(void) = default;
        private:
            /**
             * Parses the bit depths
             *
             * @param depth the bit depth
             */
            void parseBitDepth(uint8 depth);

            /**
             * Parses the color type
             *
             * @param colorType the color type
             */
            void parseColorType(uint8 colorType);

            /**
             * Parses the interlance
             *
             * @param interlance the interlance
             */
            void parseInterlance(uint8 interlance);

            static ColorSetters const               colorSetters;
        };

        /**
         * Parses the PNG's IDAT chunk
         */
        class IDATChunk : public ChunkInterface {
        public:
            /**
             * Constructs a new IDATChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit IDATChunk(PNGLoader& loader) noexcept
                : ChunkInterface{ loader } {}

            /**
             * Parses the IDAT chunk data
             *
             * @param length the length of the chunk
             * @param data the reference to the file's iterator
             */
            virtual void operator() (
                size_type length,
                FileIter& data) final;

            /**
             * Destroys the IDATChunk object
             */
            ~IDATChunk(void) = default;
        };

        /**
         * Sets the secure policy
         *
         * @throw SecurityUnknownPolicyException when the security
         * policy token is unknown
         * @param file the reference to the string object
         * @param policy the secure policy token
         */
        void setPolicy(DataBuffer const& file, Policy policy = {});

        /**
         * Parses image from the given iterator
         *
         * @param policy the secure policy token
         * @param file the iterator to the file's data
         */
        void readImage(Policy policy, FileIter file);

        /**
         * Checks if the chunk crc code is valid
         *
         * @throw ImageLoadingFileCorruptionException when the
         * crc code does not match the saved one
         * @param begin the iterator to the begining of the chunk
         * @param length the length of the chunk
         */
        void checkCRCCode(FileIter begin, size_type length);

        /**
         * Determines which chunk is parsed now, parses it and
         * checks its crc checksum
         *
         * @param file the iterator to the file's data
         * @param length the length of the chunk
         */
        void parseChunk(FileIter& file, size_type length);

        /**
         * Returns an iterator to the decompressed data in the
         * given security manner
         *
         * @param buffer the constant reference to the buffer with
         * decompressed data
         * @return the iterator to the buffer
         */
        FileIter decompresedIter(
            DataBuffer const& buffer) const noexcept;

        /**
         * Chooses the interlancing method
         *
         * @param policy the security policy tag
         */
        void chooseInterlance(Policy policy);

        /**
         * Performs Adam7 interlance on the decompressed data
         *
         * @param policy the security policy tag
         * @param iter the reference to the decompressed data
         * iterator
         */
        void interlance(Policy policy, FileIter& iter);

        /**
         * Returns the dimensions of the interlanced subimage
         *
         * @param startX the x-axis start position of
         * the interlance
         * @param startY the y-axis start position of
         * the interlance
         * @param incrementX the x-axis step of interlance
         * @param incrementY the y-axis step of interlance
         * @return the dimensions vector
         */
        Vector2<size_type> subimageDimensions(
            uint32 startX,
            uint32 startY,
            uint32 incrementX,
            uint32 incrementY) const noexcept;

        DataBuffer                                  rawFileData;

        /**
         * Contains data obtained from the header
         */
        struct HeaderData {
            PixelsSetter                            setter;
            bool                                    interlance;
        }                                           headerData;

        typedef std::unique_ptr<ChunkInterface>     ChunkPtr;
        typedef std::function<ChunkPtr(PNGLoader&)> ChunkFun;
        typedef std::map<std::string, ChunkFun>     ChunkMap;
        typedef std::array<Vector4u, 7>             InteranceArray;

        static ChunkMap const                       chunkParsers;

        static constexpr uint64 const               MagicNumber
            = 0x0A1A0A0D474E5089;
        static constexpr uint64 const               IENDNumber
            = 0x826042AE444E4549;
        static constexpr InteranceArray const       InterlanceCoeff {
            Vector4u{0, 0, 8, 8},
            Vector4u{4, 0, 8, 8},
            Vector4u{0, 4, 4, 8},
            Vector4u{2, 0, 4, 4},
            Vector4u{0, 2, 2, 4},
            Vector4u{1, 0, 2, 2},
            Vector4u{0, 1, 1, 2}
        };
    };

    template class PNGLoader<Secured>;
    template class PNGLoader<Unsecured>;

}

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

#include <MPGL/IO/ImageLoading/LoaderInterface.hpp>
#include <MPGL/Compression/HuffmanTree.hpp>
#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Iterators/SafeIterator.hpp>
#include <MPGL/Mathematics/Matrix.hpp>

#include <functional>
#include <memory>
#include <tuple>
#include <queue>
#include <map>

namespace mpgl {

    /**
     * Loads the JPEG format image file
     *
     * @tparam Policy the secured policy token
     */
    template <security::SecurityPolicy Policy = Secured>
    class JPEGLoader : public LoaderInterface {
    public:
        typedef std::size_t                         size_type;
        typedef std::string                         Path;

        /// The JPEG tag
        static Path const                           Tag;

        /**
         * Constructs a new JPEGLoader object. Loads the
         * JPEG format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param filePath the path to the JPEG file
         */
        explicit JPEGLoader(Path const& filePath);

        /**
         * Constructs a new JPEGLoader object. Loads the
         * JPEG format image file from the given path
         *
         * @throw ImageLoadingFileOpenException when file cannot
         * be open
         * @throw ImageLoadingFileCorruptionException when file
         * is corrupted
         * @param policy the security policy tag
         * @param filePath the path to the JPEG file
         */
        explicit JPEGLoader(Policy policy, Path const& filePath);

        /**
         * Destroys the JPEGLoader object
         */
        ~JPEGLoader(void) noexcept = default;
    private:
        typedef std::vector<uint8>                  DataBuffer;
        typedef typename DataBuffer::const_iterator DataIter;
        typedef std::vector<char>                   FileBuffer;
        typedef FileBuffer::const_iterator          StreamIter;

        typedef std::conditional_t<
            security::isSecurePolicy<Policy>,
            SafeIterator<DataIter>,
            DataIter>                               SafeIter;

        typedef std::conditional_t<
            security::isSecurePolicy<Policy>,
            SafeIterator<StreamIter>, StreamIter>   FileIter;

        /**
         * Interface for all types of the JPEG data chunks
         */
        class ChunkInterface {
        public:
            /**
             * Constructs a new Chunk Interface object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit ChunkInterface(JPEGLoader& loader) noexcept
                : loader{loader} {}

            /**
             * Pure virtual operator. Has to be overloaded.
             * Parses the chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) = 0;

            /**
             * Virtual destructor. Destroys the Chunk Interface object
             */
            virtual ~ChunkInterface(void) noexcept = default;
        protected:
            JPEGLoader& loader;
        };

        /**
         * Parses the JPEG's DHT chunk
         */
        class DHTChunk : public ChunkInterface {
        public:
            /**
             * Constructs a new DHTChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit DHTChunk(JPEGLoader& loader) noexcept
                : ChunkInterface{loader} {}

            /**
             * Parses the DHT Chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) final;

            /**
             * Destroys the DHTChunk object
             */
            ~DHTChunk(void) noexcept = default;
        private:
            /**
             * Reads the chunk's data
             *
             * @param data the reference to the file's iterator
             * @return the chunk's header
             */
            uint8 readChunk(FileIter& data);

            std::array<uint8, 17>                   symbolsLengths{};
            DataBuffer                              characters{};
        };

        /**
         * Parses the JPEG's DQT chunk
         */
        struct DQTChunk : public ChunkInterface {
            /**
             * Constructs a new DQTChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit DQTChunk(JPEGLoader& loader) noexcept
                : ChunkInterface{loader} {}

            /**
             * Parses the DQT Chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) final;

            /**
             * Destroys the DQTChunk object
             */
            ~DQTChunk(void) noexcept = default;
        };

        /**
         * Parses the JPEG's SOF0 chunk
         */
        class SOF0Chunk : public ChunkInterface {
        public:
            /**
             * Constructs a new SOF0Chunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit SOF0Chunk(JPEGLoader& loader) noexcept
                : ChunkInterface{loader} {}

            /**
             * Parses the SOF0 Chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) final;

            /**
             * Destroys the SOF0Chunk object
             */
            ~SOF0Chunk(void) noexcept = default;
        private:
            /**
             * Parses the chunk's components
             *
             * @param data the reference to the file's iterator
             * @param length the chunk length - 8
             */
            void parseComponents(FileIter& data, uint16 length);
        };

        /**
         * Parses the JPEG's SOS chunk
         */
        class SOSChunk : public ChunkInterface {
        public:
            /**
             * Constructs a new SOSChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit SOSChunk(JPEGLoader& loader) noexcept
                : ChunkInterface{loader} {}

            /**
             * Parses the SOS Chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) final;

            /**
             * Destroys the SOSChunk object
             */
            ~SOSChunk(void) noexcept = default;
        private:
            /**
             * Returns whether the iterator is in the safe range
             *
             * @param data the reference to the file's iterator
             * @return if the iterator is in the safe range
             */
            bool iterable(FileIter& data) const noexcept;
        };

       /**
         * Skips the JPEG's chunk
         */
        struct EmptyChunk : public ChunkInterface {
            /**
             * Constructs a new EmptyChunk object. Passes
             * the reference to the loader
             *
             * @param loader the reference to the loader
             */
            explicit EmptyChunk(JPEGLoader& loader) noexcept
                : ChunkInterface{loader} {}

            /**
             * Skips the chunk data
             *
             * @param data the reference to the file's iterator
             */
            virtual void operator() (FileIter& data) final;

            /**
             * Destroys the Empty Chunk object
             */
            ~EmptyChunk(void) noexcept = default;
        };

        /**
         * Contains informations about decoded huffman trees
         */
        struct HuffmanTable {
            HuffmanTree<uint16>::Decoder            decoder;

            /**
             * Constructs a new Huffman Table object from the
             * given huffman tre
             *
             * @param tree the stored huffman tree
             */
            explicit HuffmanTable(HuffmanTree<uint16> tree);
        };

        /**
         * Contains informations about quantization tables
         */
        struct QuantizationTable {
            DataBuffer                              information;
            uint8                                   precision : 4;
        };

        /**
         * Contains information about JPEG's component
         */
        struct Component {
            uint8                                   verticalSampling : 4;
            uint8                                   horizontalSampling: 4;
            uint8                                   tableNumber;

            /**
             * Constructs a new Component object from the given
             * table number and the number of samples
             *
             * @param tableNumber the number of table
             * @param samplings the number of samplings
             */
            explicit Component(
                uint8 tableNumber,
                uint8 samplings) noexcept;
        };

        typedef std::unique_ptr<QuantizationTable>  QuantizationTablePtr;
        typedef std::unique_ptr<HuffmanTable>       HuffmanTablePtr;
        typedef std::unique_ptr<ChunkInterface>     ChunkPtr;
        typedef std::unique_ptr<Component>          ComponentPtr;
        typedef std::function<
            ChunkPtr(JPEGLoader&)>                  ChunkParser;
        typedef std::map<uint8, HuffmanTablePtr>    HuffmanMap;
        typedef std::map<bool, HuffmanMap>          HuffmanArray;
        typedef std::map<uint8, ComponentPtr>       ComponentArray;
        typedef std::map<uint8,
            QuantizationTablePtr>                   QuantizationArray;
        typedef std::map<uint8, Matrix8<int16>>     MatricesMap;
        typedef std::reference_wrapper<ChunkParser> ChunkParserRef;
        typedef BigEndianInputBitIter<SafeIter>     Iter;
        typedef std::array<int16, 64>               QuantizationData;
        typedef std::queue<ChunkParser>             ChunkQueue;
        typedef std::map<uint16, ChunkParser>       ParserMap;
        typedef std::vector<int16>                  Channels;

        /**
         * The tuple of three matrixes, containing information
         * about each color component
         *
         * @tparam Tp the matrix's element type
         */
        template <typename Tp>
        using PixelMatrix = std::tuple<
            Matrix8<Tp>,
            Matrix8<Tp>,
            Matrix8<Tp>
        >;

        /**
         * Converts YCbCr system to RGB system
         *
         * @param yCbCr the pixel matrix containing YCbCr values
         * @return the pixel matrix containing RGB values
         */
        static PixelMatrix<uint8> convertYCbCrToRGB(
            PixelMatrix<int16> const& yCbCr) noexcept;

        /**
         * Converts the part of the YCbCr matrix to RGB value
         *
         * @param yCbCr the pixel matrix containing YCbCr values
         * @param rgb the reference to the rgb matrix
         * @param x the x-axis coordinate
         * @param y the y-axis coordinate
         */
        static void convertYCbCrToRGB(
            PixelMatrix<int16> const& yCbCr,
            PixelMatrix<uint8>& rgb,
            uint8 x,
            uint8 y) noexcept;

        /**
         * Returns the boundry of the decoding iterator
         *
         * @param boundry the boundry of the image
         * @return the boundry of the iterator
         */
        static size_type getBoundry(size_type boundry) noexcept;

        /**
         * Clamps the subpixel color values
         *
         * @param color the subpixel color value
         * @return the clamped subpixel valie
         */
        static uint8 adjustPixelColor(int16 color) noexcept;

        /**
         * Decodes the VLC code
         *
         * @param code the code value
         * @param bits the code length
         * @return the decoded code value
         */
        static int32 decodeNumber(uint8 code, uint16 bits) noexcept;

        /**
         * Sets the secure policy
         *
         * @throw SecurityUnknownPolicyException when the security
         * policy token is unknown
         * @param file the constant reference to the file data object
         */
        void setPolicy(FileBuffer const& file);

        /**
         * Determines which chunk is parsed now and parses it
         *
         * @param file the iterator to the file's data
         */
        void parseChunks(FileIter file);

        /**
         * Parses the next chunk header
         *
         * @param file the iterator to the file's data
         * @param signature the chunk's signature
         */
        void parseNextChunk(FileIter& file, uint16 signature);

        /**
         * Reads the 8x8 matrix from the iterator
         *
         * @param iter the reference to the iterator to
         * the file's data
         * @param id the component's id
         * @param coeff the main coefficient
         * @return the read matrix
         */
        Matrix8<int16> readMatrix(Iter& iter, uint8 id, int16& coeff);

        /**
         * Decodes the given matrix
         *
         * @param data the reference to the quantization data
         * @param table the constant reference to the huffman
         * table pointer
         * @param quant the constant reference to the quantization
         * table
         * @param iter the reference to the iterator to
         * the file's data
         */
        void decodeMatrix(
            QuantizationData& data,
            HuffmanTablePtr const& table,
            QuantizationTablePtr const& quant,
            Iter& iter);

        /**
         * Returns the iterator to the decoded data in the given
         * security manner
         *
         * @return the iterator to the decoded data
         */
        Iter getDecodeIterator(void) noexcept;

        /**
         * Decodes the parsed image
         */
        void decodeImage(void);

        /**
         * Decodes the block of the image
         *
         * @param iter the reference to the iterator to
         * the file's data
         * @param y the image's column
         * @param x the image's row
         * @param channels the reference to the channels vector
         */
        void decodeImageBlock(
            Iter& iter,
            size_type y,
            size_type x,
            Channels& channels);

        /**
         * Converts YCbCr to the RGB and draws it on the image
         *
         * @param matrices the reference to the matrices map
         * @param row the row size
         * @param column the column size
         */
        void drawYCbCrOnImage(
            MatricesMap& matrices,
            size_type row,
            size_type column);

        /**
         * Draws the block on the image
         *
         * @param red the constant reference to the red matrix
         * @param green the constant reference to the green matrix
         * @param blue the constant reference to the blue matrix
         * @param coordinates the coordinates
         * @param bases the bases
         */
        void drawBlockOnImage(
            Matrix8<uint8> const& red,
            Matrix8<uint8> const& green,
            Matrix8<uint8> const& blue,
            Vector2<size_type> const& coordinates,
            Vector2<size_type> const& bases);

        ComponentArray                              componentsTable;
        QuantizationArray                           quantizationTables;
        ChunkQueue                                  parsingQueue;
        DataBuffer                                  imageData;
        HuffmanArray                                huffmanTables;
        bool                                        endOfImage;

        static ChunkParser const                    emptyChunk;
        static ParserMap const                      chunkParser;
    };

    template class JPEGLoader<Secured>;
    template class JPEGLoader<Unsecured>;

}

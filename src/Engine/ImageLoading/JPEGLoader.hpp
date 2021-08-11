#pragma once

#include "../Compression/HuffmanTree.hpp"
#include "../Collections/BitIterator.hpp"
#include "../Mathematics/Matrix.hpp"
#include "../Compression/IDCT.hpp"
#include "LoaderInterface.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <queue>
#include <map>

namespace ge {

    class JPEGLoader : public LoaderInterface {
    public:
        explicit JPEGLoader(const std::string& fileName);

        static const std::string Tag;

        ~JPEGLoader(void) noexcept = default;
    private:
        class ChunkInterface {
        public:
            explicit ChunkInterface(JPEGLoader& loader) noexcept : loader{loader} {}
            virtual void operator() (std::istream& data) = 0;
            virtual ~ChunkInterface(void) noexcept = default;
        protected:
            JPEGLoader& loader;
        };

        struct DHTChunk : public ChunkInterface {
            explicit DHTChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~DHTChunk(void) noexcept = default;
        };

        struct DQTChunk : public ChunkInterface {
            explicit DQTChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~DQTChunk(void) noexcept = default;
        };

        struct SOF0Chunk : public ChunkInterface {
            explicit SOF0Chunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~SOF0Chunk(void) noexcept = default;
        };

        struct SOSChunk : public ChunkInterface {
            explicit SOSChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~SOSChunk(void) noexcept = default;
        };

        struct EmptyChunk : public ChunkInterface {
            explicit EmptyChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~EmptyChunk(void) noexcept = default;
        };

        struct HuffmanTable {
            HuffmanTree<uint16_t>::Decoder decoder;
            explicit HuffmanTable(HuffmanTree<uint16_t> tree);
        };

        struct QuantizationTable {
            std::vector<uint8_t> information;
            uint8_t precision : 4;
        };

        struct Component {
            uint8_t verticalSampling : 4;
            uint8_t horizontalSampling: 4;
            uint8_t tableNumber;
            explicit Component(uint8_t tableNumber, uint8_t samplings) noexcept;
        };

        typedef std::unique_ptr<QuantizationTable> QuantizationTablePtr;
        typedef std::unique_ptr<HuffmanTable>   HuffmanTablePtr;

        typedef std::function<std::unique_ptr<ChunkInterface>(JPEGLoader&)> ChunkParser;
        typedef BigEndianBitIter<std::vector<uint8_t>::const_iterator>      Iter;
        typedef std::map<bool, std::map<uint8_t, HuffmanTablePtr>>          HuffmanArray;
        typedef std::map<uint8_t, std::unique_ptr<Component>>               ComponentArray;
        typedef std::map<uint8_t, QuantizationTablePtr>                     QuantizationArray;
        typedef std::map<uint8_t, Matrix<int16_t, 8>>                       MatricesMap;
        typedef std::reference_wrapper<ChunkParser>                         ChunkParserRef;

        template <typename T>
        using PixelMatrix = std::tuple<Matrix<T, 8>, Matrix<T, 8>, Matrix<T, 8>>;

        static PixelMatrix<uint8_t> convertYCbCrToRGB(PixelMatrix<int16_t> yCbCr) noexcept;
        static std::size_t getBoundry(std::size_t boundry) noexcept;
        static uint8_t adjustPixelColor(int16_t color) noexcept;
        static int32_t decodeNumber(uint8_t code, uint16_t bits) noexcept;

        void parseChunks(std::ifstream& file);
        void parseNextChunk(std::istream& file, uint16_t signature);
        Matrix<int16_t, 8> readMatrix(Iter& iter, uint8_t id, int16_t& coeff) noexcept;
        void decodeMatrix(std::array<int16_t, 64>& data, HuffmanTablePtr const& table,
            QuantizationTablePtr const& quant, Iter& iter) noexcept;
        void decodeImage(void);
        void drawYCbCrOnImage(MatricesMap& matrices, std::size_t row, std::size_t column) noexcept;

        ComponentArray componentsTable;
        QuantizationArray quantizationTables;
        std::queue<ChunkParser> parsingQueue;
        std::vector<uint8_t> imageData;
        HuffmanArray huffmanTables;
        bool endOfImage;

        static const IDCT<>     inverseCosineTransform;
        static const ChunkParser emptyChunk;
        static const std::map<uint16_t, ChunkParser> chunkParser;
    };

}

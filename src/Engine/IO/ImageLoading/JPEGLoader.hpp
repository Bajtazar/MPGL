#pragma once

#include "../../Compression/HuffmanTree.hpp"
#include "../../Iterators/SafeIterator.hpp"
#include "../../Iterators/BitIterator.hpp"
#include "../../Mathematics/Matrix.hpp"
#include "../../Mathematics/IDCT.hpp"
#include "../../Utility/Security.hpp"
#include "LoaderInterface.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <queue>
#include <map>

namespace ge {

    template <security::SecurityPolicy Policy = Secured>
    class JPEGLoader : public LoaderInterface {
    public:
        explicit JPEGLoader(const std::string& fileName);
        explicit JPEGLoader(Policy policy, const std::string& fileName);

        static const std::string Tag;

        ~JPEGLoader(void) noexcept = default;
    private:
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<std::vector<uint8_t>::const_iterator>,
            std::vector<uint8_t>::const_iterator>                   SafeIter;
        typedef std::istreambuf_iterator<char>                      StreamBuf;
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<StreamBuf>, StreamBuf>                     FileIter;

        class ChunkInterface {
        public:
            explicit ChunkInterface(JPEGLoader<Policy>& loader) noexcept : loader{loader} {}
            virtual void operator() (FileIter& data) = 0;
            virtual ~ChunkInterface(void) noexcept = default;
        protected:
            JPEGLoader& loader;
        };

        struct DHTChunk : public ChunkInterface {
            explicit DHTChunk(JPEGLoader<Policy>& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (FileIter& data) final;
            ~DHTChunk(void) noexcept = default;
        };

        struct DQTChunk : public ChunkInterface {
            explicit DQTChunk(JPEGLoader<Policy>& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (FileIter& data) final;
            ~DQTChunk(void) noexcept = default;
        };

        struct SOF0Chunk : public ChunkInterface {
            explicit SOF0Chunk(JPEGLoader<Policy>& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (FileIter& data) final;
            ~SOF0Chunk(void) noexcept = default;
        };

        class SOSChunk : public ChunkInterface {
        public:
            explicit SOSChunk(JPEGLoader<Policy>& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (FileIter& data) final;
            ~SOSChunk(void) noexcept = default;
        private:
            bool iterable(FileIter& data) const noexcept;
        };

        struct EmptyChunk : public ChunkInterface {
            explicit EmptyChunk(JPEGLoader<Policy>& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (FileIter& data) final;
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
        typedef std::map<bool, std::map<uint8_t, HuffmanTablePtr>>          HuffmanArray;
        typedef std::map<uint8_t, std::unique_ptr<Component>>               ComponentArray;
        typedef std::map<uint8_t, QuantizationTablePtr>                     QuantizationArray;
        typedef std::map<uint8_t, Matrix<int16_t, 8>>                       MatricesMap;
        typedef std::reference_wrapper<ChunkParser>                         ChunkParserRef;
        typedef BigEndianBitIter<SafeIter>                                  Iter;

        template <typename T>
        using PixelMatrix = std::tuple<Matrix<T, 8>, Matrix<T, 8>, Matrix<T, 8>>;

        static PixelMatrix<uint8_t> convertYCbCrToRGB(PixelMatrix<int16_t> yCbCr) noexcept;
        static std::size_t getBoundry(std::size_t boundry) noexcept;
        static uint8_t adjustPixelColor(int16_t color) noexcept;
        static int32_t decodeNumber(uint8_t code, uint16_t bits) noexcept;

        void setPolicy(std::istream& file);
        void parseChunks(FileIter file);
        void parseNextChunk(FileIter& file, uint16_t signature);
        Matrix<int16_t, 8> readMatrix(Iter& iter, uint8_t id, int16_t& coeff);
        void decodeMatrix(std::array<int16_t, 64>& data, HuffmanTablePtr const& table,
            QuantizationTablePtr const& quant, Iter& iter);
        Iter getDecodeIterator(void) noexcept;
        void decodeImage(void);
        void drawYCbCrOnImage(MatricesMap& matrices, std::size_t row, std::size_t column);

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

    template class JPEGLoader<Secured>;
    template class JPEGLoader<Unsecured>;

}

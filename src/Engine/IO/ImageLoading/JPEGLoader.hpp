#pragma once

#include "../../Compression/HuffmanTree.hpp"
#include "../../Iterators/SafeIterator.hpp"
#include "../../Mathematics/Matrix.hpp"
#include "../../Mathematics/IDCT.hpp"
#include "../../Utility/Security.hpp"
#include "LoaderInterface.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <queue>
#include <map>

namespace mpgl {

    template <security::SecurityPolicy Policy = Secured>
    class JPEGLoader : public LoaderInterface {
    public:
        explicit JPEGLoader(const std::string& fileName);
        explicit JPEGLoader(Policy policy, const std::string& fileName);

        static const std::string Tag;

        ~JPEGLoader(void) noexcept = default;
    private:
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<std::vector<uint8>::const_iterator>,
            std::vector<uint8>::const_iterator>                     SafeIter;
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
            HuffmanTree<uint16>::Decoder decoder;
            explicit HuffmanTable(HuffmanTree<uint16> tree);
        };

        struct QuantizationTable {
            std::vector<uint8> information;
            uint8 precision : 4;
        };

        struct Component {
            uint8 verticalSampling : 4;
            uint8 horizontalSampling: 4;
            uint8 tableNumber;
            explicit Component(uint8 tableNumber, uint8 samplings) noexcept;
        };

        typedef std::unique_ptr<QuantizationTable> QuantizationTablePtr;
        typedef std::unique_ptr<HuffmanTable>   HuffmanTablePtr;

        typedef std::function<std::unique_ptr<ChunkInterface>(JPEGLoader&)> ChunkParser;
        typedef std::map<bool, std::map<uint8, HuffmanTablePtr>>          HuffmanArray;
        typedef std::map<uint8, std::unique_ptr<Component>>               ComponentArray;
        typedef std::map<uint8, QuantizationTablePtr>                     QuantizationArray;
        typedef std::map<uint8, Matrix8<int16>>                         MatricesMap;
        typedef std::reference_wrapper<ChunkParser>                         ChunkParserRef;
        typedef BigEndianBitIter<SafeIter>                                  Iter;

        template <typename T>
        using PixelMatrix = std::tuple<Matrix8<T>, Matrix8<T>, Matrix8<T>>;

        static PixelMatrix<uint8> convertYCbCrToRGB(PixelMatrix<int16> yCbCr) noexcept;
        static std::size_t getBoundry(std::size_t boundry) noexcept;
        static uint8 adjustPixelColor(int16 color) noexcept;
        static int32 decodeNumber(uint8 code, uint16 bits) noexcept;

        void setPolicy(std::istream& file);
        void parseChunks(FileIter file);
        void parseNextChunk(FileIter& file, uint16 signature);
        Matrix8<int16> readMatrix(Iter& iter, uint8 id, int16& coeff);
        void decodeMatrix(std::array<int16, 64>& data, HuffmanTablePtr const& table,
            QuantizationTablePtr const& quant, Iter& iter);
        Iter getDecodeIterator(void) noexcept;
        void decodeImage(void);
        void drawYCbCrOnImage(MatricesMap& matrices, std::size_t row, std::size_t column);

        ComponentArray componentsTable;
        QuantizationArray quantizationTables;
        std::queue<ChunkParser> parsingQueue;
        std::vector<uint8> imageData;
        HuffmanArray huffmanTables;
        bool endOfImage;

        static const IDCT<>     inverseCosineTransform;
        static const ChunkParser emptyChunk;
        static const std::map<uint16, ChunkParser> chunkParser;
    };

    template class JPEGLoader<Secured>;
    template class JPEGLoader<Unsecured>;

}

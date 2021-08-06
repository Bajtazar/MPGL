#pragma once

#include "../Compression/HuffmanTree.hpp"
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
            uint8_t number : 4;
            bool type : 1;
            explicit HuffmanTable(HuffmanTree<uint16_t> tree, uint8_t header, const std::string& fileName);
        };

        struct QuantizationTable {
            std::vector<uint8_t> information;
            uint8_t number : 4;
            uint8_t precision : 4;
        };

        struct Component {
            uint8_t id;
            uint8_t verticalSampling : 4;
            uint8_t horizontalSampling: 4;
            uint8_t tableNumber;
            explicit Component(uint8_t tableNumber, uint8_t samplings, uint8_t id) noexcept;
        };

        typedef std::function<std::unique_ptr<ChunkInterface>(JPEGLoader&)> ChunkParser;
        typedef std::reference_wrapper<ChunkParser> ChunkParserRef;

        void parseChunks(std::ifstream& file);
        void parseNextChunk(std::istream& file, uint16_t signature);

        std::queue<ChunkParser> parsingQueue;
        std::vector<HuffmanTable> huffmanTables;
        std::vector<QuantizationTable> quantizationTables;
        std::vector<Component> componentsTable;
        std::vector<uint8_t> imageData;
        bool endOfImage;

        static const ChunkParser emptyChunk;
        static const std::map<uint16_t, ChunkParser> chunkParser;
    };

}

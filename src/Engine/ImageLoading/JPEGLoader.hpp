#pragma once

#include "../Compression/HuffmanTree.hpp"
#include "LoaderInterface.hpp"

#include <functional>
#include <memory>
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

        struct StartOfScanChunk : public ChunkInterface {
            explicit StartOfScanChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~StartOfScanChunk(void) noexcept = default;
        };

        struct DHTChunk : public ChunkInterface {
            explicit DHTChunk(JPEGLoader& loader) noexcept : ChunkInterface{loader} {}
            virtual void operator() (std::istream& data) final;
            ~DHTChunk(void) noexcept = default;
        };

        void parseChunks(std::ifstream& file);
        void invokeParser(uint16_t signature, std::istream& file);

        uint8_t huffmanTrees : 2;

        static const std::map<uint16_t, std::function<std::unique_ptr<ChunkInterface> (JPEGLoader&)>> chunkParser;
    };

}

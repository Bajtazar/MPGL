#pragma once

#include "LoaderInterface.hpp"
#include "../Utility/FunctionalWrapper.hpp"

#include <map>
#include <array>
#include <functional>

namespace ge {

    class PNGLoader : public LoaderInterface {
    public:
        explicit PNGLoader(const std::string& fileName);

        static const std::string Tag;

        ~PNGLoader(void) noexcept = default;
    private:
        static std::array<uint32_t, 256> createCRCTable(void) noexcept;
        void readImage(std::ifstream& file);

        struct ChunkInterface {
            explicit ChunkInterface(void) = default;
            virtual void operator() (std::size_t length, std::istream& data, PNGLoader&) = 0;
            virtual ~ChunkInterface(void) = default;
        };

        class IHDRChunk : public ChunkInterface {
        public:
            explicit IHDRChunk(void) = default;
            virtual void operator() (std::size_t length, std::istream& data, PNGLoader&) final;
            ~IHDRChunk(void) = default;
        private:
            void parseBitDepth(const uint8_t& depth, PNGLoader& loader);
            void parseColorType(const uint8_t& colorType, PNGLoader& loader);
            void parseInterlance(const uint8_t& interlance, PNGLoader& loader);
        };

        struct HeaderData {
            enum class Types {
                RGB,
                RGBA
            } type;
        } headerData;

        static const std::map<std::string, std::function<std::unique_ptr<ChunkInterface> (void)>> chunkParsers;
        static const std::array<uint32_t, 256> crcTable;
    };

}

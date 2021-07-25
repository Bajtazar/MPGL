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
        template <typename T>
        static void readCRCCode(uint32_t& crc, const T& data) noexcept;
        template <std::size_t N>
        static void readCRCCode(uint32_t& crc, const char (&data)[N]) noexcept;
        static std::array<uint32_t, 256> createCRCTable(void) noexcept;
        void readImage(std::ifstream& file);

        class ChunkInterface {
        public:
            explicit ChunkInterface(void) noexcept : crcCode{ 0xFFFFFFFF } {}
            virtual void operator() (std::size_t length, std::istream& data, PNGLoader&) = 0;
            virtual ~ChunkInterface(void) = default;
        protected:
            uint32_t crcCode;
            void checkCRCCode(const uint32_t& crc, const PNGLoader& loader);
        };

        class IHDRChunk : public ChunkInterface {
        public:
            explicit IHDRChunk(void) noexcept { readCRCCode(crcCode, "RDHI"); }
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

    template <typename T>
    void PNGLoader::readCRCCode(uint32_t& crc, const T& data) noexcept {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data) + sizeof(T) - 1;
        for (uint16_t i = sizeof(T); i != 0; --i, --ptr)
            crc = crcTable[(crc ^ (*ptr)) & 0xFF] ^ (crc >> 8);
    }

    template <std::size_t N>
    void PNGLoader::readCRCCode(uint32_t& crc, const char (&data)[N]) noexcept {
        const char* ptr = &data[N - 2];
        for (uint16_t i = N - 1; i != 0; --i, --ptr)
            crc = crcTable[(crc ^ (*ptr)) & 0xFF] ^ (crc >> 8);
    }

}

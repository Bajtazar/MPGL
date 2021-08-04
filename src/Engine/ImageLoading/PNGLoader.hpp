#pragma once

#include "LoaderInterface.hpp"
#include "../Utility/FunctionalWrapper.hpp"

#include <map>
#include <array>
#include <deque>
#include <functional>

namespace ge {

    class PNGLoader : public LoaderInterface {
    public:
        explicit PNGLoader(const std::string& fileName);

        static const std::string Tag;

        ~PNGLoader(void) noexcept = default;
    private:
        typedef std::vector<char>::const_iterator                                   CharIter;
        typedef void(PNGLoader::*PixelsSetter)(std::size_t, std::size_t, uint8_t, CharIter&);

        template <typename T>
        static void readCRCCode(uint32_t& crc, const T& data) noexcept;
        template <std::size_t N>
        static void readCRCCode(uint32_t& crc, const char (&data)[N]) noexcept;
        static std::array<uint32_t, 256> createCRCTable(void) noexcept;

        void readImage(std::ifstream& file);

        class ChunkInterface {
        public:
            explicit ChunkInterface(PNGLoader& loader) noexcept : crcCode{ 0xFFFFFFFF }, loader{ loader } {}
            virtual void operator() (std::size_t length, std::istream& data) = 0;
            virtual ~ChunkInterface(void) = default;
        protected:
            void checkCRCCode(const uint32_t& crc) const;

            uint32_t crcCode;
            PNGLoader& loader;
        };

        class IHDRChunk : public ChunkInterface {
        public:
            explicit IHDRChunk(PNGLoader& loader) noexcept : ChunkInterface{ loader } { readCRCCode(crcCode, "RDHI"); }
            virtual void operator() (std::size_t length, std::istream& data) final;
            ~IHDRChunk(void) = default;
        private:
            void parseBitDepth(const uint8_t& depth);
            void parseColorType(const uint8_t& colorType);
            void parseInterlance(const uint8_t& interlance);
        };

        class IDATChunk : public ChunkInterface {
        public:
            explicit IDATChunk(PNGLoader& loader) noexcept : ChunkInterface{ loader } { readCRCCode(crcCode, "TADI"); }
            virtual void operator() (std::size_t length, std::istream& data) final;
            ~IDATChunk(void) = default;
        };

        std::deque<char> rawFileData;
        struct HeaderData {
            enum class Types {
                RGB,
                RGBA,
                GRAY,
                GRAYALPHA
            } type;
        } headerData;

        uint8_t paethPredictor(uint8_t a, uint8_t b, uint8_t c) const noexcept;
        uint8_t reconstructA(std::size_t row, std::size_t column, uint8_t pixel) const noexcept;
        uint8_t reconstructB(std::size_t row, std::size_t column, uint8_t pixel) const noexcept;
        uint8_t reconstructC(std::size_t row, std::size_t column, uint8_t pixel) const noexcept;
        void filterPixels(const std::vector<char>& data) noexcept;
        void setRGBAPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept;
        void setRGBPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept;
        void setGrayPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept;
        void setGrayAlphaPixels(std::size_t row, std::size_t column, uint8_t filter, CharIter& iter) noexcept;
        PixelsSetter getPixelsSetter(void) const noexcept;

        static const std::map<std::string, std::function<std::unique_ptr<ChunkInterface> (PNGLoader&)>> chunkParsers;
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

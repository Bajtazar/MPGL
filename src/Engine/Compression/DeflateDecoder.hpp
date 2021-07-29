#pragma once

#include "../Utility/BitIterator.hpp"
#include "HuffmanTree.hpp"

#include <deque>
#include <vector>
#include <inttypes.h>

namespace ge {

    class DeflateDecoder {
    public:
        explicit DeflateDecoder(std::deque<char>& rawData);

        enum class CompressionLevel : uint8_t {
            Fastest,
            Fast,
            Default,
            Maximum
        };

        CompressionLevel getCompressionLevel(void) const noexcept;
        void decompress(void);
    private:
        typedef BitIterator<std::deque<char>::iterator>  BitIter;

        void parseHeader(void);
        void saveAdler32Code(void);
        bool readBlock(BitIter& iterator);

        void copyNotCompressed(BitIter& iterator);
        void decompressBlock(BitIter& iterator);

        uint32_t calculateAlder32(void) const noexcept;

        std::deque<char>& rawData;
        std::vector<char> outputStream;

        uint32_t adler32Code;
        uint8_t compressionMethod : 2;

        static HuffmanTree<uint16_t>::Decoder fixedCodeDecoder;
        static constexpr const uint16_t BlockEnd = 0x0100;
        static constexpr const uint32_t AdlerBase = 65521;
    };

}

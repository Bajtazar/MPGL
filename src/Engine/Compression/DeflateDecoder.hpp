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
        std::vector<char>& decompress(void);
    private:
        typedef BitIterator<std::deque<char>::iterator>  BitIter;
        typedef HuffmanTree<uint16_t>::Decoder Decoder;

        void parseHeader(void);
        void saveAdler32Code(void);
        bool readBlock(BitIter& iterator);

        void copyNotCompressed(BitIter& iterator);
        void decompressFixedBlock(BitIter& iterator);
        void decompressDynamicBlock(BitIter& iterator);
        void decompressFixedDistance(uint16_t token, BitIter& iterator);
        void decompressDynamicDistance(uint16_t token, BitIter& iterator, const HuffmanTree<uint16_t>::Decoder& distanceDecoder);
        std::pair<Decoder, Decoder> generateDynamicTrees(const Decoder& decoder, uint32_t literals,
            uint32_t distances, BitIter& iterator) const;
        void dynamicBlockLoop(const Decoder& mainDecoder, const Decoder& distanceDecoder, BitIter& iterator);

        uint32_t calculateAlder32(void) const noexcept;

        std::deque<char>& rawData;
        std::vector<char> outputStream;

        uint32_t adler32Code;
        uint8_t compressionMethod : 2;

        static HuffmanTree<uint16_t>::Decoder fixedCodeDecoder;
        static constexpr const uint16_t MaxAlphabetLength = 288;
        static constexpr const uint16_t BlockEnd = 256;
        static constexpr const uint32_t AdlerBase = 65521;
        static constexpr const std::array<std::pair<uint8_t, uint16_t>, 29> extraLength = {
            std::pair<uint8_t, uint8_t>{0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
            {0, 9}, {0, 10}, {1, 11}, {1, 13}, {1, 15}, {1, 17}, {2, 19}, {2, 23}, {2, 27},
            {2, 31}, {3, 35}, {3, 43}, {3, 51}, {3, 59}, {4, 67}, {4, 83},
            {4, 99}, {4, 115}, {5, 131}, {5, 163}, {5, 195}, {5, 227}, {0, 258}
        };
        static constexpr const std::array<std::pair<uint8_t, uint32_t>, 30> distances = {
            std::pair<uint8_t, uint32_t>{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 5}, {1, 7},
            {2, 9}, {2, 13}, {3, 17}, {3, 25}, {4, 33}, {4, 49}, {5, 65}, {5, 97}, {6, 129},
            {6, 193}, {7, 257}, {7, 385}, {8, 513}, {8, 769}, {9, 1025}, {9, 1537}, {10, 2049},
            {10, 3073}, {11, 4097}, {11, 6145}, {12, 8193}, {12, 12289}, {13, 16385}, {13, 24577}
        };
        static constexpr const std::array<uint8_t, 19> dynamicCodesOrder = {
            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
        };
    };

}

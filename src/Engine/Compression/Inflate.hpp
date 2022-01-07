#pragma once

#include "../Iterators/SafeIterator.hpp"
#include "../Iterators/BitIterator.hpp"
#include "../Utility/Security.hpp"
#include "HuffmanTree.hpp"

#include <deque>
#include <vector>
#include <inttypes.h>

namespace ge {

    template <security::SecurityPolicy Policy = Secured>
    class Inflate {
    public:
        explicit Inflate(std::deque<char>& rawData);
        explicit Inflate(Policy policy, std::deque<char>& rawData);

        enum class CompressionLevel : uint8 {
            Fastest,
            Fast,
            Default,
            Maximum
        };

        CompressionLevel getCompressionLevel(void) const noexcept;
        std::vector<char>& decompress(void);
        bool getDiagnosticStatus(void) const noexcept { return diagnostics; }
        void setDiagnostic(bool status) noexcept { diagnostics = status; }
    private:
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<std::deque<char>::iterator>,
            std::deque<char>::iterator>                         SafeIter;
        typedef LittleEndianBitIter<SafeIter>                   BitIter;
        typedef HuffmanTree<uint16>::Decoder                  Decoder;

        SafeIter getIterator(void);
        void parseHeader(void);
        void saveAdler32Code(void);
        bool readBlock(BitIter& iterator);

        void copyNotCompressed(BitIter& iterator);
        void decompressFixedBlock(BitIter& iterator);
        void decompressDynamicBlock(BitIter& iterator);
        void decompressFixedDistance(uint16 token, BitIter& iterator);
        void decompressDynamicDistance(uint16 token, BitIter& iterator, const HuffmanTree<uint16>::Decoder& distanceDecoder);
        std::pair<Decoder, Decoder> generateDynamicTrees(const Decoder& decoder, uint32 literals,
            uint32 distances, BitIter& iterator) const;
        void dynamicBlockLoop(const Decoder& mainDecoder, const Decoder& distanceDecoder, BitIter& iterator);
        uint16 readCodeLengthAlphabet(std::size_t& repeater, BitIter& iter,
            std::vector<uint16>& bitlengths, uint16 token) const;
        std::vector<uint16> readBitLengths(const Decoder& decoder, uint32 literals,
            uint32 distances, BitIter& iterator) const;

        uint32 calculateAlder32(void) const noexcept;

        std::deque<char>& rawData;
        std::vector<char> outputStream;

        uint32 adler32Code;
        uint8 compressionMethod : 2;

        static HuffmanTree<uint16>::Decoder fixedCodeDecoder;
        static constexpr const uint16 MaxAlphabetLength = 288;
        static constexpr const uint16 BlockEnd = 256;
        static constexpr const uint32 AdlerBase = 65521;
        static constexpr const std::array<std::pair<uint8, uint16>, 29> extraLength = {
            std::pair<uint8, uint8>{0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
            {0, 9}, {0, 10}, {1, 11}, {1, 13}, {1, 15}, {1, 17}, {2, 19}, {2, 23}, {2, 27},
            {2, 31}, {3, 35}, {3, 43}, {3, 51}, {3, 59}, {4, 67}, {4, 83},
            {4, 99}, {4, 115}, {5, 131}, {5, 163}, {5, 195}, {5, 227}, {0, 258}
        };
        static constexpr const std::array<std::pair<uint8, uint32>, 30> distances = {
            std::pair<uint8, uint32>{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 5}, {1, 7},
            {2, 9}, {2, 13}, {3, 17}, {3, 25}, {4, 33}, {4, 49}, {5, 65}, {5, 97}, {6, 129},
            {6, 193}, {7, 257}, {7, 385}, {8, 513}, {8, 769}, {9, 1025}, {9, 1537}, {10, 2049},
            {10, 3073}, {11, 4097}, {11, 6145}, {12, 8193}, {12, 12289}, {13, 16385}, {13, 24577}
        };
        static constexpr const std::array<uint8, 19> dynamicCodesOrder = {
            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
        };
        static bool diagnostics;
    };

    template class Inflate<Secured>;
    template class Inflate<Unsecured>;

}

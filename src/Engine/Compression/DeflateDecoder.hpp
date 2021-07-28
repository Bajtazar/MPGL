#pragma once

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
    private:
        void parseHeader(void);
        void saveAdler32Code(void);
        uint32_t calculateAlder32(void) const noexcept;

        std::deque<char>& rawData;
        std::vector<char> outputStream;

        uint32_t adler32Code;
        uint8_t compressionMethod : 2;
        bool isDict : 1;

        static constexpr const uint32_t AdlerBase = 65521;
    };

}

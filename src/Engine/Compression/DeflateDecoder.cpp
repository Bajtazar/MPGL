#include "DeflateDecoder.hpp"

#include "../Exceptions/NotSupportedException.hpp"
#include "../Exceptions/DeflateDecoderInvalidHeaderException.hpp"
#include "../Exceptions/DeflateDecoderDataCorruptionException.hpp"

#include <bitset>
#include <ranges>
#include <algorithm>

namespace ge {

    HuffmanTree<uint16_t>::Decoder DeflateDecoder::fixedCodeDecoder{};

    DeflateDecoder::DeflateDecoder(std::deque<char>& rawData) : rawData{rawData} {
        if (rawData.size() < 6)
            throw DeflateDecoderDataCorruptionException{};
        try {
            parseHeader();
        } catch (HuffmanTreeException&) {
            if (diagnostics)
                throw;
            else
                throw DeflateDecoderDataCorruptionException{};
        }
        if (rawData.size() < 4)
            throw DeflateDecoderDataCorruptionException{};
        saveAdler32Code();
    }

    DeflateDecoder::CompressionLevel DeflateDecoder::getCompressionLevel(void) const noexcept {
        switch(compressionMethod) {
            case 0:
                return CompressionLevel::Fastest;
            case 1:
                return CompressionLevel::Fast;
            case 2:
                return CompressionLevel::Default;
            case 3:
                return CompressionLevel::Maximum;
        }
        return CompressionLevel::Default;
    }

    void DeflateDecoder::parseHeader(void) {
        uint8_t cmf = rawData[0], flg = rawData[1];
        rawData.erase(rawData.begin(), rawData.begin() + 2);
        if (cmf != 0x78)    // @TODO add later more indepth info
            throw DeflateDecoderInvalidHeaderException{};
        if (((256u * cmf + flg) % 31))
            throw DeflateDecoderDataCorruptionException{};
        std::bitset<8> bits{flg};
        if (bits[5]) // isDict
            throw NotSupportedException{"No-default dicts are not supported."};
        compressionMethod = bits[6] + 2 * bits[7];
    }

    std::vector<char>& DeflateDecoder::decompress(void) {
        BitIter iterator {SafeIter{rawData.begin(), rawData.end()}};
        while (readBlock(iterator));
        // adler32
        return outputStream;
    }

    bool DeflateDecoder::readBlock(BitIter& iterator) {
        bool isFinal = *iterator++;
        if (*iterator++) {
            if (*iterator++)
                throw DeflateDecoderDataCorruptionException{};
            else
                decompressFixedBlock(iterator);
        } else {
            if (*iterator++)
                decompressDynamicBlock(iterator);
            else
                copyNotCompressed(iterator);
        }
        return !isFinal;
    }

    void DeflateDecoder::decompressFixedBlock(BitIter& iterator) {
        auto token = fixedCodeDecoder.decodeToken(iterator);
        for (;token != BlockEnd; token = fixedCodeDecoder.decodeToken(iterator)) {
            if (token < BlockEnd)
                outputStream.push_back(token);
            else
                decompressFixedDistance(token - 257, iterator);
        }
    }

    void DeflateDecoder::decompressFixedDistance(uint16_t token, BitIter& iterator) {
        auto [lenBits, length] = extraLength.at(token);
        length += readNBits<uint16_t>(lenBits, iterator);
        uint16_t distanceToken = readRNBits<uint8_t>(5, iterator);
        auto [distBits, distance] = distances.at(distanceToken);
        distance += readNBits<uint32_t>(distBits, iterator);
        uint32_t offset = outputStream.size() - distance;
        for (auto i : std::views::iota(0u, length))
            outputStream.push_back(outputStream.at(offset + i));
    }

    std::pair<DeflateDecoder::Decoder, DeflateDecoder::Decoder> DeflateDecoder::generateDynamicTrees(
        const Decoder& decoder, uint32_t literals, uint32_t distances, BitIter& iterator) const
    {
        std::vector<uint16_t> bitLengths, distanceLength;
        bitLengths.reserve(MaxAlphabetLength);
        std::size_t repeat = 0;
        while (bitLengths.size() < literals + distances) {
            uint16_t token = decoder.decodeToken(iterator);
            if (token < 16)
                repeat = 1;
            else if (token == 16) {
                repeat = 3 +  readNBits<uint8_t>(2, iterator);
                token = bitLengths.back();
            } else if (token == 17) {
                repeat = 3 + readNBits<uint8_t>(3, iterator);
                token = 0;
            } else if (token == 18) {
                repeat = 11 + readNBits<uint8_t>(7, iterator);
                token = 0;
            }
            while (repeat--)
                bitLengths.push_back(token);
        }
        bitLengths.resize(literals + 32, 0);
        std::copy(bitLengths.begin() + literals, bitLengths.begin() + literals + 32, std::back_inserter(distanceLength));
        bitLengths.resize(MaxAlphabetLength);
        std::fill(bitLengths.begin() + literals, bitLengths.end(), 0);
        return {HuffmanTree<uint16_t>{bitLengths}, HuffmanTree<uint16_t>{distanceLength}};
    }

    void DeflateDecoder::dynamicBlockLoop(const Decoder& mainDecoder, const Decoder& distanceDecoder, BitIter& iterator) {
        auto token = mainDecoder.decodeToken(iterator);
        for (;token != BlockEnd; token = mainDecoder.decodeToken(iterator)) {
            if (token < BlockEnd)
                outputStream.push_back(token);
            else
                decompressDynamicDistance(token - 257, iterator, distanceDecoder);
        }
    }

    void DeflateDecoder::decompressDynamicBlock(BitIter& iterator) {
        uint16_t literals = 257 + readNBits<uint16_t>(5, iterator);
        uint8_t distances = 1 + readNBits<uint8_t>(5, iterator);
        uint8_t codeLength = 4 + readNBits<uint8_t>(4, iterator);
        std::array<uint16_t, 19> codes{};
        for (uint16_t i = 0;i < codeLength; ++i)
            codes[dynamicCodesOrder[i]] = readNBits<uint8_t>(3, iterator);
        HuffmanTree<uint16_t>::Decoder mainDecoder{HuffmanTree<uint16_t>{codes}};
        auto [treeDecoder, distanceDecoder] = generateDynamicTrees(mainDecoder, literals, distances, iterator);
        dynamicBlockLoop(treeDecoder, distanceDecoder, iterator);
    }

    void DeflateDecoder::decompressDynamicDistance(uint16_t token, BitIter& iterator, const HuffmanTree<uint16_t>::Decoder& distanceDecoder) {
        auto [addbits, addLength] = extraLength.at(token);
        uint32_t length = addLength + readNBits<uint32_t>(addbits, iterator);
        uint32_t distanceToken = distanceDecoder.decodeToken(iterator);
        auto [distBits, distLength] = distances.at(distanceToken);
        uint32_t distance = distLength + readNBits<uint32_t>(distBits, iterator);
        std::size_t offset = outputStream.size() - distance;
        for (auto i : std::views::iota(0u, length))
            outputStream.push_back(outputStream.at(offset + i));
    }

    void DeflateDecoder::copyNotCompressed(BitIter& iterator) {
        iterator.skipToNextByte();
        uint16_t length = readType<uint16_t, true>(iterator);
        uint16_t complement = readType<uint16_t, true>(iterator);
        if (length != 0xFFFF - complement)
            throw DeflateDecoderDataCorruptionException{};
        outputStream.reserve(outputStream.size() + length);
        for ([[maybe_unused]] auto i : std::views::iota(uint16_t(0), length))
            outputStream.push_back(static_cast<char>(iterator.readByte()));
    }

    void DeflateDecoder::saveAdler32Code(void) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&adler32Code);
        for (uint8_t* end = ptr + 4; ptr != end; ++ptr) {
            *ptr = rawData.back();
            rawData.pop_back();
        }
    }

    uint32_t DeflateDecoder::calculateAlder32(void) const noexcept {
        uint32_t s1 = 1, s2 = 0;
        for (const auto& value : outputStream) {
            s1 = (s1 + value) % DeflateDecoder::AdlerBase;
            s2 = (s1 + s2) % DeflateDecoder::AdlerBase;
        }
        return (s2 << 16) + s1;
    }

    bool DeflateDecoder::diagnostics = false;

}

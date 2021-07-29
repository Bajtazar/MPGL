#include "DeflateDecoder.hpp"

#include "../Exceptions/NotSupportedException.hpp"
#include "../Exceptions/DeflateDecoderInvalidHeaderException.hpp"
#include "../Exceptions/DeflateDecoderDataCorruptionException.hpp"

#include <bitset>
#include <ranges>
#include <iostream>
#include <algorithm>

namespace ge {

    HuffmanTree<uint16_t>::Decoder DeflateDecoder::fixedCodeDecoder{};

    DeflateDecoder::DeflateDecoder(std::deque<char>& rawData) : rawData{rawData} {
        parseHeader();
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

    void DeflateDecoder::decompress(void) {
        BitIter iterator {rawData.begin()};
        while (readBlock(iterator)) {

        }
    }

    bool DeflateDecoder::readBlock(BitIter& iterator) {
        bool isNotFinal = *iterator++;   // ? inverse bit sequence ?
        if (*iterator++) {
            if (*iterator++)
            { std::cout << "Throw\n"; return false; }
            //    return true; // throw error
            else
            { std::cout << "Dynamic\n"; return false; }
            //    return true; // dynamic huffman codes
        } else {
            if (*iterator++)
            { std::cout << "Fixed\n"; return false; }
            //    return true; // fixed huffman codes
            else
                copyNotCompressed(iterator);
        }
        return isNotFinal;
    }

    void DeflateDecoder::decompressBlock(BitIter& iterator) {
        auto token = fixedCodeDecoder.decodeToken(iterator);
        for (;token != BlockEnd; token = fixedCodeDecoder.decodeToken(iterator)) {
            if (token < BlockEnd) {
                outputStream.push_back(token);
                continue;
            }

        }
    }

    void DeflateDecoder::copyNotCompressed(BitIter& iterator) {
        iterator.skipToNextByte();
        uint16_t length = iterator.readType<uint16_t, true>();
        uint16_t complement = iterator.readType<uint16_t, true>();
        if (length != 0xFFFF - complement)
            throw DeflateDecoderDataCorruptionException{};
        outputStream.reserve(outputStream.size() + length);
        for ([[maybe_unused]] auto i : std::views::iota(uint16_t(0), length))
            outputStream.push_back(iterator.readByte());
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

}

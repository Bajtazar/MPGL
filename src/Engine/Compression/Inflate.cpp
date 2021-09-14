#include "Inflate.hpp"

#include "../Exceptions/NotSupportedException.hpp"
#include "../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../Exceptions/InflateInvalidHeaderException.hpp"
#include "../Exceptions/InflateDataCorruptionException.hpp"

#include <bitset>
#include <ranges>
#include <algorithm>

namespace ge {

    template <security::SecurityPolicy Policy>
    HuffmanTree<uint16_t>::Decoder Inflate<Policy>::fixedCodeDecoder{};

    template <security::SecurityPolicy Policy>
    Inflate<Policy>::Inflate([[maybe_unused]] Policy policy, std::deque<char>& rawData)
        : rawData{rawData}
    {
        if (rawData.size() < 6)
            throw InflateDataCorruptionException{};
        try {
            parseHeader();
        } catch (HuffmanTreeException const&) {
            if (diagnostics)
                throw;
            else
                throw InflateDataCorruptionException{};
        }
        if (rawData.size() < 4)
            throw InflateDataCorruptionException{};
        saveAdler32Code();
    }

    template <security::SecurityPolicy Policy>
    Inflate<Policy>::Inflate(std::deque<char>& rawData)
        : Inflate{Policy{}, rawData} {}

    template <security::SecurityPolicy Policy>
    Inflate<Policy>::CompressionLevel Inflate<Policy>::getCompressionLevel(void) const noexcept {
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

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::parseHeader(void) {
        uint8_t cmf = rawData[0], flg = rawData[1];
        rawData.erase(rawData.begin(), rawData.begin() + 2);
        if (cmf != 0x78)    // @TODO add later more indepth info
            throw InflateInvalidHeaderException{};
        if (((256u * cmf + flg) % 31))
            throw InflateDataCorruptionException{};
        std::bitset<8> bits{flg};
        if (bits[5]) // isDict
            throw NotSupportedException{"No-default dicts are not supported."};
        compressionMethod = bits[6] + 2 * bits[7];
    }

    template <security::SecurityPolicy Policy>
    Inflate<Policy>::SafeIter Inflate<Policy>::getIterator(void) {
        if constexpr (security::isSecurePolicy<Policy>)
            return SafeIter{rawData.begin(), rawData.end()};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return rawData.begin();
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    std::vector<char>& Inflate<Policy>::decompress(void) {
        BitIter iterator{getIterator()};
        while (readBlock(iterator));
        // adler32
        return outputStream;
    }

    template <security::SecurityPolicy Policy>
    bool Inflate<Policy>::readBlock(BitIter& iterator) {
        bool isFinal = *iterator++;
        if (*iterator++) {
            if (*iterator++)
                throw InflateDataCorruptionException{};
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

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::decompressFixedBlock(BitIter& iterator) {
        auto token = fixedCodeDecoder.decodeToken(iterator);
        for (;token != BlockEnd; token = fixedCodeDecoder.decodeToken(iterator)) {
            if (token < BlockEnd)
                outputStream.push_back(token);
            else
                decompressFixedDistance(token - 257, iterator);
        }
    }

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::decompressFixedDistance(uint16_t token, BitIter& iterator) {
        auto [lenBits, length] = extraLength.at(token);
        length += readNBits<uint16_t>(lenBits, iterator);
        uint16_t distanceToken = readRNBits<uint8_t>(5, iterator);
        auto [distBits, distance] = distances.at(distanceToken);
        distance += readNBits<uint32_t>(distBits, iterator);
        uint32_t offset = outputStream.size() - distance;
        for (auto i : std::views::iota(0u, length))
            outputStream.push_back(outputStream.at(offset + i));
    }

    template <security::SecurityPolicy Policy>
    uint16_t Inflate<Policy>::readCodeLengthAlphabet(std::size_t& repeater,
        BitIter& iter, std::vector<uint16_t>& bitLengths, uint16_t token) const
    {
        switch (token) {
            case 16:
                repeater = 3 +  readNBits<uint8_t>(2, iter);
                return bitLengths.back();
            case 17:
                repeater = 3 + readNBits<uint8_t>(3, iter);
                return 0;
            case 18:
                repeater = 11 + readNBits<uint8_t>(7, iter);
                return 0;
            default:
                repeater = 1;
                return token;
        }
    }

    template <security::SecurityPolicy Policy>
    std::vector<uint16_t> Inflate<Policy>::readBitLengths(const Decoder& decoder,
        uint32_t literals, uint32_t distances, BitIter& iterator) const
    {
        std::vector<uint16_t> bitLengths;
        bitLengths.reserve(MaxAlphabetLength);
        std::size_t repeater = 0;
        while (bitLengths.size() < literals + distances) {
            auto token = readCodeLengthAlphabet(repeater, iterator, bitLengths, decoder.decodeToken(iterator));
            while (repeater--)
                bitLengths.push_back(token);
        }
        bitLengths.resize(literals + 32, 0);
        return bitLengths;
    }

    template <security::SecurityPolicy Policy>
    std::pair<typename Inflate<Policy>::Decoder, typename Inflate<Policy>::Decoder>
        Inflate<Policy>::generateDynamicTrees(const Decoder& decoder, uint32_t literals,
            uint32_t distances, BitIter& iterator) const
    {
        std::vector<uint16_t> distanceLength, bitLengths = readBitLengths(decoder, literals, distances, iterator);
        std::copy(bitLengths.begin() + literals, bitLengths.begin() + literals + 32, std::back_inserter(distanceLength));
        bitLengths.resize(MaxAlphabetLength);
        std::fill(bitLengths.begin() + literals, bitLengths.end(), 0);
        return {HuffmanTree<uint16_t>{bitLengths}, HuffmanTree<uint16_t>{distanceLength}};
    }

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::dynamicBlockLoop(const Decoder& mainDecoder, const Decoder& distanceDecoder, BitIter& iterator) {
        auto token = mainDecoder.decodeToken(iterator);
        for (;token != BlockEnd; token = mainDecoder.decodeToken(iterator)) {
            if (token < BlockEnd)
                outputStream.push_back(token);
            else
                decompressDynamicDistance(token - 257, iterator, distanceDecoder);
        }
    }

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::decompressDynamicBlock(BitIter& iterator) {
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

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::decompressDynamicDistance(uint16_t token, BitIter& iterator, const HuffmanTree<uint16_t>::Decoder& distanceDecoder) {
        auto [addbits, addLength] = extraLength.at(token);
        uint32_t length = addLength + readNBits<uint32_t>(addbits, iterator);
        uint32_t distanceToken = distanceDecoder.decodeToken(iterator);
        auto [distBits, distLength] = distances.at(distanceToken);
        uint32_t distance = distLength + readNBits<uint32_t>(distBits, iterator);
        std::size_t offset = outputStream.size() - distance;
        for (auto i : std::views::iota(0u, length))
            outputStream.push_back(outputStream.at(offset + i));
    }

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::copyNotCompressed(BitIter& iterator) {
        iterator.skipToNextByte();
        uint16_t length = readType<uint16_t, true>(iterator);
        uint16_t complement = readType<uint16_t, true>(iterator);
        if (length != 0xFFFF - complement)
            throw InflateDataCorruptionException{};
        outputStream.reserve(outputStream.size() + length);
        for ([[maybe_unused]] auto i : std::views::iota(uint16_t(0), length))
            outputStream.push_back(static_cast<char>(iterator.readByte()));
    }

    template <security::SecurityPolicy Policy>
    void Inflate<Policy>::saveAdler32Code(void) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&adler32Code);
        for (uint8_t* end = ptr + 4; ptr != end; ++ptr) {
            *ptr = rawData.back();
            rawData.pop_back();
        }
    }

    template <security::SecurityPolicy Policy>
    uint32_t Inflate<Policy>::calculateAlder32(void) const noexcept {
        uint32_t s1 = 1, s2 = 0;
        for (const auto& value : outputStream) {
            s1 = (s1 + value) % Inflate::AdlerBase;
            s2 = (s1 + s2) % Inflate::AdlerBase;
        }
        return (s2 << 16) + s1;
    }

    template <security::SecurityPolicy Policy>
    bool Inflate<Policy>::diagnostics = false;

}

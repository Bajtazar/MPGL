/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

namespace mpgl {

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    HuffmanTree<uint16>::Decoder
        Inflate<Range, Policy>::fixedCodeDecoder{};

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    Inflate<Range, Policy>::Inflate(
        Range&& range,
        [[maybe_unused]] Policy policy)
            : range{std::forward<Range>(range)} {}

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    Inflate<Range, Policy>::Iterator
        Inflate<Range, Policy>::getIterator(void)
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iterator{range.begin(), range.end() - 4};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return range.begin();
        else
            throw SecurityUnknownPolicyException{};
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] Range Inflate<Range, Policy>::operator() (
        void)
    {
        BitIter iterator{getIterator()};
        Range decompressed;
        while (readBlock(iterator, decompressed));
        return decompressed;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] bool Inflate<Range, Policy>::readBlock(
        BitIter& iterator,
        Range& decompressed) const
    {
        bool isFinal = *iterator++;
        if (*iterator++) {
            if (*iterator++)
                throw InflateDataCorruptionException{};
            else
                decompressFixedBlock(iterator, decompressed);
        } else {
            if (*iterator++)
                decompressDynamicBlock(iterator, decompressed);
            else
                copyNotCompressed(iterator, decompressed);
        }
        return !isFinal;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::decompressFixedBlock(
        BitIter& iterator,
        Range& decompressed) const
    {
        auto token = fixedCodeDecoder(iterator);
        for (;token != BlockEnd; token = fixedCodeDecoder(iterator)) {
            if (token < BlockEnd)
                decompressed.push_back(token);
            else
                decompressFixedDistance(token - 257, iterator,
                    decompressed);
        }
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::decompressFixedDistance(
        uint16 token,
        BitIter& iterator,
        Range& decompressed) const
    {
        auto [lenBits, length] = extraLength.at(token);
        length += readNBits<uint16>(lenBits, iterator);
        uint16 distanceToken = readRNBits<uint8>(5, iterator);
        auto [distBits, distance] = distances.at(distanceToken);
        distance += readNBits<uint32>(distBits, iterator);
        uint32 offset = decompressed.size() - distance;
        for (uint32 i = 0; i < length; ++i)
            decompressed.push_back(decompressed.at(offset + i));
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::decompressDynamicBlock(
        BitIter& iterator,
        Range& decompressed) const
    {
        uint16 literals = 257 + readNBits<uint16>(5, iterator);
        uint8 distances = 1 + readNBits<uint8>(5, iterator);
        uint8 codeLength = 4 + readNBits<uint8>(4, iterator);
        std::array<uint16, 19> codes{};
        for (uint16 i = 0;i < codeLength; ++i)
            codes[dynamicCodesOrder[i]] = readNBits<uint8>(3, iterator);
        Decoder mainDecoder{HuffmanTree<uint16>{codes}};
        auto [treeDecoder, distanceDecoder] = generateDynamicTrees(
            mainDecoder, literals, distances, iterator);
        dynamicBlockLoop(treeDecoder, distanceDecoder, iterator,
            decompressed);
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    std::pair<typename Inflate<Range, Policy>::Decoder,
        typename Inflate<Range, Policy>::Decoder>
            Inflate<Range, Policy>::generateDynamicTrees(
                Decoder const& decoder,
                uint32 literals,
                uint32 distances,
                BitIter& iterator) const
    {
        VectorU16 distanceLength, bitLengths = readBitLengths(
            decoder, literals, distances, iterator);
        std::copy(bitLengths.begin() + literals, bitLengths.begin()
            + literals + 32, std::back_inserter(distanceLength));
        bitLengths.resize(MaxAlphabetLength);
        std::fill(bitLengths.begin() + literals, bitLengths.end(), 0);
        return {HuffmanTree<uint16>{bitLengths},
            HuffmanTree<uint16>{distanceLength}};
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    Inflate<Range, Policy>::VectorU16
        Inflate<Range, Policy>::readBitLengths(
            Decoder const& decoder,
            uint32 literals,
            uint32 distances,
            BitIter& iterator) const
    {
        VectorU16 bitLengths;
        bitLengths.reserve(MaxAlphabetLength);
        std::size_t repeater = 0;
        while (bitLengths.size() < literals + distances) {
            auto token = readCodeLength(repeater,
                iterator, bitLengths, decoder(iterator));
            while (repeater--)
                bitLengths.push_back(token);
        }
        bitLengths.resize(literals + 32, 0);
        return bitLengths;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    uint16 Inflate<Range, Policy>::readCodeLength(
        std::size_t& repeater,
        BitIter& iter,
        VectorU16 const& bitLengths,
        uint16 token) const
    {
        switch (token) {
            case 16:
                repeater = 3 + readNBits<uint8>(2, iter);
                return bitLengths.back();
            case 17:
                repeater = 3 + readNBits<uint8>(3, iter);
                return 0;
            case 18:
                repeater = 11 + readNBits<uint8>(7, iter);
                return 0;
            default:
                repeater = 1;
                return token;
        }
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::dynamicBlockLoop(
        Decoder const& mainDecoder,
        Decoder const& distanceDecoder,
        BitIter& iterator,
        Range& decompressed) const
    {
        auto token = mainDecoder(iterator);
        for (;token != BlockEnd; token = mainDecoder(iterator)) {
            if (token < BlockEnd)
                decompressed.push_back(token);
            else
                decompressDynamicDistance(token - 257, iterator,
                    distanceDecoder, decompressed);
        }
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::decompressDynamicDistance(
        uint16 token,
        BitIter& iterator,
        Decoder const& distanceDecoder,
        Range& decompressed) const
    {
        auto [addbits, addLength] = extraLength.at(token);
        uint32 length = addLength + readNBits<uint32>(
            addbits, iterator);
        uint32 distanceToken = distanceDecoder(iterator);
        auto [distBits, distLength] = distances.at(distanceToken);
        uint32 distance = distLength + readNBits<uint32>(
            distBits, iterator);
        std::size_t offset = decompressed.size() - distance;
        for (uint32 i = 0; i < length; ++i)
            decompressed.push_back(decompressed.at(offset + i));
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void Inflate<Range, Policy>::copyNotCompressed(
        BitIter& iterator,
        Range& decompressed) const
    {
        iterator.skipToNextByte();
        uint16 length = readType<uint16, true>(iterator);
        uint16 complement = readType<uint16, true>(iterator);
        if (length != 0xFFFF - complement)
            throw InflateDataCorruptionException{};
        decompressed.reserve(decompressed.size() + length);
        for (uint32 i = 0; i < length; ++i)
            decompressed.push_back(static_cast<char>(iterator.readByte()));
    }

}

/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Exceptions/Inflate/InflateDataCorruptionException.hpp>
#include <MPGL/Exceptions/SecurityUnknownPolicyException.hpp>
#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Compression/HuffmanTree.hpp>
#include <MPGL/Iterators/SafeIterator.hpp>
#include <MPGL/IO/Readers.hpp>

namespace mpgl {

    /**
     * Decompression algorithm for DEFLATE compression standard
     *
     * @tparam Range the compressed data range
     * @tparam Policy the security policy
     */
    template <ByteFlexibleRange Range,
        security::SecurityPolicy Policy = Secured>
    class Inflate {
    public:
        /**
         * Constructs a new Inflate object from the given
         * range universal reference and policy token
         *
         * @param range the universal reference to the decompressed
         * object
         * @param policy the secure policy token
         */
        explicit Inflate(Range&& range, Policy policy = {});

        /**
         * Decompresses the given range and returns the range
         * containing decompressed data
         *
         * @throw InflateDataCorruptionException when headers
         * are corrupted
         * @return the decompressed data
         */
        [[nodiscard]] Range operator()(void);
    private:
        typedef PolicyIterRT<Policy, Range>             Iterator;
        typedef LittleEndianInputBitIter<Iterator>      BitIter;
        typedef HuffmanTree<uint16>::Decoder            Decoder;
        typedef std::vector<uint16>                     VectorU16;

        /**
         * Returns an iterator to the compressed range
         *
         * @return the iterator to the compressed range
         */
        Iterator getIterator(void);

        /**
         * Decompresses a block of compressed data
         *
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         * @return if the block is not the final one
         */
        bool readBlock(BitIter& iterator, Range& decompressed) const;

        /**
         * Decompresses the fixed block
         *
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         */
        void decompressFixedBlock(
            BitIter& iterator,
            Range& decompressed) const;

        /**
         * Decompresses the element of fixed block containing data
         * about previous occurence of decompressed data
         *
         * @param token the distance token
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         */
        void decompressFixedDistance(
            uint16 token,
            BitIter& iterator,
            Range& decompressed) const;

        /**
         * Decompresses the dynamic block
         *
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         */
        void decompressDynamicBlock(
            BitIter& iterator,
            Range& decompressed) const;

        /**
         * Generates the dynamic block huffman trees and decoders
         *
         * @param decoder the constant reference to the main decoder
         * @param literals the number of literals
         * @param distances the number of distances
         * @param iterator the reference to the bit iterator
         * @return the pair containing normal and distance decoder
         */
        std::pair<Decoder, Decoder> generateDynamicTrees(
            Decoder const& decoder,
            uint32 literals,
            uint32 distances,
            BitIter& iterator) const;

        /**
         * Parses the bit lengths of tokens in the dynamic
         * huffman trees
         *
         * @param decoder the constant reference to the main decoder
         * @param literals the number of literals
         * @param distances the number of distances
         * @param iterator the reference to the bit iterator
         * @return the vector containing bit lengths of tokens
         */
        VectorU16 readBitLengths(
            Decoder const& decoder,
            uint32 literals,
            uint32 distances,
            BitIter& iterator) const;

        /**
         * Parses the length of the code in the dynamic huffman tree
         *
         * @param repeater the reference to the counter which
         * copies given token the given number of times
         * @param iterator the reference to the bit iterator
         * @param bitLengths the constant reference to the bit lengths
         * vector
         * @param token the token
         * @return the new token
         */
        uint16 readCodeLength(
            std::size_t& repeater,
            BitIter& iterator,
            VectorU16 const& bitLengths,
            uint16 token) const;

        /**
         * Decompresses the dynamic block in loop
         *
         * @param mainDecoder the constant reference to the main
         * decoder
         * @param distanceDecoder the constant reference to the
         * distance decoder
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         */
        void dynamicBlockLoop(
            Decoder const& mainDecoder,
            Decoder const& distanceDecoder,
            BitIter& iterator,
            Range& decompressed) const;

        /**
         * Decompresses the element of the dynamic block containing
         * data about previous occurence of decompressed data
         *
         * @param token the distance token
         * @param iterator the reference to the bit iterator
         * @param distanceDecoder the constant reference to the
         * distance decoder
         * @param decompressed the reference to the decompressed
         * data object
         */
        void decompressDynamicDistance(
            uint16 token,
            BitIter& iterator,
            Decoder const& distanceDecoder,
            Range& decompressed) const;

        /**
         * Copies the uncompressed data block to the output range
         *
         * @param iterator the reference to the bit iterator
         * @param decompressed the reference to the decompressed
         * data object
         */
        void copyNotCompressed(
            BitIter& iterator,
            Range& decompressed) const;

        Range                                           range;

        /// The fixed block decoder
        static Decoder                                  fixedCodeDecoder;
        static constexpr const uint16                   MaxAlphabetLength
            = 288;
        static constexpr const uint16                   BlockEnd
            = 256;
        /// Extra distance bits lengths
        static constexpr const std::array<
            std::pair<uint8, uint16>, 29>               extraLength =
        {
            std::pair<uint8, uint8>{0, 3}, {0, 4}, {0, 5}, {0, 6},
            {0, 7}, {0, 8}, {0, 9}, {0, 10}, {1, 11}, {1, 13}, {1, 15},
            {1, 17}, {2, 19}, {2, 23}, {2, 27}, {2, 31}, {3, 35},
            {3, 43}, {3, 51}, {3, 59}, {4, 67}, {4, 83}, {4, 99},
            {4, 115}, {5, 131}, {5, 163}, {5, 195}, {5, 227}, {0, 258}
        };
        /// Extra distance bits distances
        static constexpr const std::array<
            std::pair<uint8, uint32>, 30>               distances =
        {
            std::pair<uint8, uint32>{0, 1}, {0, 2}, {0, 3}, {0, 4},
            {1, 5}, {1, 7}, {2, 9}, {2, 13}, {3, 17}, {3, 25}, {4, 33},
            {4, 49}, {5, 65}, {5, 97}, {6, 129}, {6, 193}, {7, 257},
            {7, 385}, {8, 513}, {8, 769}, {9, 1025}, {9, 1537},
            {10, 2049}, {10, 3073}, {11, 4097}, {11, 6145}, {12, 8193},
            {12, 12289}, {13, 16385}, {13, 24577}
        };
        /// The order of the dynamic codes
        static constexpr const std::array<uint8, 19>    dynamicCodesOrder =
        {
            16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2,
            14, 1, 15
        };
    };

}

#include <MPGL/Compression/Inflate.tpp>

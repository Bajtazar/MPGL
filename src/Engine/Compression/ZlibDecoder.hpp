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

#include "../Exceptions/InflateInvalidHeaderException.hpp"
#include "../Exceptions/NotSupportedException.hpp"
#include "Checksums/Adler32.hpp"
#include "Inflate.hpp"

#include <bitset>

namespace mpgl {

    /**
     * Parses the zlib header and decompresses range using
     * DEFLATE decompressor
     *
     * @tparam Range the compressed data range
     * @tparam Policy the security policy
     */
    template <ByteFlexibleRange Range,
        security::SecurityPolicy Policy = Secured>
    class ZlibDecoder {
    public:
        /**
         * Compression level of the given compressed file
         */
        enum class CompressionLevel : uint8 {
            /// The fastest compression
            Fastest,
            /// Fast compression
            Fast,
            /// Default compression
            Default,
            /// Maximum [the most effective] compression
            Maximum
        };

        /**
         * Constructs a new Zlib Decoder object. Parses
         * zlib header
         *
         * @throw InflateInvalidHeaderException when header magic
         * number is invalid
         * @throw InflateDataCorruptionException when header
         * checksum is invalid
         * @throw NotSupportedException when the given DEFLATE
         * block uses dictionaries
         * @param range the universal reference to the decompressed
         * object
         * @param policy the secure policy token
         */
        explicit ZlibDecoder(Range&& range, Policy policy = {});

        /**
         * Returns a compression level of the block
         *
         * @return the compression level of the block
         */
        [[nodiscard]] CompressionLevel
            getCompressionLevel(void) const noexcept;

        /**
         * Decompresses the given range and returns the range
         * containing decompressed data
         *
         * @throw InflateDataCorruptionException when data is
         * corrupted
         * @return the decompressed data
         */
        [[nodiscard]] Range operator()(void);
    private:
        typedef std::conditional_t<
            security::isSecurePolicy<Policy>,
            SafeIterator<typename Range::iterator>,
            typename Range::iterator>                   Iterator;

        Range                                           range;
        Iterator                                        rangeIterator;
        uint8                                           compressionMethod;
        [[no_unique_address]] Policy                    policy;

        /**
         * Returns an iterator to the compressed range
         *
         * @return the iterator to the compressed range
         */
        Iterator getIterator(void);

        /**
         * Parses DEFLATE block header
         */
        void parseHeader(void);

        /**
         * Returns a checksum saved at the end of the data
         *
         * @return the checksum
         */
        uint32 getChecksum(void) const;
    };

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    ZlibDecoder<Range, Policy>::Iterator
        ZlibDecoder<Range, Policy>::getIterator(void)
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iterator{range.begin(), range.end() - 4};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return range.begin();
        else
            throw SecurityUnknownPolicyException{};
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    ZlibDecoder<Range, Policy>::ZlibDecoder(
        Range&& range,
        Policy policy)
            : range{std::forward<Range>(range)}, policy{policy}
    {
        if (this->range.size() < 6)
            throw InflateInvalidHeaderException{};
        rangeIterator = getIterator();
        parseHeader();
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] ZlibDecoder<Range, Policy>::CompressionLevel
        ZlibDecoder<Range, Policy>::getCompressionLevel(
            void) const noexcept
    {
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

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void ZlibDecoder<Range, Policy>::parseHeader(void) {
        uint8 cmf = *rangeIterator++;
        uint8 flg = *rangeIterator++;
        if (cmf != 0x78)
            throw InflateInvalidHeaderException{};
        if (((256u * cmf + flg) % 31))
            throw InflateDataCorruptionException{};
        std::bitset<8> bits{flg};
        if (bits[5]) // isDict
            throw NotSupportedException{
                "No-default dicts are not supported."};
        compressionMethod = bits[6] + 2 * bits[7];
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    uint32 ZlibDecoder<Range, Policy>::getChecksum(void) const {
        return peekType<uint32, true>(range.end() - 4);
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] Range ZlibDecoder<Range, Policy>::operator() (
        void)
    {
        auto distance = rangeIterator - getIterator();
        range.erase(range.begin(), range.begin() + distance);
        uint32 checksum = getChecksum();
        auto decompressed = Inflate{std::move(range), policy}();
        if (adler32(decompressed) != checksum)
            throw InflateDataCorruptionException{};
        return decompressed;
    }

}

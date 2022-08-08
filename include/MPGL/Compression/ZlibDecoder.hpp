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

#include <MPGL/Exceptions/Inflate/InflateInvalidHeaderException.hpp>
#include <MPGL/Exceptions/NotSupportedException.hpp>
#include <MPGL/Compression/Checksums/Adler32.hpp>
#include <MPGL/Compression/Inflate.hpp>

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
        typedef PolicyIterRT<Policy, Range>             Iterator;

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

}

#include <MPGL/Compression/ZlibDecoder.tpp>

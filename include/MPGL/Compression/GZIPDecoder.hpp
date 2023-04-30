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

#include <MPGL/Exceptions/Inflate/InflateInvalidHeaderException.hpp>
#include <MPGL/Exceptions/NotSupportedException.hpp>
#include <MPGL/Compression/Checksums/CRC32.hpp>
#include <MPGL/Compression/Inflate.hpp>
#include <MPGL/IO/Readers.hpp>

namespace mpgl {

    /**
     * Parses the gzip header and decompresses range using
     * DEFLATE decompressor
     *
     * @tparam Range the compressed data range
     * @tparam Policy the security policy
     */
    template <
        ByteFlexibleRange Range,
        security::SecurityPolicy Policy = Secured>
    class GZIPDecoder {
    public:
        typedef std::optional<std::string>              OptString;

        /**
         * Compression level of the given compressed file
         */
        enum class CompressionLevel : uint8 {
            Maximum,
            Fastest
        };

        /**
         * Operating system on which file was compressed
         */
        enum class OperatingSystem : uint8 {
            Fat,
            Amiga,
            VMS,
            Unix,
            VM,
            Atari,
            HPFS,
            Macintosh,
            ZSystem,
            CP,
            TOPS20,
            NTFS,
            QDOS,
            AcornRISCOS,
            Unknown
        };

        /**
         * Constructs a new GZIP Decoder object. Parses
         * gzip header
         *
         * @throw InflateInvalidHeaderException when header magic
         * number is invalid
         * @throw InflateDataCorruptionException when header
         * checksum is invalid
         * @throw NotSupportedException when the given DEFLATE
         * block uses other compression than DEFLATE
         * @param range the universal reference to the decompressed
         * object
         * @param policy the secure policy token
         */
        explicit GZIPDecoder(Range&& range, Policy policy = {});

        /**
         * Returns a compression level of the data
         *
         * @return the compression level of the data
         */
        [[nodiscard]] CompressionLevel
            getCompressionLevel(void) const noexcept;

        /**
         * Returns last modification time of the data
         *
         * @return the last modification time of the data
         */
        [[nodiscard]] uint32
            getModificationTime(void) const noexcept
                { return modificationTime; }

        /**
         * Returns the operating system on which the data was
         * compressed
         *
         * @return the operating system on which the data was
         * compressed
         */
        [[nodiscard]] OperatingSystem
            getOperatingSystem(void) const noexcept;

        /**
         * Returns an optional with the oryginal file name.
         * If this data is not included then returns an empty
         * optional
         *
         * @return the optional with oryginal file name
         */
        [[nodiscard]] OptString
            getOryginalName(void) const noexcept
                { return oryginalName; }

        /**
         * Returns an optional with the data's comment.
         * If this data is not included then returns an empty
         * optional
         *
         * @return the optional with oryginal file name
         */
        [[nodiscard]] OptString
            getComment(void) const noexcept
                { return comment; }

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
        OptString                                       oryginalName;
        OptString                                       comment;
        uint32                                          modificationTime;
        uint8                                           compressionLevel;
        uint8                                           operatingSystem;
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
         * Parses the zlib flags
         *
         * @param flags the zlib header flags
         */
        void parseFlags(uint8 flags);

        /**
         * Skips the extra header fields
         */
        void passExtraFields(void);

        /**
         * Obtains the file name from the header
         */
        void getFileName(void);

        /**
         * Obtains the comment from the header
         */
        void getComment(void);

        /**
         * Checks whether the header checksum matches the calculated
         * one
         */
        void checkHeaderChecksum(void);

        /**
         * Returns a checksum of the decompressed data
         *
         * @return the checksum of the decompressed data
         */
        uint32 getChecksum(void);
    };

}

#include <MPGL/Compression/GZIPDecoder.tpp>

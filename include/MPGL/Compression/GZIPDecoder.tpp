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
    GZIPDecoder<Range, Policy>::Iterator
        GZIPDecoder<Range, Policy>::getIterator(void)
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iterator{range.begin(), range.end() - 8};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return range.begin();
        else
            throw SecurityUnknownPolicyException{};
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    GZIPDecoder<Range, Policy>::GZIPDecoder(
        Range&& range,
        Policy policy)
            : range{std::forward<Range>(range)}, policy{policy}
    {
        rangeIterator = getIterator();
        parseHeader();
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::parseHeader(void) {
        if (readType<uint16, true>(rangeIterator) != 0x1F8B)
            throw InflateInvalidHeaderException{};
        if (*rangeIterator++ != 0x08)
            throw NotSupportedException{
                "Not DEFLATE compressed gzip files are not valid"};
        uint8 flags = *rangeIterator++;
        modificationTime = readType<uint32, false>(rangeIterator);
        compressionLevel = *rangeIterator++;
        operatingSystem = *rangeIterator++;
        parseFlags(flags);
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::parseFlags(uint8 flags) {
        if (flags & 0x04)
            passExtraFields();
        if (flags & 0x08)
            getFileName();
        if (flags & 0x10)
            getComment();
        if (flags & 0x02)
            checkHeaderChecksum();
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::passExtraFields(void) {
        uint16 length = readType<uint16, false>(rangeIterator);
        std::advance(rangeIterator, length);
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::getFileName(void) {
        std::string fileName;
        char symbol = *rangeIterator++;
        for (; symbol; symbol = *rangeIterator++)
            fileName.push_back(symbol);
        oryginalName = fileName;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::getComment(void) {
        std::string comment;
        char symbol = *rangeIterator++;
        for (; symbol; symbol = *rangeIterator++)
            comment.push_back(symbol);
        this->comment = comment;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    void GZIPDecoder<Range, Policy>::checkHeaderChecksum(void) {
        auto headerRange
            = std::ranges::subrange{getIterator(), rangeIterator};
        uint16 crc = readType<uint16, false>(rangeIterator);
        if (crc != (crc32(headerRange) & 0x0000FFFF))
            throw InflateDataCorruptionException{};
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    uint32 GZIPDecoder<Range, Policy>::getChecksum(void) {
        return peekType<uint32, false>(range.end() - 8);
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] Range
        GZIPDecoder<Range, Policy>::operator()(void)
    {
        auto distance = rangeIterator - getIterator();
        range.erase(range.begin(), range.begin() + distance);
        uint32 checksum = getChecksum();
        auto decompressed = Inflate{std::move(range), policy}();
        if (checksum != crc32(decompressed))
            throw InflateDataCorruptionException{};
        return decompressed;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] GZIPDecoder<Range, Policy>::CompressionLevel
        GZIPDecoder<Range, Policy>::getCompressionLevel(
            void) const noexcept
    {
        return compressionLevel == 2 ? CompressionLevel::Maximum
            : CompressionLevel::Fastest;
    }

    template <ByteFlexibleRange Range, security::SecurityPolicy Policy>
    [[nodiscard]] GZIPDecoder<Range, Policy>::OperatingSystem
        GZIPDecoder<Range, Policy>::getOperatingSystem(
            void) const noexcept
    {
        switch(operatingSystem) {
            case 0:
                return OperatingSystem::Fat;
            case 1:
                return OperatingSystem::Amiga;
            case 2:
                return OperatingSystem::VMS;
            case 3:
                return OperatingSystem::Unix;
            case 4:
                return OperatingSystem::VM;
            case 5:
                return OperatingSystem::Atari;
            case 6:
                return OperatingSystem::HPFS;
            case 7:
                return OperatingSystem::Macintosh;
            case 8:
                return OperatingSystem::ZSystem;
            case 9:
                return OperatingSystem::CP;
            case 10:
                return OperatingSystem::TOPS20;
            case 11:
                return OperatingSystem::NTFS;
            case 12:
                return OperatingSystem::QDOS;
            case 13:
                return OperatingSystem::AcornRISCOS;
            default:
                return OperatingSystem::Unknown;
        }
        return OperatingSystem::Uknown;
    }

}

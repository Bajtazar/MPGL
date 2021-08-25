#include "TTFLoader.hpp"

#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../ImageLoading/LoadingHelpers.hpp"
#include "UTF-8.hpp"

#include <iostream>
#include <fstream>
#include <ranges>

namespace ge {

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TTFLoader(std::string const& fileName)
        : TTFLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TTFLoader(Policy policy, std::string const& fileName)
    {
        if (auto stream = FileIO::readFile(fileName))
            buffer = stream->str();
        else {} // throw
        parseFile(getIterator(buffer.begin()));
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Iter TTFLoader<Policy>::getIterator(BuffIter iter) const {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iter{iter, buffer.end()};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return iter;
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::parseFile(Iter iter) {
        parseHead(iter);
        readTable(headTable, tables["head"]);
        readTable(maxpTable, tables["maxp"]);
        readTable(hheaTable, tables["hhea"]);
        loadHmtx();
        loadLoca();
        loadGlyf();
        loadCmap();
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadCmap(void) {
        auto iter = getIterator(buffer.begin() + tables["cmap"].offset);
        cmapTable = Cmap{iter};
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadGlyf(void) {
        if (std::holds_alternative<Loca16>(locaTable)) {
            auto& table = std::get<Loca16>(locaTable);
            for (uint16_t const& index : std::ranges::subrange{table.begin(), table.end()-1})
                glyfTable.emplace_back(getIterator(buffer.begin() + index + tables["glyf"].offset));
        } else {
            auto& table = std::get<Loca32>(locaTable);
            for (uint32_t const& index : std::ranges::subrange{table.begin(), table.end()-1})
                glyfTable.emplace_back(getIterator(buffer.begin() + 2 * index + tables["glyf"].offset));
        }
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::GlyfTable::GlyfTable(Iter iter) {
        int16_t* ptr = &numberOfContours;
        for (uint8_t i = 0;i != 5; ++i, ++ptr)
            *ptr = readType<int16_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadLoca(void) {
        auto iter = getIterator(buffer.begin() + tables["loca"].offset);
        if (headTable.indexFormat) { // 16
            Loca16 loca;
            for (uint16_t i = 0; i < maxpTable.numGlyphs + 1; ++i)
                loca.push_back(readType<uint16_t, true>(iter));
            locaTable = std::move(loca);
        } else { // 32
            Loca32 loca;
            for (uint16_t i = 0; i < maxpTable.numGlyphs + 1; ++i)
                loca.push_back(readType<uint32_t, true>(iter));
            locaTable = std::move(loca);
        }
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::readTable(TableInterface& table, TableDirectory const& config) {
        // checksum check
        table.readTable(getIterator(buffer.begin() + config.offset));
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::parseHead(Iter& iter) {
        ignoreNBytes(4, iter);
        auto tablesRange = std::views::iota(uint16_t(0), readType<uint16_t, true>(iter));
        ignoreNBytes(6, iter);
        auto range = tablesRange | std::views::transform([&iter](auto const& i){ return readNChars(4, iter); });
        std::ranges::for_each(range, [this, &iter](auto const& tag) { tables[tag] = TableDirectory{iter}; });
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TableDirectory::TableDirectory(Iter& iter) {
        checksum = readType<uint32_t, true>(iter);
        offset = readType<uint32_t, true>(iter);
        length = readType<uint32_t, true>(iter);    // checksum purposes
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Head::readTable(Iter iter) {
        // unused variables will be skipped
        majorVersion = readType<uint16_t, true>(iter);
        minorVersion = readType<uint16_t, true>(iter);
        fontRevision = readFixed<true>(iter);
        checksumAdjustment = readType<uint32_t, true>(iter);
        magicNumber = readType<uint32_t, true>(iter);   // 0x5F0F3CF5
        flags = readType<uint16_t, true>(iter);
        unitsPerEm = readType<uint16_t, true>(iter);
        created = readType<uint64_t, true>(iter);
        modified = readType<uint64_t, true>(iter);
        xMin = readType<int16_t, true>(iter);
        yMin = readType<int16_t, true>(iter);
        xMax = readType<int16_t, true>(iter);
        yMax = readType<int16_t, true>(iter);
        macStyle = readType<uint16_t, true>(iter);
        lowestRecPPEM = readType<uint16_t, true>(iter);
        fontHint = readType<int16_t, true>(iter);
        indexFormat = readType<int16_t, true>(iter);
        glyphFormat = readType<int16_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Maxp::readTable(Iter iter) {
        version = readFixed<true>(iter);
        uint16_t* ptr = &numGlyphs;
        for (uint8_t i = 0;i != 14; ++i, ++ptr)
            *ptr = readType<uint16_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Hhea::readTable(Iter iter) {
        uint16_t* ptr = &majorVersion;  // valid uint to int conversion
        for (uint8_t i = 0;i != 12; ++i, ++ptr)
            *ptr = readType<uint16_t, true>(iter);
        ignoreNBytes(8, iter);
        metricDataFormat = readType<int16_t, true>(iter);
        numberOfHMetrics = readType<uint16_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadHmtx(void) {
        auto iter = getIterator(buffer.begin() + tables["hmtx"].offset);
        for (uint16_t i = 0;i < hheaTable.numberOfHMetrics; ++i)
            hmtxTable.hMetrics.emplace_back(std::ref(iter));
        for (uint16_t i = 0;i < (maxpTable.numGlyphs - hheaTable.numberOfHMetrics); ++i)
            hmtxTable.leftSideBearings.push_back(readType<int16_t, true>(iter));
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Hmtx::LongHorMatrix::LongHorMatrix(Iter& iter) {
        advanceWidth = readType<uint16_t, true>(iter);
        leftSideBearing = readType<int16_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Cmap::Cmap(Iter& iter) {
        version = readType<uint16_t, true>(iter);
        numTables = readType<uint16_t, true>(iter);
        for (auto i : std::views::iota(uint16_t(0), numTables))
            encodingRecords.emplace_back(iter);
        if (auto offset = readPlatform()) {
            auto format = readType<uint16_t, true>(iter);
            if (format == 4)
                format4Subtable = Format4Subtable{iter};
            else
                std::cout << "Format4 exception\n";
        } else std::cout << "Platfrom exception\n"; // throw an exception
    }

    template <security::SecurityPolicy Policy>
    std::optional<uint32_t> TTFLoader<Policy>::Cmap::readPlatform(void) {
        for (EncodingRecord const& record : encodingRecords) {
            bool windowsPlatform = (record.platformID == 3) &&
                (record.encodingID < 2 || record.encodingID == 10);
            bool unicodePlatform = !record.platformID &&
                record.encodingID < 5;
            if (windowsPlatform || unicodePlatform)
                return {record.subtableOffset};
        }
        return {};
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Cmap::EncodingRecord::EncodingRecord(Iter& iter) {
        platformID = readType<uint16_t, true>(iter);
        encodingID = readType<uint16_t, true>(iter);
        subtableOffset = readType<uint32_t, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Cmap::Format4Subtable::Format4Subtable(Iter& iter) {
        uint16_t* ptr = &length;
        for (uint8_t i = 0;i != 6; ++i, ++ptr)
            *ptr = readType<uint16_t, true>(iter);
        auto reader = [&iter](uint16_t const& _) { return readType<uint16_t, true>(iter); };
        auto limit = std::views::iota(uint16_t(0), uint16_t(segCountx2 >> 1));
        std::ranges::transform(limit, std::back_inserter(endCode), reader);
        ignoreNBytes(2, iter);
        std::ranges::transform(limit, std::back_inserter(startCode), reader);
        std::ranges::transform(limit, std::back_inserter(idDelta), reader);
        rangeOffsets = iter;
        std::ranges::transform(limit, std::back_inserter(idRangeOffsets), reader);
        getGlyphID();
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Cmap::Format4Subtable::getGlyphID(void) {
        for (uint16_t i = 0;i != endCode.size() - 1; ++i) {
            uint16_t glyphID = 0;
            for (uint16_t j = startCode[i]; j != endCode[i]; ++j) {
                if (idRangeOffsets[i]) {
                    auto glyphIndexOffset = rangeOffsets + (j - startCode[i] + i) * 2
                        + idRangeOffsets[i];
                    glyphID = readType<uint16_t, true>(glyphIndexOffset);
                    if (glyphID)
                        glyphID = (glyphID + idDelta[i]) & 0xFFFF;
                } else
                    glyphID = (j + idDelta[i]) & 0xFFFF;
                // j -> key, glyphID -> value;
            }
        }
    }

}

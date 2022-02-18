#include "TTFLoader.hpp"

#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../Exceptions/TTFLoaderFileCorruption.hpp"

#include <fstream>
#include <ranges>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TTFLoader(FileName const& fileName)
        : TTFLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TTFLoader(Policy policy, FileName const& fileName)
        : fileName{fileName}
    {
        if (auto stream = FileIO::readFile(fileName))
            buffer = stream->str();
        else
            throw TTFLoaderFileCorruption{fileName};
        try {
            parseFile(getIterator());
        } catch (std::out_of_range const&) {
            throw TTFLoaderFileCorruption{fileName};
        }
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Iter TTFLoader<Policy>::getIterator(void) const {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iter{buffer.begin(), buffer.end()};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return buffer.begin();
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::parseFile(Iter iter) {
        parseHead(iter);
        loadHead();
        loadMaxp();
        loadHhea();
        loadHmtx();
        loadLoca();
        loadKern();
        loadCmap();
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadMaxp(void) {
        auto iter = getIterator() + tables["maxp"].offset;
        std::advance(iter, 4);
        numGlyphs = readType<uint16, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadHhea(void) {
        auto iter = getIterator() + tables["hhea"].offset;
        std::advance(iter, 48);
        numberOfHMetrics = readType<uint16, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadLoca(void) {
        locaTable = LocaTable{getIterator() + tables["loca"].offset,
            indexFormat, numGlyphs};
    }

    template <security::SecurityPolicy Policy>
    GlyphData TTFLoader<Policy>::createGlyph(uint16 index) {
        auto iter = getIterator() + tables["glyf"].offset;
        auto glyphOffset = locaTable(index);
        auto nextGlyph = locaTable(index + 1);
        if (glyphOffset == nextGlyph)
            return GlyphData{VectorizedGlyph{},
                metrics.at(index).advanceWidth,
                metrics.at(index).leftSideBearing};
        return GlyphData{VectorizedGlyph{iter, glyphOffset, locaTable},
            metrics.at(index).advanceWidth,
            metrics.at(index).leftSideBearing};
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadKern(void) {
        auto iter = getIterator() + tables["kern"].offset;
        std::advance(iter, 2);
        auto size = readType<uint16, true>(iter);
        kernTable.reserve(size);
        for (auto _ : std::views::iota(uint16(0), size))
            kernTable.emplace_back(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::parseHead(Iter& iter) {
        std::advance(iter, 4);
        auto tablesRange = std::views::iota(uint16(0), readType<uint16, true>(iter));
        std::advance(iter, 6);
        auto range = tablesRange | std::views::transform([&iter](auto const& i){ return readNChars(4, iter); });
        std::ranges::for_each(range, [this, &iter](auto const& tag) { tables[tag] = TableDirectory{iter}; });
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::TableDirectory::TableDirectory(Iter& iter) {
        checksum = readType<uint32, true>(iter);
        offset = readType<uint32, true>(iter);
        length = readType<uint32, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadHead(void) {
        auto iter = getIterator() + tables["head"].offset;
        std::advance(iter, 12);
        if (readType<uint32, true>(iter) != 0x5F0F3CF5)
            throw TTFLoaderFileCorruption{fileName};
        std::advance(iter, 2);
        fontData.unitsPerEm = readType<uint16, true>(iter);
        std::advance(iter, 16);
        fontData.xMin = readType<int16, true>(iter);
        fontData.yMin = readType<int16, true>(iter);
        fontData.xMax = readType<int16, true>(iter);
        fontData.yMax = readType<int16, true>(iter);
        std::advance(iter, 6);
        indexFormat = readType<int16, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadHmtx(void) {
        auto iter = getIterator() + tables["hmtx"].offset;
        metrics.reserve(numGlyphs);
        for (uint16 i = 0; i != numberOfHMetrics; ++i)
            metrics.emplace_back(iter);
        auto advanceWidth = metrics.size() ? metrics.back().advanceWidth : 0;
        for (uint16 i = 0; i != (numGlyphs - numberOfHMetrics); ++i)
            metrics.emplace_back(advanceWidth,
                readType<int16, true>(iter));
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::LongHorMatrix::LongHorMatrix(Iter& iter) {
        advanceWidth = readType<uint16, true>(iter);
        leftSideBearing = readType<int16, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::LongHorMatrix::LongHorMatrix(uint16 advanceWidth,
        int16 bearing) noexcept : advanceWidth{advanceWidth}, leftSideBearing{bearing} {}

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadCmap(void) {
        auto iter = getIterator() + tables["cmap"].offset;
        auto const begin = iter;
        if (readType<uint16, true>(iter))
            throw TTFLoaderFileCorruption{fileName};
        loadCmapSubtables(iter, begin);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::loadCmapSubtables(Iter& iter, Iter const& begin) {
        for (auto i : std::views::iota(uint16(0), readType<uint16, true>(iter))) {
            if (auto offset = readPlatform({iter})) {
                auto subtableIter = begin + *offset;
                if (readType<uint16, true>(subtableIter) == 4)
                    Format4Subtable{subtableIter, *this};
                else
                    throw TTFLoaderFileCorruption{fileName};
            }
        }
    }

    template <security::SecurityPolicy Policy>
    std::optional<uint32> TTFLoader<Policy>::readPlatform(EncodingRecord const& record) {
        bool windowsPlatform = (record.platformID == 3) &&
            (record.encodingID < 2 || record.encodingID == 10);
        bool unicodePlatform = !record.platformID &&
            record.encodingID < 5;
        if (windowsPlatform)
            return {record.subtableOffset};
        return {};
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::EncodingRecord::EncodingRecord(Iter& iter) {
        platformID = readType<uint16, true>(iter);
        encodingID = readType<uint16, true>(iter);
        subtableOffset = readType<uint32, true>(iter);
    }

    template <security::SecurityPolicy Policy>
    TTFLoader<Policy>::Format4Subtable::Format4Subtable(Iter& iter, TTFLoader& loader) {
        std::advance(iter, 4);
        auto limit = reserve(readType<uint16, true>(iter) >> 1);
        std::advance(iter, 6);
        auto reader = [&iter](uint16 const& _) { return readType<uint16, true>(iter); };
        std::ranges::transform(limit, std::back_inserter(endCode), reader);
        std::advance(iter, 2);
        std::ranges::transform(limit, std::back_inserter(startCode), reader);
        std::ranges::transform(limit, std::back_inserter(idDelta), reader);
        rangeOffsets = iter;
        std::ranges::transform(limit, std::back_inserter(idRangeOffsets), reader);
        getGlyphID(loader);
    }

    template <security::SecurityPolicy Policy>
    auto TTFLoader<Policy>::Format4Subtable::reserve(uint16 segCount) {
        endCode.reserve(segCount);
        startCode.reserve(segCount);
        idDelta.reserve(segCount);
        idRangeOffsets.reserve(segCount);
        return std::views::iota(uint16(0), segCount);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Format4Subtable::getGlyphID(TTFLoader& loader) {
        for (std::size_t i = 0; i != endCode.size(); ++i)
            for (std::size_t j = startCode[i]; j != endCode[i] + 1; ++j)
                tryReadGlyph(i, j, loader);
    }

    template <security::SecurityPolicy Policy>
    void TTFLoader<Policy>::Format4Subtable::tryReadGlyph(std::size_t const& i,
        std::size_t const& j, TTFLoader& loader)
    {
        uint16 index = 0;
        if (auto&& [iter, empty] = loader.glyphMap.try_emplace(j, GlyphData{}); empty) {
            if (idRangeOffsets[i]) {
                auto glyphOffset = rangeOffsets + (j - startCode[i] + i) * 2
                    + idRangeOffsets[i];
                index = readType<uint16, true>(glyphOffset);
                if (index)
                    index = (index + idDelta[i]) & 0xFFFF;
            } else
                index = (j + idDelta[i]) & 0xFFFF;
            iter->second = std::move(loader.createGlyph(index));
        }
    }

}

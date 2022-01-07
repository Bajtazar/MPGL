#pragma once

#include "../../Iterators/SafeIterator.hpp"
#include "../../Utility/Security.hpp"
#include "../../IO/FileIO.hpp"
#include "FontComponents.hpp"

#include <variant>

namespace ge {

    template <security::SecurityPolicy Policy = Secured>
    class TTFLoader {
    private:
        typedef std::string::const_iterator                 BuffIter;
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<BuffIter>, BuffIter>               Iter;
        typedef std::string                                 FileName;
    public:
        explicit TTFLoader(FileName const& fileName);
        explicit TTFLoader(Policy policy, FileName const& fileName);

        GlyphMap const& getGlyphs(void) const noexcept { return glyphMap; }
        GlyphMap&& getGlyphs(void) noexcept { return std::move(glyphMap); }

        FontData const& getFontData(void) const noexcept { return fontData; }

        Kern const& getKern(void) const noexcept { return kernTable; }
        Kern&& getKern(void) noexcept { return std::move(kernTable); }
    private:
        typedef std::vector<uint16>             UHalfVec;
        typedef std::vector<int16>              HalfVec;

        struct TableDirectory {
            explicit TableDirectory(void) noexcept = default;
            explicit TableDirectory(Iter& iter);

            uint32                              checksum;
            uint32                              offset;
            uint32                              length;
        };

        struct LongHorMatrix {
            explicit LongHorMatrix(void) noexcept = default;
            explicit LongHorMatrix(uint16 advanceWidth, int16 bearing) noexcept;
            explicit LongHorMatrix(Iter& iter);

            uint16                              advanceWidth;
            int16                               leftSideBearing;
        };

        struct EncodingRecord {
            EncodingRecord(void) noexcept = default;
            EncodingRecord(Iter& iter);

            uint32                              subtableOffset;
            uint16                              platformID;
            uint16                              encodingID;
        };

        class Format4Subtable {
        public:
            explicit Format4Subtable(Iter& iter, TTFLoader& map);

            UHalfVec                            endCode;
            UHalfVec                            startCode;
            HalfVec                             idDelta;
            UHalfVec                            idRangeOffsets;
            Iter                                rangeOffsets;
        private:
            auto reserve(uint16 segCount);
            void getGlyphID(TTFLoader& map);
            void tryReadGlyph(std::size_t const& i,
                std::size_t const& j, TTFLoader& loader);
        };

        typedef std::vector<LongHorMatrix>      Metrics;
        typedef std::string                     Buffer;
        typedef std::map<Buffer, TableDirectory>
                                                Tables;

        Buffer                                  buffer;
        FileName                                fileName;
        Tables                                  tables;
        Metrics                                 metrics;
        LocaTable                               locaTable;
        Kern                                    kernTable;
        GlyphMap                                glyphMap;
        FontData                                fontData;
        int16                                   indexFormat;
        uint16                                  numGlyphs;
        uint16                                  numberOfHMetrics;

        void loadHead(void);
        void loadMaxp(void);
        void loadHhea(void);
        void loadKern(void);
        void loadCmap(void);
        void loadLoca(void);
        void loadHmtx(void);
        Iter getIterator(void) const;
        void parseFile(Iter iter);
        void parseHead(Iter& iter);
        GlyphData createGlyph(uint16 index);
        void loadCmapSubtables(Iter& iter, Iter const& begin);
        std::optional<uint32> readPlatform(EncodingRecord const& record);
    };

    template class TTFLoader<Secured>;
    template class TTFLoader<Unsecured>;

}

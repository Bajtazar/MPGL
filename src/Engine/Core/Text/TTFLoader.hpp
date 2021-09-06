#pragma once

#include "../../Collections/SafeIterator.hpp"
#include "../../Utility/Security.hpp"
#include "../../Utility/FileIO.hpp"
#include "VectorizedGlyph.hpp"

#include <variant>
#include <vector>
#include <map>

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

        struct FontData {
            uint16_t                            unitsPerEm;
            int16_t                             xMin;
            int16_t                             yMin;
            int16_t                             xMax;
            int16_t                             yMax;

            explicit FontData(void) noexcept = default;
        };

        struct Glyph {
            VectorizedGlyph                     glyph;
            uint16_t                            advanceWidth;
            int16_t                             leftSideBearing;

            explicit Glyph(VectorizedGlyph const& glyph,
                uint16_t advanceWidth, int16_t leftSideBearing) noexcept
                    : glyph{glyph}, advanceWidth{advanceWidth},
                    leftSideBearing{leftSideBearing} {}
            explicit Glyph(void) noexcept = default;
        };

        struct KernTable {
            typedef std::map<uint32_t, int16_t> DistanceMap;

            DistanceMap                         distance;
            bool                                axis;

            explicit KernTable(Iter& iter);

            void setAxis(uint16_t const& coverage) noexcept;
        };

        typedef std::map<uint16_t, Glyph>       GlyphMap;
        typedef std::vector<KernTable>          Kern;

        GlyphMap const& getGlyphs(void) const noexcept { return glyphMap; }
        GlyphMap&& getGlyphs(void) noexcept { return std::move(glyphMap); }

        FontData const& getFontData(void) const noexcept { return fontData; }

        Kern const& getKern(void) const noexcept { return kernTable; }
        Kern&& getKern(void) noexcept { return std::move(kernTable); }
    private:
        typedef std::vector<uint16_t>           UHalfVec;
        typedef std::vector<int16_t>            HalfVec;

        struct TableDirectory {
            explicit TableDirectory(void) noexcept = default;
            explicit TableDirectory(Iter& iter);

            uint32_t                            checksum;
            uint32_t                            offset;
            uint32_t                            length;
        };

        struct LongHorMatrix {
            explicit LongHorMatrix(void) noexcept = default;
            explicit LongHorMatrix(uint16_t advanceWidth, int16_t bearing) noexcept;
            explicit LongHorMatrix(Iter& iter);

            uint16_t                            advanceWidth;
            int16_t                             leftSideBearing;
        };

        struct EncodingRecord {
            EncodingRecord(void) noexcept = default;
            EncodingRecord(Iter& iter);

            uint32_t                            subtableOffset;
            uint16_t                            platformID;
            uint16_t                            encodingID;
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
            auto reserve(uint16_t segCount);
            void getGlyphID(TTFLoader& map);
            void tryReadGlyph(std::size_t const& i,
                std::size_t const& j, TTFLoader& loader);
        };

        typedef std::vector<uint16_t>           Loca16;
        typedef std::vector<uint32_t>           Loca32;
        typedef std::variant<Loca16, Loca32>    Loca;
        typedef std::vector<LongHorMatrix>      Metrics;
        typedef std::string                     Buffer;
        typedef std::map<Buffer, TableDirectory>
                                                Tables;

        Buffer                                  buffer;
        FileName                                fileName;
        Tables                                  tables;
        Metrics                                 metrics;
        Loca                                    locaTable;
        Kern                                    kernTable;
        GlyphMap                                glyphMap;
        FontData                                fontData;
        int16_t                                 indexFormat;
        uint16_t                                numGlyphs;
        uint16_t                                numberOfHMetrics;

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
        Glyph createGlyph(uint16_t index);
        uint32_t getGlyphOffset(uint16_t index) const;
        void loadCmapSubtables(Iter& iter, Iter const& begin);
        std::optional<uint32_t> readPlatform(EncodingRecord const& record);
    };

    template class TTFLoader<Secured>;
    template class TTFLoader<Unsecured>;

}

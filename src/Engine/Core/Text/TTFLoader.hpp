#pragma once

#include "../../Collections/SafeIterator.hpp"
#include "../../Utility/Security.hpp"
#include "../../Utility/FileIO.hpp"

#include <variant>
#include <vector>
#include <map>

namespace ge {

    template <security::SecurityPolicy Policy = Secured>
    class TTFLoader {
    public:
        explicit TTFLoader(std::string const& fileName);
        explicit TTFLoader(Policy policy, std::string const& fileName);

    private:
        typedef std::string::const_iterator                 BuffIter;
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<BuffIter>, BuffIter>               Iter;

        struct TableInterface {
            virtual void readTable(Iter iter) = 0;
            virtual ~TableInterface(void) noexcept = default;
        };

        struct TableDirectory {
            explicit TableDirectory(void) noexcept = default;
            explicit TableDirectory(Iter& iter);
            uint32_t        checksum;
            uint32_t        offset;
            uint32_t        length;
        };

        struct Head : TableInterface {
            uint16_t        majorVersion;
            uint16_t        minorVersion;
            float           fontRevision;
            uint32_t        checksumAdjustment;
            uint32_t        magicNumber;
            uint16_t        flags;
            uint16_t        unitsPerEm;
            uint64_t        created;    // useless
            uint64_t        modified;   // useless
            int16_t         xMin;
            int16_t         yMin;
            int16_t         xMax;
            int16_t         yMax;
            uint16_t        macStyle;
            uint16_t        lowestRecPPEM;
            int16_t         fontHint;
            int16_t         indexFormat;    // +
            int16_t         glyphFormat;
            void readTable(Iter iter) final;
            ~Head(void) noexcept = default;
        };

        struct Maxp : TableInterface {
            float           version;
            uint16_t        numGlyphs;  // +
            uint16_t        maxPoints;
            uint16_t        maxContorous;
            uint16_t        maxCompositePoints;
            uint16_t        maxCompositeContours;
            uint16_t        maxZones;
            uint16_t        maxTwilightPoints;
            uint16_t        maxStorage;
            uint16_t        maxFunctionDefs;
            uint16_t        maxInstructionDefs;
            uint16_t        maxStackElements;
            uint16_t        maxSizeOfInstructions;
            uint16_t        maxComponentElement;
            uint16_t        maxComponentDepth;
            void readTable(Iter iter) final;
            ~Maxp(void) noexcept = default;
        };

        struct Hhea : TableInterface {
            uint16_t        majorVersion;
            uint16_t        minorVersion;
            int16_t         ascender;
            int16_t         descender;
            int16_t         lineGrip;
            uint16_t        advanceWithMax;
            int16_t         minLeftSideBearing;
            int16_t         minRightSideBearing;
            int16_t         xMaxExtent;
            int16_t         caretSlopeRise;
            int16_t         caretSlopeRun;
            int16_t         caretOffset;
            int16_t         metricDataFormat;
            uint16_t        numberOfHMetrics;   // +
            void readTable(Iter iter) final;
            ~Hhea(void) noexcept = default;
        };

        struct Hmtx {
            struct LongHorMatrix {
                explicit LongHorMatrix(void) noexcept = default;
                explicit LongHorMatrix(Iter& iter);
                uint16_t    advanceWidth;
                int16_t     leftSideBearing;
            };
            std::vector<LongHorMatrix>  hMetrics;
            std::vector<int16_t>        leftSideBearings;
        };

        struct GlyfTable {
            explicit GlyfTable(void) noexcept = default;
            explicit GlyfTable(Iter iter);
            int16_t         numberOfContours;
            int16_t         xMin;
            int16_t         yMin;
            int16_t         xMax;
            int16_t         yMax;
        };

        struct Cmap {
            struct EncodingRecord {
                explicit EncodingRecord(void) noexcept = default;
                explicit EncodingRecord(Iter& iter);
                uint16_t    platformID;
                uint16_t    encodingID;
                uint32_t    subtableOffset;
            };
            struct Format4Subtable {
                explicit Format4Subtable(void) noexcept = default;
                explicit Format4Subtable(Iter& iter);
                uint16_t    length;
                uint16_t    language;
                uint16_t    segCountx2;
                uint16_t    searchRange;
                uint16_t    entrySelector;
                uint16_t    rangeShift;
                std::vector<uint16_t>   endCode;
                std::vector<uint16_t>   startCode;
                std::vector<int16_t>    idDelta;
                std::vector<uint16_t>   idRangeOffsets;
                Iter                    rangeOffsets;
            };
            explicit Cmap(void) noexcept = default;
            explicit Cmap(Iter& iter);
            uint16_t                        version;
            uint16_t                        numTables;  // +
            std::vector<EncodingRecord>     encodingRecords;
            std::optional<uint32_t> readPlatform(void);
            Format4Subtable                 format4Subtable;
            // encoded records;
            // glyph index map?
        };

        typedef std::vector<uint16_t>           Loca16;
        typedef std::vector<uint32_t>           Loca32;
        typedef std::variant<Loca16, Loca32>    Loca;
        typedef std::vector<GlyfTable>          Glyf;

        Head                                    headTable;
        Maxp                                    maxpTable;
        Hhea                                    hheaTable;
        Hmtx                                    hmtxTable;
        Loca                                    locaTable;
        Glyf                                    glyfTable;
        Cmap                                    cmapTable;

        void loadCmap(void);
        void loadGlyf(void);
        void loadLoca(void);
        void loadHmtx(void);
        void readTable(TableInterface& table, TableDirectory const& config);
        Iter getIterator(BuffIter iter) const;
        void parseFile(Iter iter);
        void parseHead(Iter& iter);

        std::string buffer;
        std::map<std::string, TableDirectory> tables;
    };

    template class TTFLoader<Secured>;
    template class TTFLoader<Unsecured>;

}

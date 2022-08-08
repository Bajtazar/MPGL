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

#include <MPGL/Core/Text/FontComponents.hpp>
#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Iterators/SafeIterator.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <variant>

namespace mpgl {

    /**
     * Reads and parses font data from a TTF format file
     *
     * @tparam Policy the scecurity policy used by TTF parser
     */
    template <security::SecurityPolicy Policy = Secured>
    class TTFLoader {
    private:
        typedef std::string::const_iterator                 BuffIter;
        typedef PolicyIterIT<Policy, BuffIter>              Iter;
        typedef std::string                                 FileName;
    public:
        /**
         * Constructs a new TTFLoader object. Loads TTF file from
         * the given path and parses it
         *
         * @param fileName the path to the TTF file
         */
        explicit TTFLoader(FileName const& fileName);

        /**
         * Constructs a new TTFLoader object. Loads TTF file from
         * a given path and parses it with given policy
         *
         * @param policy the policy token
         * @param fileName the path to the TTF file
         */
        explicit TTFLoader(
            Policy policy,
            FileName const& fileName);

        /**
         * Returns a constant reference to the map of glyph
         * informations
         *
         * @return the constant reference to the map of glyph
         * informations
         */
        GlyphMap const& getGlyphs(void) const noexcept
            { return glyphMap; }

        /**
         * Returns a rvalue reference to the map of glyph
         * informations
         *
         * @return the rvalue reference to the map of glyph
         * informations
         */
        GlyphMap&& getGlyphs(void) noexcept
            { return std::move(glyphMap); }

        /**
         * Returns a constant reference to the font data object
         *
         * @return the constant reference to the font data object
         */
        FontData const& getFontData(void) const noexcept
            { return fontData; }

        /**
         * Returns a constant reference to the kern table
         *
         * @return the constant reference to the kern table
         */
        Kern const& getKern(void) const noexcept
            { return kernTable; }

        /**
         * Returns a rvalue reference to the kern table
         *
         * @return the rvalue reference to the kern table
         */
        Kern&& getKern(void) noexcept
            { return std::move(kernTable); }
    private:
        typedef std::vector<uint16>             UHalfVec;
        typedef std::vector<int16>              HalfVec;

        /**
         * Provides information about a TTF table
         */
        struct TableDirectory {
            /**
             * Constructs a new Table Directory object
             */
            explicit TableDirectory(void) noexcept = default;

            /**
             * Constructs a new Table Directory object from
             * the given reference to the iterator
             *
             * @param iter the reference to the iterator
             */
            explicit TableDirectory(Iter& iter);

            uint32                              checksum;
            uint32                              offset;
            uint32                              length;
        };

        /**
         * Provides information about the Long Hor Matrix Table
         */
        struct LongHorMatrix {
            /**
             * Constructs a new Long Hor Matrix object
             */
            explicit LongHorMatrix(void) noexcept = default;

            /**
             * Constructs a new Long Hor Matrix object from
             * the given advance width and bearing
             *
             * @param advanceWidth the advance width
             * @param bearing the bearing
             */
            explicit LongHorMatrix(
                uint16 advanceWidth,
                int16 bearing) noexcept;

            /**
             * Constructs a new Long Hor Matrix object from
             * the given reference to the iterator
             *
             * @param iter the reference to the iterator
             */
            explicit LongHorMatrix(Iter& iter);

            uint16                              advanceWidth;
            int16                               leftSideBearing;
        };

        /**
         * Provides information about the Encoding Record Table
         */
        struct EncodingRecord {
            /**
             *  Constructs a new Encoding Record object
             */
            EncodingRecord(void) noexcept = default;

            /**
             * Constructs a new Encoding Record object from
             * the given reference to the iterator
             *
             * @param iter the reference to the iterator
             */
            EncodingRecord(Iter& iter);

            uint32                              subtableOffset;
            uint16                              platformID;
            uint16                              encodingID;
        };

        /**
         * Provides information about the subtables provided
         * by a format 4 subtable
         */
        class Format4Subtable {
        public:
            /**
             * Constructs a new Format 4 Subtable object from
             * the given reference to the iterator and the reference
             * to the TTFLoader object
             *
             * @param iter the reference to the iterator
             * @param loader the reference to the TTFLoader object
             */
            explicit Format4Subtable(
                Iter& iter,
                TTFLoader& loader);

            UHalfVec                            endCode;
            UHalfVec                            startCode;
            HalfVec                             idDelta;
            UHalfVec                            idRangeOffsets;
            Iter                                rangeOffsets;
        private:
            /**
             * Reserves the memory for the given number of segments
             * in the memory
             *
             * @param segCount the number of segments in the table
             * @return the iota view counting from 0 to the number
             * of segments
             */
            auto reserve(uint16 segCount);

            /**
             * Loads the glyphs into the TTF loader's glyph map
             *
             * @param loader the reference to the TTFLoader object
             */
            void loadGlyphs(TTFLoader& loader);

            /**
             * Tries to read the glyph into the TTF loader's
             * glyph map
             *
             * @param i the number of start code
             * @param j the number of end code
             * @param loader the reference to the TTFLoader object
             */
            void tryReadGlyph(
                std::size_t const& i,
                std::size_t const& j,
                TTFLoader& loader);
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

        /**
         * Loads the HEAD table into memory
         */
        void loadHead(void);

        /**
         * Loads the MAXP table into memory
         */
        void loadMaxp(void);

        /**
         * Loads the HHEA table into memory
         */
        void loadHhea(void);

        /**
         * Loads the KERN table into memory
         */
        void loadKern(void);

        /**
         * Loads the CMAP table into memory
         */
        void loadCmap(void);

        /**
         * Loads the LOCA table into memory
         */
        void loadLoca(void);

        /**
         * Loads the HMTX table into memory
         */
        void loadHmtx(void);

        /**
         * Returns the iterator to the parsed file
         *
         * @return Iter the iterator to the parsed file
         */
        Iter getIterator(void) const;

        /**
         * Parses TTF file using the given iterator
         *
         * @param iter the iterator to the parsed file
         */
        void parseFile(Iter iter);

        /**
         * Parses the TTF file's header
         *
         * @param iter
         */
        void parseHead(Iter& iter);

        /**
         * Creates the Glyph Data object containing informations
         * about the glyph with the given id
         *
         * @param index the glyph's id
         * @return the glyph data object holding glyph's data
         */
        GlyphData createGlyph(uint16 index);

        /**
         * Loads the CMAP table subtables
         *
         * @param iter the reference to the parsed file
         * @param begin the iterator to the end of the CMAP table
         */
        void loadCmapSubtables(Iter& iter, Iter const& begin);

        /**
         * Returns an optional with the offset to the format 4 subtable
         * on the given platform. Returns an empty optional if
         * the platform is not supported
         *
         * @param record the constant reference to the encoding
         * record
         * @return the optional holding offset to the format 4
         * subtable
         */
        std::optional<uint32> readPlatform(
            EncodingRecord const& record);
    };

    template class TTFLoader<Secured>;
    template class TTFLoader<Unsecured>;

}

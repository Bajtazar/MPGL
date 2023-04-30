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

#include <MPGL/Core/Text/TTFLoader.hpp>
#include <MPGL/Core/Text/Glyph.hpp>

namespace mpgl {

    /**
     * Contains the kern table and all of the available subfont
     * glyphs
     */
    class Subfont {
    public:
        typedef std::reference_wrapper<Glyph const> GlyphRef;
        typedef std::optional<GlyphRef>             FontGlyph;

        /**
         * Loads the subfont from the given file path
         *
         * @param path the file path
         */
        explicit Subfont(std::string const& path);

        /**
         * Returns an optional with the reference wrapper to
         * the constant glyph with given id number and level.
         * If there is no glyph with given id then returns
         * an empty optional. If there is no glyph with given
         * level than renders one
         *
         * @param number the id number of a glyph
         * @param level the level of the glyph
         * @return the optional with the reference wrapper to
         * the constant glyph
         */
        FontGlyph operator() (uint16 number, uint8 level);

        /**
         * Returns an optional with the reference wrapper to
         * the constant glyph with given id number and level.
         * If there is no glyph with given id or level then returns
         * an empty optional.
         *
         * @param number the id number of a glyph
         * @param level the level of the glyph
         * @return the optional with the reference wrapper to
         * the constant glyph
         */
        FontGlyph operator() (uint16 number, uint8 level) const;

        /**
         * Returns a constant reference to the kern table
         *
         * @return the constant reference to the kern table
         */
        Kern const& getKern(void) const noexcept
            { return kern; }

        /// The shift base of the subfont
        static constexpr std::size_t                shiftBase = 64;
    private:
        typedef std::map<uint16, Glyph>             RasterMap;
        typedef std::map<uint8, RasterMap>          SizeMap;
        typedef Glyph::TextureVar                   TextureVar;
        typedef typename GlyphMap::const_iterator   Iter;
        typedef std::reference_wrapper<
            RasterMap const>                        RasterMapCref;
        typedef std::optional<RasterMapCref>        MapVar;

        /**
         * Returns a reference to the map containing glyphs with
         * given level. If there is no such a map then creates
         * it
         *
         * @param level the level of the glyph
         * @return the reference to the raster map
         */
        RasterMap& getMap(uint8 level);

        /**
         * Returns an optional with the reference wrapper to the
         * constant raster map with given level. If there is no
         * such a map then returns an empty optional
         *
         * @param level the level of the glyph
         * @return the optional with the reference wrapper to the
         * constant raster map
         */
        MapVar getMap(uint8 level) const;

        /**
         * Rasterizes glyph from the given constant reference
         * to the constant iterator of the glyph map and glyph's
         * level
         *
         * @param iter the glyph map's constant iterator
         * @param level the glyph's level
         * @return the rasterized glyph
         */
        Glyph createGlyph(Iter const& iter, uint8 level);

        /**
         * Returns glyph's dimensions
         *
         * @param glyph the constant reference to the glyph
         * data object
         * @param size the glyph size
         * @return the vector containing glyph dimensions
         */
        Vector2u getDimensions(
            GlyphData const& glyph,
            std::size_t size) const noexcept;

        /**
         * Returns glyph's bearings
         *
         * @param glyph the constant reference to the glyph
         * data object
         * @param size the glyph size
         * @return the vector containing glyph bearings
         */
        Vector2i getBearings(
            GlyphData const& glyph,
            std::size_t size) const noexcept;

        /**
         * Renders glyph's texture if there exist the glyph's
         * outline and returns it inside an optional. Otherwise
         * returns an empty optional
         *
         * @param iter the glyph map's constant iterator
         * @param size the glyph's size
         * @return the optional with glyph's texture
         */
        TextureVar renderTexture(
            Iter const& iter,
            std::size_t size) const;

        SizeMap                                     sizeMap;
        GlyphMap                                    glyphMap;
        FontData                                    fontData;
        Kern                                        kern;
    };

}

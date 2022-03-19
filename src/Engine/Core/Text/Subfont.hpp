#pragma once

#include "Glyph.hpp"
#include "TTFLoader.hpp"

namespace mpgl {

    class Subfont {
    public:
        typedef std::reference_wrapper<Glyph const> GlyphRef;
        typedef std::optional<GlyphRef>             FontGlyph;

        explicit Subfont(std::string const& path);

        FontGlyph operator() (uint16 number, uint8 level);

        FontGlyph operator() (uint16 number, uint8 level) const;

        Kern const& getKern(void) const noexcept
            { return kern; }

        static constexpr std::size_t                shiftBase = 64;
    private:
        typedef std::map<uint16, Glyph>             RasterMap;
        typedef std::map<uint8, RasterMap>          SizeMap;
        typedef Glyph::TextureVar                   TextureVar;
        typedef typename GlyphMap::const_iterator   Iter;
        typedef std::reference_wrapper<
            RasterMap const>                        RasterMapCref;
        typedef std::optional<RasterMapCref>        MapVar;

        RasterMap& getMap(uint8 level);
        MapVar getMap(uint8 level) const;

        Glyph createGlyph(Iter const& iter, uint8 level);

        Vector2u getDimensions(GlyphData const& glyph,
            std::size_t size) const noexcept;

        Vector2i getBearings(GlyphData const& glyph,
            std::size_t size) const noexcept;

        TextureVar renderTexture(Iter const& iter,
            std::size_t size) const;

        SizeMap                                     sizeMap;
        GlyphMap                                    glyphMap;
        FontData                                    fontData;
        Kern                                        kern;
    };

}

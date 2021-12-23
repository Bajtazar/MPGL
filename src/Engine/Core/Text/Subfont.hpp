#pragma once

#include "Glyph.hpp"
#include "TTFLoader.hpp"

namespace ge {

    class Subfont {
    public:
        typedef std::reference_wrapper<Glyph const>     GlyphRef;
        typedef std::optional<GlyphRef>                 FontGlyph;

        explicit Subfont(std::string const& path);

        FontGlyph operator() (uint16_t number, uint8_t level);

        Kern const& getKern(void) const noexcept { return kern; }
    private:
        typedef std::map<uint16_t, Glyph>               RasterMap;
        typedef std::map<uint8_t, RasterMap>            SizeMap;
        typedef Glyph::TextureVar                       TextureVar;
        typedef typename GlyphMap::const_iterator       Iter;

        RasterMap& getMap(uint8_t level);

        Glyph createGlyph(Iter const& iter, uint8_t level);

        Vector2ui getDimmensions(GlyphData const& glyph,
            std::size_t size) const noexcept;

        Vector2i getBearings(GlyphData const& glyph,
            std::size_t size) const noexcept;

        TextureVar renderTexture(Iter const& iter,
            std::size_t size) const;

        SizeMap                                         sizeMap;
        GlyphMap                                        glyphMap;
        FontData                                        fontData;
        Kern                                            kern;
    };

}

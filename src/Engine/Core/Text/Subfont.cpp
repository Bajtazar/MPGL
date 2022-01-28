#include "Subfont.hpp"
#include "FontRasterizer.hpp"

#include <math.h>

namespace ge {

    Subfont::Subfont(std::string const& path) {
        TTFLoader loader{path};
        glyphMap = std::move(loader.getGlyphs());
        fontData = std::move(loader.getFontData());
        kern = std::move(loader.getKern());
    }

    Subfont::FontGlyph Subfont::operator() (
        uint16 number, uint8 level)
    {
        auto& map = getMap(level);
        auto iter = map.find(number);
        if (iter == map.end()) {
            auto glpyhIter = glyphMap.find(number);
            if (glpyhIter == glyphMap.end())
                return {};
            iter = map.emplace(number,
                createGlyph(glpyhIter, level)).first;
        }
        return {std::cref(iter->second)};
    }

    Subfont::RasterMap& Subfont::getMap(uint8 level) {
        auto iter = sizeMap.find(level);
        if (iter == sizeMap.end())
            iter = sizeMap.emplace(level, RasterMap{}).first;
        return iter->second;
    }

    Vector2u Subfont::getDimensions(GlyphData const& glyph,
        std::size_t size) const noexcept
    {
        return vectorCast<uint32>(
            vectorCast<uint32>(glyph.glyph.getMaxDimensions()
                - glyph.glyph.getMinDimensions())
            * static_cast<uint32>(size) /
                static_cast<uint32>(fontData.unitsPerEm));
    }

    Vector2i Subfont::getBearings(GlyphData const& glyph,
        std::size_t size) const noexcept
    {
        return vectorCast<int32>(glyph.glyph.getMinDimensions())
            * static_cast<int32>(size) /
                static_cast<int32>(fontData.unitsPerEm);
    }

    Glyph Subfont::createGlyph(Iter const& iter, uint8 level) {
        std::size_t size = shiftBase << level;
        auto const& glyphData = iter->second;
        auto&& dimensions = getDimensions(glyphData, size);
        auto&& bearings = getBearings(glyphData, size);
        uint16 advanceWidth = size * glyphData.advanceWidth
            / fontData.unitsPerEm;
        return Glyph{renderTexture(iter, size),
            dimensions, bearings, advanceWidth};
    }

    Subfont::TextureVar Subfont::renderTexture(
        Iter const& iter, std::size_t size) const
    {
        if (!iter->second.glyph.exist())
            return {};
        FontRasterizer raster{fontData, iter->second, size};
        return {Texture{raster()}};
    }

}

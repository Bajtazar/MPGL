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

    Subfont::FontGlyph Subfont::operator() (uint16_t number, uint8_t level) {
        auto& map = getMap(level);
        auto iter = map.find(number);
        if (iter == map.end()) {
            auto glpyhIter = glyphMap.find(number);
            if (glpyhIter == glyphMap.end())
                return {};
            iter = map.emplace(number, createGlyph(glpyhIter, level)).first;
        }
        return {std::cref(iter->second)};
    }

    Subfont::RasterMap& Subfont::getMap(uint8_t level) {
        auto iter = sizeMap.find(level);
        if (iter == sizeMap.end())
            iter = sizeMap.emplace(level, RasterMap{}).first;
        return iter->second;
    }

    Vector2ui Subfont::getDimmensions(GlyphData const& glyph,
        std::size_t size) const noexcept
    {
        return vectorCast<uint32_t>(
            vectorCast<uint32_t>(glyph.glyph.getMaxDimmensions() - glyph.glyph.getMinDimmensions())
            * static_cast<uint32_t>(size) / static_cast<uint32_t>(fontData.unitsPerEm));
    }

    Vector2i Subfont::getBearings(GlyphData const& glyph,
        std::size_t size) const noexcept
    {
        return vectorCast<int32_t>(glyph.glyph.getMinDimmensions())
            * static_cast<int32_t>(size) / static_cast<int32_t>(fontData.unitsPerEm);
    }

    Glyph<> Subfont::createGlyph(Iter const& iter, uint8_t level) {
        std::size_t size = 64 << level;
        auto const& glyphData = iter->second;
        auto&& dimmensions = getDimmensions(glyphData, size);
        auto&& bearings = getBearings(glyphData, size);
        uint16_t advanceWidth = size * glyphData.advanceWidth / fontData.unitsPerEm;
        return Glyph<>{renderTexture(iter, size), dimmensions, bearings, advanceWidth};
    }

    Subfont::TextureVar Subfont::renderTexture(Iter const& iter, std::size_t size) const {
        if (!iter->second.glyph.exist())
            return {};
        FontRasterizer raster{fontData, iter->second, size};
        Texture<>::Options options;
        return {Texture<>{raster(), options}};
    }

}

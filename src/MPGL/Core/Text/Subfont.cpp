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
#include <MPGL/Core/Text/FontRasterizer.hpp>
#include <MPGL/Core/Text/Subfont.hpp>

#include <math.h>

namespace mpgl {

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

    Subfont::FontGlyph Subfont::operator() (
        uint16 number, uint8 level) const
    {
        if (MapVar var = getMap(level)) {
            auto iter = var->get().find(number);
            if (iter == var->get().end())
                return {};
            return {std::cref(iter->second)};
        }
        return {};
    }

    Subfont::RasterMap& Subfont::getMap(uint8 level) {
        auto iter = sizeMap.find(level);
        if (iter == sizeMap.end())
            iter = sizeMap.emplace(level, RasterMap{}).first;
        return iter->second;
    }

    Subfont::MapVar Subfont::getMap(uint8 level) const {
        auto iter = sizeMap.find(level);
        if (iter == sizeMap.end())
            return {};
        return {std::cref(iter->second)};
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
        return {Texture{raster(), {
            Texture::Options::TextureWrapper::ClampToEdge,
            Texture::Options::TextureWrapper::ClampToEdge,
            Texture::Options::MinifyingTextureFilter::Linear,
            Texture::Options::MagnifyingTextureFilter::Linear,
            {},
            false
        }}};
    }

}

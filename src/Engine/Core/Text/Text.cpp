#include "Text.hpp"
#include "UTF-8.hpp"

namespace ge {

    template <bool isPolichromatic>
    Text<isPolichromatic>::Text(ScenePtr const& scene,
        Font& font, std::size_t size, std::string const& text,
        Vector2f const& position, Color const& color, Font::Type const& type)
            : Drawable{scene}, text{text}, /*(color{color},*/ position{position},
            size{size}, angle{0.f}, font{font}, type{type}
    {
        auto indexes = parseString(text);
        drawGlyphs(indexes);
    }

    template <bool isPolichromatic>
    Text<isPolichromatic>::IDArray Text<isPolichromatic>::parseString(std::string string)
    {
        auto iter = string.begin();
        IDArray array;
        while (iter < string.end()) {   // put SafeIterator in case
            uint8_t length = getUTF8SequenceLength(*iter);
            array.push_back(fromUTF8(iter, std::next(iter, length)));
            std::advance(iter, length);
        }
        return array;
    }

    template <bool isPolichromatic>
    uint8_t Text<isPolichromatic>::getLevel(void) const {
        uint8_t level = std::ceil(log2(size));
        return level > 7 ? level - 7 : 0;
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::drawGlyphs(IDArray const& indexes) {
        auto& subfont = font(type);
        auto level = getLevel();
        auto rotation = rotationMatrix<float>(angle);
        float scale = (float) size / (128 << level);
        for (uint16_t const& index : indexes)
            drawGlyph(subfont, level, scale, index, rotation);
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::drawSingleGlyph(uint16_t const& index) {
        auto& subfont = font(type);
        auto level = getLevel();
        auto rotation = rotationMatrix<float>(angle);
        float scale = (float) size / (128 << level);
        drawGlyph(subfont, level, scale, index, rotation);
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::drawGlyph(Subfont& subfont, uint8_t level,
        float scale, uint16_t const& index, Matrix<float, 2> const& rotation)
    {
        if (auto glyph = subfont(index, level)) {
            if (auto texture = glyph->get().texture) {
                Vector2f xVersor = rotation * Vector2f{float(glyph->get().dimmensions[0]), 0.f} * scale;
                Vector2f yVersor = rotation * Vector2f{0.f, float(glyph->get().dimmensions[1])} * scale;
                Vector2f bearing = rotation * vectorCast<float>(glyph->get().bearing) * scale;
                glyphs.emplace_back(scene, *texture, position + bearing,
                    position + bearing + yVersor, position + bearing + xVersor);
            }
            position += rotation * Vector2f{float(glyph->get().advance * scale), 0.f};
        }
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setShaders(ShaderLibrary const& library) noexcept {
        std::ranges::for_each(glyphs, [&library](auto& glyph) { glyph.setShaders(library); });
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::copyToGPU(void) noexcept {
        std::ranges::for_each(glyphs, [](auto& glyph) { glyph.copyToGPU(); });
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::draw(void) const noexcept {
        std::ranges::for_each(glyphs, [](auto const& glyph) { glyph.draw(); });
    }

}

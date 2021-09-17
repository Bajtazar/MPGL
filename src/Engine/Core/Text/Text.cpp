#include "Text.hpp"
#include "UTF-8.hpp"
#include "../Figures/Views.hpp"

namespace ge {

    template <bool isPolichromatic>
    Text<isPolichromatic>::Text(Font& font, std::size_t size,
        Vector2f const& position, std::string const& text,
        Color const& color, Font::Type const& type)
            : text{text}, color{color}, position{position},
            size{size}, angle{0.f}, font{font}, library{nullptr}, type{type}
    {
        drawGlyphs(parseString(text));
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
        return level > 6 ? level - 6 : 0;
    }

    template <bool isPolichromatic>
    Text<isPolichromatic>::ArgTuple Text<isPolichromatic>::getArgs(void) const noexcept
    {
        uint8_t level = getLevel();
        auto rotation = rotationMatrix<float>(angle);
        float scale = (float) size / (64 << level);
        return {level, scale, rotation};
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::drawGlyphs(IDArray const& indexes) {
        auto& subfont = font.get()(type);
        auto&& [level, scale, rotation] = getArgs();
        for (uint16_t const& index : indexes)
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
                glyphs.emplace_back(*texture, position + bearing,
                    position + bearing + yVersor, position + bearing + xVersor, color);
            }
            position += rotation * Vector2f{float(glyph->get().advance * scale), 0.f};
        }
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setShaders(ShaderLibrary const& library) noexcept {
        this->library = &library;
        std::ranges::for_each(glyphs, [&library](auto& glyph) { glyph.setShaders(library); });
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::copyToGPU(void) const noexcept {
        std::ranges::for_each(glyphs, [](auto const& glyph) { glyph.copyToGPU(); });
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::draw(void) const noexcept {
        std::ranges::for_each(glyphs, [](auto const& glyph) { glyph.draw(); });
    }

    template <bool isPolichromatic>
    Vector2f Text<isPolichromatic>::getPosition(void) const noexcept {
        if (!glyphs.size())
            return position;
        uint8_t len = getUTF8SequenceLength(text.front());
        uint16_t index = fromUTF8(text.begin(), std::next(text.begin(), len));
        auto&& [level, scale, rotation] = getArgs();
        auto const& glyph = font.get()(type)(index, level);
        Vector2f bearing = rotation * vectorCast<float>(
            glyph->get().bearing) * scale;
        return Vector2f{glyphs.front()[0].position} - bearing;
    }

    template <bool isPolichromatic>
    Vector2f Text<isPolichromatic>::getDimmensions(void) const noexcept {
        if (glyphs.size())
            return {(static_cast<Vector2f>(glyphs.size() > 1 ? glyphs.back()[2].position
                : glyphs.front()[2].position) - static_cast<Vector2f>(
                glyphs.front()[1].position)).length(), float(size)};
        return {0.f, 0.f};
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setShadersIfLibrary(void) noexcept {
        if (library)
            std::ranges::for_each(glyphs, [this](auto& glyph) { glyph.setShaders(*library); });
    }

    template <bool isPolichromatic>
    Text<isPolichromatic>& Text<isPolichromatic>::operator+= (std::string const& left) {
        text += left;
        drawGlyphs(parseString(left));
        setShadersIfLibrary();
        copyToGPU();
        return *this;
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::clear(void) noexcept {
        position = getPosition();
        text.clear();
        glyphs.clear();
        copyToGPU();
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::redrawGlyphs(void) {
        position = getPosition();
        glyphs.clear();
        drawGlyphs(parseString(text));
        setShadersIfLibrary();
        copyToGPU();
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setString(std::string const& text) {
        this->text = text;
        redrawGlyphs();
    }

    template <bool isPolichromatic>
    Text<isPolichromatic>& Text<isPolichromatic>::operator= (
        std::string const& text)
    {
        setString(text);
        return *this;
    }

    template <bool isPolichromatic>
    Text<isPolichromatic>& Text<isPolichromatic>::operator= (
        std::string&& text)
    {
        this->text = std::move(text);
        redrawGlyphs();
        return *this;
    }

    template <bool isPolichromatic>
    void  Text<isPolichromatic>::setSize(std::size_t size) {
        this->size = size;
        redrawGlyphs();
    }

    template <bool isPolichromatic>
    void  Text<isPolichromatic>::setColor(Color const& color) {
        this->color = color;
        std::ranges::for_each(glyphs, [&color](auto& glyph){ glyph.setColor(color); });
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setStyle(Font::Type const& type) {
        this->type = type;
        redrawGlyphs();
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::setFont(Font& font) {
        this->font = std::ref(font);
        redrawGlyphs();
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::translate(Vector2f const& shift) noexcept {
        std::ranges::for_each(glyphs, [&shift](auto& glyph){ glyph.translate(shift); });
        position += shift;
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::rotate(Vector2f const& center, float angle) noexcept {
        rotate(center, rotationMatrix<float>(angle));
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        std::ranges::for_each(glyphs, [&center, &rotation](auto& glyph){ glyph.rotate(center, rotation); });
        position = rotation * (position - center) + center;
        auto twoPi = std::numbers::pi_v<float> * 2;
        this->angle += twoPi - angle;
        this->angle = angle > twoPi ? angle - twoPi : angle < 0.f ? twoPi + angle : angle;
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::scale(Vector2f const& center, float factor) noexcept {
        std::ranges::for_each(glyphs, [&center, &factor](auto& glyph){ glyph.scale(center, factor); });
        size *= factor;
        position = (position - center) * factor + center;
    }

    template <bool isPolichromatic>
    void Text<isPolichromatic>::onScreenTransformation(Vector2ui const& oldDimmensions) noexcept {
        std::ranges::for_each(glyphs, [&oldDimmensions](auto& glyph)
            { glyph.onScreenTransformation(oldDimmensions); });
    }

}

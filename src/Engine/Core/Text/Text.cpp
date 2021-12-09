#include "Text.hpp"
#include "UTF-8.hpp"
#include "../Figures/Views.hpp"

namespace ge {

    template <bool IsPolichromatic>
    Text<IsPolichromatic>::Text(Font& font, std::size_t size,
        Vector2f const& position, std::string const& text,
        Color const& color, Font::Type const& type)
            : Shadeable{shaderType(), shaderExec}, text{text},
            color{color}, position{position}, size{size},
            angle{0.f}, font{font}, type{type}
    {
        drawGlyphs(parseString(text));
    }

    template <bool IsPolichromatic>
    Text<IsPolichromatic>::IDArray Text<IsPolichromatic>::parseString(std::string string)
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

    template <bool IsPolichromatic>
    uint8_t Text<IsPolichromatic>::getLevel(void) const {
        uint8_t level = std::ceil(log2(size));
        return level > 6 ? level - 6 : 0;
    }

    template <bool IsPolichromatic>
    Text<IsPolichromatic>::ArgTuple Text<IsPolichromatic>::getArgs(void) const noexcept
    {
        uint8_t level = getLevel();
        auto rotation = rotationMatrix<float>(angle);
        float scale = (float) size / (64 << level);
        return {level, scale, rotation};
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::drawGlyphs(IDArray const& indexes) {
        auto& subfont = font.get()(type);
        auto&& [level, scale, rotation] = getArgs();
        for (uint16_t const& index : indexes)
            drawGlyph(subfont, level, scale, index, rotation);
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::drawGlyph(Subfont& subfont, uint8_t level,
        float scale, uint16_t const& index, Matrix2f const& rotation)
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

    template <bool IsPolichromatic>
    const Text<IsPolichromatic>::Executable Text<IsPolichromatic>::shaderExec =
        [](ProgramPtr& ptr) -> void { ptr->use(); ptr->setUniform("tex", 0); };

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::copyToGPU(void) const noexcept {
        std::ranges::for_each(glyphs, [](auto const& glyph) { glyph.copyToGPU(); });
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::draw(void) const noexcept {
        shaderProgram->use();
        if constexpr (!IsPolichromatic)
            shaderProgram->setUniform("color", color);
        std::ranges::for_each(glyphs, [](auto const& glyph) { glyph.draw(); });
    }

    template <bool IsPolichromatic>
    Vector2f Text<IsPolichromatic>::getPosition(void) const noexcept {
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

    template <bool IsPolichromatic>
    Vector2f Text<IsPolichromatic>::getDimmensions(void) const noexcept {
        if (glyphs.size())
            return {(static_cast<Vector2f>(glyphs.size() > 1 ? glyphs.back()[2].position
                : glyphs.front()[2].position) - static_cast<Vector2f>(
                glyphs.front()[1].position)).length(), float(size)};
        return {0.f, 0.f};
    }

    template <bool IsPolichromatic>
    Text<IsPolichromatic>& Text<IsPolichromatic>::operator+= (std::string const& left) {
        text += left;
        drawGlyphs(parseString(left));
        copyToGPU();
        return *this;
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::clear(void) noexcept {
        position = getPosition();
        text.clear();
        glyphs.clear();
        copyToGPU();
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::redrawGlyphs(void) {
        position = getPosition();
        glyphs.clear();
        drawGlyphs(parseString(text));
        copyToGPU();
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::setString(std::string const& text) {
        this->text = text;
        redrawGlyphs();
    }

    template <bool IsPolichromatic>
    Text<IsPolichromatic>& Text<IsPolichromatic>::operator= (
        std::string const& text)
    {
        setString(text);
        return *this;
    }

    template <bool IsPolichromatic>
    Text<IsPolichromatic>& Text<IsPolichromatic>::operator= (
        std::string&& text)
    {
        this->text = std::move(text);
        redrawGlyphs();
        return *this;
    }

    template <bool IsPolichromatic>
    void  Text<IsPolichromatic>::setSize(std::size_t size) {
        this->size = size;
        redrawGlyphs();
    }

    template <bool IsPolichromatic>
    void  Text<IsPolichromatic>::setColor(Color const& color) {
        this->color = color;
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::setStyle(Font::Type const& type) {
        this->type = type;
        redrawGlyphs();
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::setFont(Font& font) {
        this->font = std::ref(font);
        redrawGlyphs();
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::translate(Vector2f const& shift) noexcept {
        std::ranges::for_each(glyphs, [&shift](auto& glyph){ glyph.translate(shift); });
        position += shift;
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::rotate(Vector2f const& center, float angle) noexcept {
        rotate(center, rotationMatrix<float>(angle));
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        std::ranges::for_each(glyphs, [&center, &rotation](auto& glyph){ glyph.rotate(center, rotation); });
        position = rotation * (position - center) + center;
        auto twoPi = std::numbers::pi_v<float> * 2;
        this->angle += twoPi - angle;
        this->angle = angle > twoPi ? angle - twoPi : angle < 0.f ? twoPi + angle : angle;
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::scale(Vector2f const& center, float factor) noexcept {
        std::ranges::for_each(glyphs, [&center, &factor](auto& glyph){ glyph.scale(center, factor); });
        size *= factor;
        position = (position - center) * factor + center;
    }

    template <bool IsPolichromatic>
    void Text<IsPolichromatic>::onScreenTransformation(Vector2ui const& oldDimmensions) noexcept {
        std::ranges::for_each(glyphs, [&oldDimmensions](auto& glyph)
            { glyph.onScreenTransformation(oldDimmensions); });
    }

}

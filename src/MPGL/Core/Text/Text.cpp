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
#include <MPGL/Utility/Deferred/DeferredExecutionWrapper.hpp>
#include <MPGL/Core/Figures/Views.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>
#include <MPGL/Core/Text/Text.hpp>

namespace mpgl {

    MonochromaticTextBase::MonochromaticTextBase(void)
        : colorLoc{new ShaderLocation} {}

    template <bool IsColorable>
    Text<IsColorable>::String const
        Text<IsColorable>::shaderType(void)
    {
        if constexpr (IsColorable)
            return "MPGL/2D/PoliGlyph";
        else
            return "MPGL/2D/MonoGlyph";
    }

    template <bool IsColorable>
    const Text<IsColorable>::Executable Text<IsColorable>::shaderExec =
        [](ShaderProgram const& program) -> void {
            ShaderLocation{program, "tex"}(0);
        };

    template <bool IsColorable>
    Tetragon Text<IsColorable>::generateUnderline(
        Vector2f position,
        float32 angle,
        SizeT textSize,
        Color const& color) noexcept
    {
        float span = 2 * std::ceil(0.05f * textSize);
        Tetragon line{position - Vector2f{0.f,
            span}, Vector2f{0.f, span}, color};
        line.rotate(position, angle);
        return line;
    }

    template <bool IsColorable>
    Tetragon Text<IsColorable>::generateStrikethrough(
        Vector2f position,
        float32 angle,
        SizeT textSize,
        Color const& color) noexcept
    {
        float midspan = std::floor(textSize / 2.5);
        float halfspan =  std::ceil(0.05f * textSize);
        Tetragon line{position + Vector2f{0.f,
            midspan - halfspan}, Vector2f{0.f, 2 * halfspan}, color};
        line.rotate(position, angle);
        return line;
    }

    template <bool IsColorable>
    void Text<IsColorable>::setLocations(void) {
        if constexpr (!IsColorable)
            Shadeable::setLocations(DeferredExecutionWrapper{
                this->shaderProgram, MonochromaticTextBase::colorLoc}(
                [](auto program, auto color) {
                    *color = ShaderLocation{*program, "color"};
                }));
    }

    template <bool IsColorable>
    Text<IsColorable>::Text(
        Font const& font,
        Vector2f const& position,
        String const& text,
        TextOptions const& options) : Shadeable{shaderType(),
            shaderExec}, text{text}, font{font}, underlines{
                generateUnderline(position, options.angle, options.size,
                options.color)}, color{options.color},
            strikethroughs{
                generateStrikethrough(position, options.angle,
                options.size, options.color)},
            position{position}, textSize{options.size},
            angle{options.angle}, style{options.style},
            mods{options.mods}
    {
        loadGlyphs(parseString(text));
        setLocations();
    }

    template <bool IsColorable>
    Text<IsColorable>::IDArray
        Text<IsColorable>::parseString(String const& string)
    {
        auto iter = string.begin();
        IDArray array;
        while (iter < string.end()) {
            uint8 length = getUTF8SequenceLength(*iter);
            array.push_back(fromUTF8(iter, std::next(iter, length)));
            std::advance(iter, length);
        }
        return array;
    }

    template <bool IsColorable>
    void Text<IsColorable>::extendModifiers(
        Vector2f advance) noexcept
    {
        if (mods & Modifiers::Underline)
            extendUnderline(advance);
        if (mods & Modifiers::Strikethrough)
            extendStrikethrough(advance);
    }

    template <bool IsColorable>
    void Text<IsColorable>::extendUnderline(
        Vector2f advance) noexcept
    {
        underlines.back()[2] & cast::position = advance
            + Vector2f{underlines.back()[2] & cast::position};
        underlines.back()[3] & cast::position = advance
            + Vector2f{underlines.back()[3] & cast::position};
    }

    template <bool IsColorable>
    void Text<IsColorable>::extendStrikethrough(
        Vector2f advance) noexcept
    {
        strikethroughs.back()[2] & cast::position = advance
            + Vector2f{strikethroughs.back()[2] & cast::position};
        strikethroughs.back()[3] & cast::position = advance
            + Vector2f{strikethroughs.back()[3] & cast::position};
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceUnderline(void) {
        underlines.push_back(generateUnderline(
            position, angle, textSize, color));
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceStrikethrough(void) {
        strikethroughs.push_back(generateStrikethrough(
            position, angle, textSize, color));
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceModifiers(void) {
        if (mods & Modifiers::Underline)
            emplaceUnderline();
        if (mods & Modifiers::Strikethrough)
            emplaceStrikethrough();
    }

    template <bool IsColorable>
    uint8 Text<IsColorable>::getLevel(void) const noexcept {
        uint8 level = std::ceil(log2(textSize));
        return level > ShiftValue ? level - ShiftValue : 0;
    }

    template <bool IsColorable>
    Text<IsColorable>::ArgTuple
        Text<IsColorable>::glyphCoefficients(void) const noexcept
    {
        uint8 level = getLevel();
        return {
            level,
            (float32) textSize / (ShiftBase << level),
            rotationMatrix<float32>(angle)
        };
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadGlyphs(IDArray const& indexes) {
        auto& subfont = font(style);
        auto&& [level, scale, rotation] = glyphCoefficients();
        for (uint16 const& index : indexes)
            loadGlyph(subfont, level, scale, index, rotation);
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadGlyph(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        uint16 index,
        Matrix2f const& rotation)
    {
        switch (index) {
            case 10:
                return loadNewline(subfont, level, scale, rotation);
            case 9:
                return loadTab(subfont, level, scale, rotation);
            default:
                return loadCharacter(
                    subfont, level, scale, index, rotation);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadTab(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        Matrix2f const& rotation)
    {
        /// tab is 4 times longer than space
        if (auto glyph = subfont(32, level)) {
            Vector2f advance = rotation * Vector2f{
                4 * float32(glyph->get().advance * scale), 0.f};
            position += advance;
            extendModifiers(advance);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadNewline(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        Matrix2f const& rotation)
    {
        Vector2f position = getPosition();
        Matrix2f rot = rotationMatrix<float32>(angle);
        Vector2f yVersor = rot * 1._y;
        Vector2f intersec = intersectionOf(
            position, yVersor, this->position, rot * 1._x);
        this->position = position - 1.1f * textSize * yVersor;
        emplaceModifiers();
    }

    template <bool IsColorable>
    Text<IsColorable>::VectorTuple
        Text<IsColorable>::getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale,
            Matrix2f const& rotation) const noexcept
    {
        return {
            rotation * Vector2f{
                float32(glyph.get().dimensions[0]), 0.f} * scale,
            rotation * Vector2f{0.f,
                float32(glyph.get().dimensions[1])} * scale,
            rotation * vectorCast<float32>(
                glyph.get().bearing) * scale
        };
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceGlyph(
        Texture const& texture,
        Subfont::GlyphRef glyph,
        float32 scale,
        Matrix2f const& rotation)
    {
        auto&& [xVersor, yVersor, bearing] = getGlyphDimensions(
            glyph, scale, rotation);
        if constexpr (IsColorable) {
            glyphs.emplace_back(texture, position + bearing,
                position + bearing + yVersor,
                position + bearing + xVersor + yVersor, color);
        } else {
            glyphs.emplace_back(texture, position + bearing,
                position + bearing + yVersor,
                position + bearing + xVersor + yVersor);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadCharacter(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        uint16 index,
        Matrix2f const& rotation)
    {
        if (auto glyph = subfont(index, level)) {
            if (auto texture = glyph->get().texture)
                emplaceGlyph(*texture, *glyph, scale, rotation);
            Vector2f advance = rotation * Vector2f{
                float32(glyph->get().advance * scale), 0.f};
            position += advance;
            extendModifiers(advance);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        Shadeable::setShader(program);
        shaderExec(*shaderProgram);
        setLocations();
    }

    template <bool IsColorable>
    void Text<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        Shadeable::setShader(std::move(program));
        shaderExec(*shaderProgram);
        setLocations();
    }

    template <bool IsColorable>
    void Text<IsColorable>::setShader(String const& name) {
        Shadeable::setShader(name, shaderExec);
        setLocations();
    }

    template <bool IsColorable>
    void Text<IsColorable>::draw(void) const noexcept {
        shaderProgram->use();
        if constexpr (!IsColorable)
            (*MonochromaticTextBase::colorLoc)(color);
        glyphs.draw();
        underlines.draw();
        strikethroughs.draw();
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::getPosition(void) const noexcept
    {
        if (!glyphs.size())
            return position;
        uint16 index = fromUTF8(text.begin(), std::next(text.begin(),
            getUTF8SequenceLength(text.front())));
        auto&& [level, scale, rotation] = glyphCoefficients();
        auto const& glyph = font(style)(index, level);
        Vector2f bearing = rotation * vectorCast<float32>(
            glyph->get().bearing) * scale;
        return Vector2f{cast::position(glyphs.front()[0])} - bearing;
    }

    template <bool IsColorable>
    Vector2f Text<IsColorable>::intersectionOf(
        Vector2f const& firstPoint,
        Vector2f const& firstVersor,
        Vector2f const& secondPoint,
        Vector2f const& secondVersor) noexcept
    {
        Vector2f pos = *invert(transpose(Matrix2f{firstVersor,
            -secondVersor})) * (secondPoint - firstPoint);
        return firstPoint + firstVersor * pos[0];
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::getDimensions(void) const noexcept
    {
        if (!glyphs.size())
            return {0.f, 0.f};
        Matrix2f rot = rotationMatrix<float>(angle);
        Vector2f intersec = intersectionOf(
            glyphs.front()[1] & cast::position, rot * 1._y,
            glyphs.back()[3] & cast::position, rot * 1._x);
        float x = (Vector2f{glyphs.front()[0] & cast::position}
            + rot * Vector2f{0.f, textSize} - intersec).length();
        float y = (Vector2f{glyphs.back()[3] & cast::position}
            - intersec).length();
        return {y, x};
    }

    template <bool IsColorable>
    Text<IsColorable>&
        Text<IsColorable>::operator+= (String const& left)
    {
        text += left;
        loadGlyphs(parseString(left));
        return *this;
    }

    template <bool IsColorable>
    void Text<IsColorable>::clear(void) noexcept {
        position = getPosition();
        text.clear();
        glyphs.clear();
        if (mods & Modifiers::Underline)
            underlines.clear();
        if (mods & Modifiers::Strikethrough)
            strikethroughs.clear();
    }

    template <bool IsColorable>
    void Text<IsColorable>::reloadGlyphs(void) {
        position = getPosition();
        glyphs.clear();
        loadGlyphs(parseString(text));
    }

    template <bool IsColorable>
    void Text<IsColorable>::setString(String const& text) {
        this->text = text;
        reloadGlyphs();
    }

    template <bool IsColorable>
    Text<IsColorable>& Text<IsColorable>::operator= (
        String const& text)
    {
        setString(text);
        return *this;
    }

    template <bool IsColorable>
    Text<IsColorable>& Text<IsColorable>::operator= (
        String&& text)
    {
        this->text = std::move(text);
        reloadGlyphs();
        return *this;
    }

    template <bool IsColorable>
    void  Text<IsColorable>::setSize(SizeT size) {
        this->textSize = size;
        reloadGlyphs();
    }

    template <bool IsColorable>
    void  Text<IsColorable>::setColor(Color const& color) {
        this->color = color;
        if constexpr(IsColorable)
            for (auto& vcolor : glyphs | std::views::join | views::color)
                    vcolor = color;
        if (mods & Modifiers::Underline)
            for (auto& vcolor : underlines | std::views::join | views::color)
                    vcolor = color;
        if (mods & Modifiers::Strikethrough)
            for (auto& vcolor : strikethroughs | std::views::join | views::color)
                    vcolor = color;
    }

    template <bool IsColorable>
    void Text<IsColorable>::setStyle(Font::Type const& style) {
        this->style = style;
        reloadGlyphs();
    }

    template <bool IsColorable>
    void Text<IsColorable>::setModifiers(Modifiers const& mods) {
        this->mods = mods;
        reloadGlyphs();
    }

    template <bool IsColorable>
    void Text<IsColorable>::setFont(Font const& font) {
        this->font = font;
        reloadGlyphs();
    }

    template <bool IsColorable>
    void Text<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        glyphs.translate(shift);
        if (mods & Modifiers::Underline)
            underlines.translate(shift);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.translate(shift);
        position += shift;
    }

    template <bool IsColorable>
    void Text<IsColorable>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    template <bool IsColorable>
    void Text<IsColorable>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        glyphs.rotate(center, rotation);
        if (mods & Modifiers::Underline)
            underlines.rotate(center, rotation);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.rotate(center, rotation);
        position = rotation * (position - center) + center;
        auto twoPi = std::numbers::pi_v<float32> * 2;
        this->angle += twoPi - angle;
        this->angle = angle > twoPi ? angle - twoPi : angle < 0.f ?
            twoPi + angle : angle;
    }

    template <bool IsColorable>
    void Text<IsColorable>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        glyphs.scale(center, factor);
        if (mods & Modifiers::Underline)
            underlines.scale(center, factor);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.scale(center, factor);
        textSize *= factor;
        position = (position - center) * factor + center;
    }

    template <bool IsColorable>
    void Text<IsColorable>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        glyphs.onScreenTransformation(oldDimensions);
        if (mods & Modifiers::Underline)
            underlines.onScreenTransformation(oldDimensions);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.onScreenTransformation(oldDimensions);
    }

}

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
#include <MPGL/Core/Transformations/Rotation.hpp>
#include <MPGL/Core/Figures/Views.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>
#include <MPGL/Core/Text/Text.hpp>

namespace mpgl {

    MonochromaticTextBase::MonochromaticTextBase(void)
        : colorLoc{new ShaderLocation} {}

    template <bool IsColorable>
    Text<IsColorable>::PositionHolder::PositionHolder(
        Vector2f const& position) noexcept
            : vertices{{
                Adapter2D{position},
                Adapter2D{position + 1._y},
                Adapter2D{position + 1._x + 1._y}
            }}, xVersor{1._x}, yVersor{1._y}, position{position} {}

    template <bool IsColorable>
    void Text<IsColorable>::PositionHolder::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{vertices};
        transformator(positions);
        actualize();
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::PositionHolder::getPosition(
            void) const noexcept
    {
        return position;
    }

    template <bool IsColorable>
    void Text<IsColorable>::PositionHolder::actualize(
        void) noexcept
    {
        position = Vector2f{vertices[0]};
        yVersor = Vector2f{vertices[1]} - position;
        xVersor = Vector2f{vertices[2]} - Vector2f{vertices[1]};
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::PositionHolder::changeSystem(
            Vector2f const& position) const noexcept
    {
        return position[0] * xVersor + position[1] * yVersor;
    }

    template <bool IsColorable>
    Vector2f Text<IsColorable>::PositionHolder::advance(
        Vector2f const& advanceVector) noexcept
    {
        Vector2f const realAdvance = changeSystem(advanceVector);
        for (Adapter2D& vertex : vertices)
            vertex = Vector2f{vertex} + realAdvance;
        actualize();
        return realAdvance;
    }

    template <bool IsColorable>
    [[nodiscard]] Text<IsColorable>::VectorTuple
        Text<IsColorable>::PositionHolder::calculatePositon(
            Vector2f const& bearing,
            float32 width,
            float32 height) const noexcept
    {
        Vector2f const start = position + changeSystem(bearing);
        Vector2f const vWidth = xVersor * width;
        Vector2f const vHeight = yVersor * height;
        return {
            start,
            start + vHeight,
            start + vWidth + vHeight
        };
    }

    template <bool IsColorable>
    [[nodiscard]] Text<IsColorable>::VectorTuple
        Text<IsColorable>::PositionHolder::calculatePositon(
            float32 midspan,
            float32 halfspan) const noexcept
    {
        Vector2f const vMid = yVersor * midspan;
        Vector2f const vHalf = yVersor * halfspan;
        Vector2f const start = position + vMid - vHalf;
        return {
            start,
            start + 2.f * vHalf,
            start + 2.f * vHalf
        };
    }

    template <bool IsColorable>
    [[nodiscard]] Text<IsColorable>::VectorTuple
        Text<IsColorable>::PositionHolder::calculatePositon(
            float32 span) const noexcept
    {
        return {
            position - yVersor * span,
            position,
            position
        };
    }

    template <bool IsColorable>
    void Text<IsColorable>::PositionHolder::move(
        Vector2f const& point) noexcept
    {
        Vector2f const shift = point - position;
        for (Adapter2D& vertex : vertices)
            vertex = Vector2f{vertex} + shift;
        actualize();
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::PositionHolder::findOrigin(
            Vector2f const& position,
            Vector2f const& bearing) const noexcept
    {
        // versors cannot be parallel
        return *intersectionOf(position - xVersor * bearing, yVersor,
            this->position, xVersor);
    }

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
        PositionHolder const& positionSpace,
        SizeT textSize,
        Color const& color) noexcept
    {
        float span = 2 * std::ceil(0.05f * textSize);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(span);
        return Tetragon{firstVertex, secondVertex, thirdVertex, color};
    }

    template <bool IsColorable>
    Tetragon Text<IsColorable>::generateStrikethrough(
        PositionHolder const& positionSpace,
        SizeT textSize,
        Color const& color) noexcept
    {
        float midspan = std::floor(textSize / 2.5f);
        float halfspan = std::ceil(0.05f * textSize);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(midspan, halfspan);
        return Tetragon{firstVertex, secondVertex, thirdVertex, color};
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
            shaderExec}, positionSpace{position}, text{text},
            font{font}, underlines{
                generateUnderline(positionSpace, options.size,
                options.color)},
            strikethroughs{
                generateStrikethrough(positionSpace, options.size,
                options.color)},
            color{options.color}, textSize{options.size},
            style{options.style}, mods{options.mods}
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
        Vector2f const& advance) noexcept
    {
        if (mods & Modifiers::Underline)
            extendUnderline(advance);
        if (mods & Modifiers::Strikethrough)
            extendStrikethrough(advance);
    }

    template <bool IsColorable>
    void Text<IsColorable>::extendUnderline(
        Vector2f const& advance) noexcept
    {
        underlines.back()[2] | cast::position = advance
            + Vector2f{underlines.back()[2] | cast::position};
        underlines.back()[3] | cast::position = advance
            + Vector2f{underlines.back()[3] | cast::position};
    }

    template <bool IsColorable>
    void Text<IsColorable>::extendStrikethrough(
        Vector2f const& advance) noexcept
    {
        strikethroughs.back()[2] | cast::position = advance
            + Vector2f{strikethroughs.back()[2] | cast::position};
        strikethroughs.back()[3] | cast::position = advance
            + Vector2f{strikethroughs.back()[3] | cast::position};
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceUnderline(void) {
        underlines.push_back(generateUnderline(
            positionSpace, textSize, color));
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceStrikethrough(void) {
        strikethroughs.push_back(generateStrikethrough(
           positionSpace, textSize, color));
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
        return {level, (float32) textSize / (ShiftBase << level)};
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadGlyphs(IDArray const& indexes) {
        auto& subfont = font(style);
        auto&& [level, scale] = glyphCoefficients();
        for (uint16 const& index : indexes)
            loadGlyph(subfont, level, scale, index);
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadGlyph(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        uint16 index)
    {
        switch (index) {
            case Newline:
                return loadNewline(subfont, level, scale);
            case Tabulator:
                return loadTab(subfont, level, scale);
            default:
                return loadCharacter(subfont, level, scale, index);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadTab(
        Subfont& subfont,
        uint8 level,
        float32 scale)
    {
        /// tab is 4 times longer than space
        if (auto glyph = subfont(32, level))
            extendModifiers(positionSpace.advance({
                4.f * float32(glyph->get().advance * scale), 0.f}));
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadNewline(
        Subfont& subfont,
        uint8 level,
        float32 scale)
    {
        positionSpace.move(getPosition());
        positionSpace.advance({0.f, -1.1f * textSize});
        emplaceModifiers();
    }

    template <bool IsColorable>
    Text<IsColorable>::GlyphDimensions
        Text<IsColorable>::getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale) const noexcept
    {
        return {
            float32(glyph.get().dimensions[0]) * scale,
            float32(glyph.get().dimensions[1]) * scale,
            vectorCast<float32>(glyph.get().bearing) * scale
        };
    }

    template <bool IsColorable>
    void Text<IsColorable>::emplaceGlyph(
        Texture const& texture,
        Subfont::GlyphRef glyph,
        float32 scale)
    {
        auto&& [width, height, bearing] = getGlyphDimensions(
            glyph, scale);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(bearing, width, height);
        if constexpr (IsColorable) {
            glyphs.emplace_back(texture, firstVertex, secondVertex,
                thirdVertex, color);
        } else {
            glyphs.emplace_back(texture, firstVertex, secondVertex,
                thirdVertex);
        }
    }

    template <bool IsColorable>
    void Text<IsColorable>::loadCharacter(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        uint16 index)
    {
        if (auto glyph = subfont(index, level)) {
            if (auto texture = glyph->get().texture)
                emplaceGlyph(*texture, *glyph, scale);
            extendModifiers(positionSpace.advance({
                float32(glyph->get().advance * scale), 0.f}));
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
    Text<IsColorable>::GlyphPosPair
        Text<IsColorable>::findFirstGlyphBearing(void) const noexcept
    {
        auto&& [level, scale] = glyphCoefficients();
        std::size_t id = 0;
        for (auto iter = text.begin(); iter < text.end(); ++id) {
            std::size_t seqLen = getUTF8SequenceLength(*iter);
            uint16 index = fromUTF8(iter, std::next(iter, seqLen));
            if (auto glyph = font(style)(index, level))
                return {std::get<2>(getGlyphDimensions(*glyph, scale)),
                    id};
            std::advance(iter, seqLen);
        }
        return {};
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        Text<IsColorable>::getPosition(void) const noexcept
    {
        if (!glyphs.size())
            return positionSpace.getPosition();
        auto&& [bearing, id] = findFirstGlyphBearing();
        return positionSpace.findOrigin({glyphs.front()[id]
            | cast::position}, bearing);
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
        positionSpace.move(getPosition());
        text.clear();
        glyphs.clear();
        if (mods & Modifiers::Underline)
            underlines.clear();
        if (mods & Modifiers::Strikethrough)
            strikethroughs.clear();
    }

    template <bool IsColorable>
    void Text<IsColorable>::reloadGlyphs(void) {
        positionSpace.move(getPosition());
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
            setColorOnJoinableRange(glyphs, color);
        if (mods & Modifiers::Underline)
            setColorOnJoinableRange(underlines, color);
        if (mods & Modifiers::Strikethrough)
            setColorOnJoinableRange(strikethroughs, color);
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
    void Text<IsColorable>::transform(
        Transformation2D const& transformator) noexcept
    {
        glyphs.transform(transformator);
        if (mods & Modifiers::Underline)
            underlines.transform(transformator);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.transform(transformator);
        positionSpace.transform(transformator);
    }

}

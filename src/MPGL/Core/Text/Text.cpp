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
#include <MPGL/Core/Transformations/Rotation.hpp>
#include <MPGL/Core/Figures/Views.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>
#include <MPGL/Core/Text/Text.hpp>

namespace mpgl {

    template <Dimension Dim>
    Text<Dim>::PositionHolder::PositionHolder(
        Vector const& position) noexcept
            : vertices{{
                Adapter{position},
                Adapter{position + static_cast<Vector>(1._y)},
                Adapter{position + static_cast<Vector>(1._x + 1._y)}
            }},
            xVersor{static_cast<Vector>(1._x)},
            yVersor{static_cast<Vector>(1._y)},
            position{position} {}

    template <Dimension Dim>
    void Text<Dim>::PositionHolder::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<Adapter> positions{vertices};
        transformator(positions);
        actualize();
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::Vector
        Text<Dim>::PositionHolder::getPosition(
            void) const noexcept
    {
        return position;
    }

    template <Dimension Dim>
    void Text<Dim>::PositionHolder::actualize(
        void) noexcept
    {
        position = static_cast<Vector>(vertices[0]);
        yVersor = static_cast<Vector>(vertices[1]) - position;
        xVersor = static_cast<Vector>(vertices[2]) - static_cast<Vector>(vertices[1]);
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::Vector
        Text<Dim>::PositionHolder::changeSystem(
            Vector2f const& position) const noexcept
    {
        return position[0] * xVersor + position[1] * yVersor;
    }

    template <Dimension Dim>
    Text<Dim>::Vector Text<Dim>::PositionHolder::advance(
        Vector2f const& advanceVector) noexcept
    {
        Vector const realAdvance = changeSystem(advanceVector);
        for (Adapter& vertex : vertices)
            vertex = static_cast<Vector>(vertex) + realAdvance;
        actualize();
        return realAdvance;
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::VectorTuple
        Text<Dim>::PositionHolder::calculatePositon(
            Vector2f const& bearing,
            float32 width,
            float32 height) const noexcept
    {
        Vector const start = position + changeSystem(bearing);
        Vector const vWidth = xVersor * width;
        Vector const vHeight = yVersor * height;
        return {
            start,
            start + vHeight,
            start + vWidth + vHeight
        };
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::VectorTuple
        Text<Dim>::PositionHolder::calculatePositon(
            float32 midspan,
            float32 halfspan) const noexcept
    {
        Vector const vMid = yVersor * midspan;
        Vector const vHalf = yVersor * halfspan;
        Vector const start = position + vMid - vHalf;
        return {
            start,
            start + 2.f * vHalf,
            start + 2.f * vHalf
        };
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::VectorTuple
        Text<Dim>::PositionHolder::calculatePositon(
            float32 span) const noexcept
    {
        return {
            position - yVersor * span,
            position,
            position
        };
    }

    template <Dimension Dim>
    void Text<Dim>::PositionHolder::move(
        Vector const& point) noexcept
    {
        Vector const shift = point - position;
        for (Adapter& vertex : vertices)
            vertex = static_cast<Vector>(vertex) + shift;
        actualize();
    }

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::Vector
        Text<Dim>::PositionHolder::findOrigin(
            Vector const& position,
            Vector2f const& bearing) const noexcept
    {
        // versors cannot be parallel
        return *intersectionOf(position - changeSystem(bearing), yVersor,
            this->position, xVersor);
    }

    template <Dimension Dim>
    Text<Dim>::String const
        Text<Dim>::shaderType(void)
    {
        if constexpr (TwoDimensional<Dim>)
            return "MPGL/2D/Glyph";
        else
            return "MPGL/3D/Glyph";
    }

    template <Dimension Dim>
    Tetragon<Dim> Text<Dim>::generateUnderline(
        PositionHolder const& positionSpace,
        float32 textSize,
        Color const& color) noexcept
    {
        float span = 2 * std::ceil(0.05f * textSize);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(span);
        return Tetragon<Dim>{
            firstVertex, secondVertex, thirdVertex, color};
    }

    template <Dimension Dim>
    Tetragon<Dim> Text<Dim>::generateStrikethrough(
        PositionHolder const& positionSpace,
        float32 textSize,
        Color const& color) noexcept
    {
        float midspan = std::floor(textSize / 2.5f);
        float halfspan = std::ceil(0.05f * textSize);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(midspan, halfspan);
        return Tetragon<Dim>{
            firstVertex, secondVertex, thirdVertex, color};
    }

    template <Dimension Dim>
    void Text<Dim>::setLocations(void) {
        ShaderLocation{*shaderProgram, "tex"}(0);
        if constexpr (ThreeDimensional<Dim>) {
            this->locationSetterBuilder(
                shaderProgram,
                this->locations);
        }
    }

    template <Dimension Dim>
    Text<Dim>::Text(
        Font const& font,
        Vector const& position,
        String const& text,
        TextOptions const& options) : Shadeable{shaderType()},
            positionSpace{position}, text{text},
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

    template <Dimension Dim>
    Text<Dim>::IDArray Text<Dim>::parseString(String const& string) {
        auto iter = string.begin();
        IDArray array;
        while (iter < string.end()) {
            uint8 length = getUTF8SequenceLength(*iter);
            array.push_back(fromUTF8(iter, std::next(iter, length)));
            std::advance(iter, length);
        }
        return array;
    }

    template <Dimension Dim>
    void Text<Dim>::extendModifiers(Vector const& advance) noexcept {
        if (mods & Modifiers::Underline)
            extendUnderline(advance);
        if (mods & Modifiers::Strikethrough)
            extendStrikethrough(advance);
    }

    template <Dimension Dim>
    void Text<Dim>::extendUnderline(
        Vector const& advance) noexcept
    {
        underlines.back()[2] | cast::position = advance
            + static_cast<Vector>(underlines.back()[2] | cast::position);
        underlines.back()[3] | cast::position = advance
            + static_cast<Vector>(underlines.back()[3] | cast::position);
    }

    template <Dimension Dim>
    void Text<Dim>::extendStrikethrough(
        Vector const& advance) noexcept
    {
        strikethroughs.back()[2] | cast::position = advance
            + static_cast<Vector>(strikethroughs.back()[2] | cast::position);
        strikethroughs.back()[3] | cast::position = advance
            + static_cast<Vector>(strikethroughs.back()[3] | cast::position);
    }

    template <Dimension Dim>
    void Text<Dim>::emplaceUnderline(void) {
        underlines.push_back(generateUnderline(
            positionSpace, textSize, color));
    }

    template <Dimension Dim>
    void Text<Dim>::emplaceStrikethrough(void) {
        strikethroughs.push_back(generateStrikethrough(
           positionSpace, textSize, color));
    }

    template <Dimension Dim>
    void Text<Dim>::emplaceModifiers(void) {
        if (mods & Modifiers::Underline)
            emplaceUnderline();
        if (mods & Modifiers::Strikethrough)
            emplaceStrikethrough();
    }

    template <Dimension Dim>
    uint8 Text<Dim>::getLevel(void) const noexcept {
        if (textSize < 1.f)
            return 0;
        uint8 level = std::ceil(log2(textSize));
        return level > ShiftValue ? level - ShiftValue : 0;
    }

    template <Dimension Dim>
    Text<Dim>::ArgTuple
        Text<Dim>::glyphCoefficients(void) const noexcept
    {
        uint8 level = getLevel();
        return {level, (float32) textSize / (ShiftBase << level)};
    }

    template <Dimension Dim>
    void Text<Dim>::loadGlyphs(IDArray const& indices) {
        auto& subfont = font(style);
        auto&& [level, scale] = glyphCoefficients();
        for (uint16 const& index : indices)
            loadGlyph(subfont, level, scale, index);
    }

    template <Dimension Dim>
    void Text<Dim>::loadGlyph(
        Subfont& subfont,
        uint8 level,
        float32 scale,
        uint16 index)
    {
        switch (index) {
            case Newline:
                return loadNewline();
            case Tabulator:
                return loadTab(subfont, level, scale);
            default:
                return loadCharacter(subfont, level, scale, index);
        }
    }

    template <Dimension Dim>
    void Text<Dim>::loadTab(
        Subfont& subfont,
        uint8 level,
        float32 scale)
    {
        /// tab is 4 times longer than space
        if (auto glyph = subfont(32, level))
            extendModifiers(positionSpace.advance({
                4.f * float32(glyph->get().advance * scale), 0.f}));
    }

    template <Dimension Dim>
    void Text<Dim>::loadNewline(void){
        positionSpace.move(getPosition());
        positionSpace.advance({0.f, -1.1f * textSize});
        emplaceModifiers();
    }

    template <Dimension Dim>
    Text<Dim>::GlyphDimensions
        Text<Dim>::getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale) const noexcept
    {
        return {
            float32(glyph.get().dimensions[0]) * scale,
            float32(glyph.get().dimensions[1]) * scale,
            vectorCast<float32>(glyph.get().bearing) * scale
        };
    }

    template <Dimension Dim>
    void Text<Dim>::emplaceGlyph(
        Texture const& texture,
        Subfont::GlyphRef glyph,
        float32 scale)
    {
        auto&& [width, height, bearing] = getGlyphDimensions(
            glyph, scale);
        auto const& [firstVertex, secondVertex, thirdVertex]
            = positionSpace.calculatePositon(bearing, width, height);
        glyphs.emplace_back(texture, firstVertex, secondVertex,
            thirdVertex, color);
    }

    template <Dimension Dim>
    void Text<Dim>::loadCharacter(
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

    template <Dimension Dim>
    void Text<Dim>::setShader(ShaderProgram const& program) noexcept {
        Shadeable::setShader(program);
        setLocations();
    }

    template <Dimension Dim>
    void Text<Dim>::setShader(ShaderProgram&& program) noexcept {
        Shadeable::setShader(std::move(program));
        setLocations();
    }

    template <Dimension Dim>
    void Text<Dim>::setShader(String const& name) {
        Shadeable::setShader(name);
        setLocations();
    }

    template <Dimension Dim>
    void Text<Dim>::draw(void) const noexcept {
        shaderProgram->use();
        if constexpr (ThreeDimensional<Dim>)
            this->actualizeLocations();
        glyphs.draw();
        underlines.draw();
        strikethroughs.draw();
    }

    template <Dimension Dim>
    Text<Dim>::GlyphPosPair
        Text<Dim>::findFirstGlyphBearing(void) const noexcept
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

    template <Dimension Dim>
    [[nodiscard]] Text<Dim>::Vector
        Text<Dim>::getPosition(void) const noexcept
    {
        if (!glyphs.size())
            return positionSpace.getPosition();
        auto&& [bearing, id] = findFirstGlyphBearing();
        return positionSpace.findOrigin(static_cast<Vector>(
            glyphs.front()[id] | cast::position), bearing);
    }

    template <Dimension Dim>
    Text<Dim>& Text<Dim>::operator+= (String const& left) {
        text += left;
        loadGlyphs(parseString(left));
        return *this;
    }

    template <Dimension Dim>
    void Text<Dim>::clear(void) noexcept {
        positionSpace.move(getPosition());
        text.clear();
        glyphs.clear();
        if (mods & Modifiers::Underline)
            underlines.clear();
        if (mods & Modifiers::Strikethrough)
            strikethroughs.clear();
    }

    template <Dimension Dim>
    void Text<Dim>::reloadGlyphs(void) {
        positionSpace.move(getPosition());
        glyphs.clear();
        loadGlyphs(parseString(text));
    }

    template <Dimension Dim>
    void Text<Dim>::setString(String const& text) {
        this->text = text;
        reloadGlyphs();
    }

    template <Dimension Dim>
    Text<Dim>& Text<Dim>::operator= (String const& text) {
        setString(text);
        return *this;
    }

    template <Dimension Dim>
    Text<Dim>& Text<Dim>::operator= (String&& text) {
        this->text = std::move(text);
        reloadGlyphs();
        return *this;
    }

    template <Dimension Dim>
    void  Text<Dim>::setSize(float32 size) {
        this->textSize = size;
        reloadGlyphs();
    }

    template <Dimension Dim>
    void  Text<Dim>::setColor(Color const& color) {
        this->color = color;
        setColorOnJoinableRange(glyphs, color);
        if (mods & Modifiers::Underline)
            setColorOnJoinableRange(underlines, color);
        if (mods & Modifiers::Strikethrough)
            setColorOnJoinableRange(strikethroughs, color);
    }

    template <Dimension Dim>
    void Text<Dim>::setStyle(Font::Type const& style) {
        this->style = style;
        reloadGlyphs();
    }

    template <Dimension Dim>
    void Text<Dim>::setModifiers(Modifiers const& mods) {
        this->mods = mods;
        reloadGlyphs();
    }

    template <Dimension Dim>
    void Text<Dim>::setFont(Font const& font) {
        this->font = font;
        reloadGlyphs();
    }

    template <Dimension Dim>
    void Text<Dim>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        glyphs.transform(transformator);
        if (mods & Modifiers::Underline)
            underlines.transform(transformator);
        if (mods & Modifiers::Strikethrough)
            strikethroughs.transform(transformator);
        positionSpace.transform(transformator);
    }

}

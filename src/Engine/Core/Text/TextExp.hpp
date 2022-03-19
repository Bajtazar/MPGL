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
#pragma once

#include "Font.hpp"
#include "GlyphSprite.hpp"
#include "../DrawableCollection.hpp"
#include "../Shaders/Shadeable.hpp"
#include "../Figures/Primitives/Tetragon.hpp"

namespace mpgl::exp {

    template <bool IsColorable = false>
    class Text : public Shadeable, public Transformable2D {
    public:
        typedef GlyphSprite<IsColorable>            FontGlyph;
        typedef DrawableCollection<FontGlyph>       GlyphsVector;
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;
        typedef std::string                         String;

        enum class Modifiers : uint8 {
            None                                  = 0x00,
            Underline                             = 0x01,
            Strikethrough                         = 0x02,
            UnderlineAndStrikethrough             = 0x03
        };

        struct Options {
            SizeT                                   size = 18;
            Color                                   color = Color{};
            Style                                   style = Style::Regular;
            Modifiers                               mods = Modifiers::None;
            float32                                 angle = 0.f;
        };

        explicit Text(
            Font const& font = {},
            Vector2f const& position = {},
            String const& text = {},
            Options const& options = {});

        Text(Text const& text) = default;
        Text(Text&& text) = default;

        Text& operator= (Text const& text) = default;
        Text& operator= (Text&& text) = default;

        Text& operator= (String const& text);
        Text& operator= (String&& text);

        [[nodiscard]] operator String() const& noexcept
            { return text; }

        void setFont(Font const& font);
        void setStyle(Style const& style);
        void setColor(Color const& color);
        void setModifiers(Modifiers const& mods);
        void setSize(SizeT size);

        [[nodiscard]] Font const& getFont(void) const noexcept
            { return font; }
        [[nodiscard]] Style const& getStyle(void) const noexcept
            { return style; }
        [[nodiscard]] Color const& getColor(void) const noexcept
            { return color; }
        [[nodiscard]] SizeT const& getTextSize(void) const noexcept
            { return textSize; }
        [[nodiscard]] Modifiers const& getModifiers(void) const noexcept
            { return mods; }

        void clear(void) noexcept;

        Text& operator+= (String const& left);

        void setString(String const& text);

        [[nodiscard]] String const& getString(
            void) const noexcept
                { return text; }

        [[nodiscard]] Vector2f getDimensions(void) const noexcept;
        [[nodiscard]] Vector2f getPosition(void) const noexcept;
        [[nodiscard]] float32 getAngle(void) const noexcept
            { return angle; }

        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(
            Vector2f const& center,
            float32 factor) noexcept final;
        void rotate(
            Vector2f const& center,
            float32 angle) noexcept final;
        void rotate(
            Vector2f const& center,
            Matrix2f const& rotation) noexcept final;

        void setShader(
            ShaderProgram const& program) noexcept final override;
        void setShader(
            ShaderProgram&& program) noexcept final override;
        void setShader(String const& name) final override
            { Shadeable::setShader(name, shaderExec); }

        [[nodiscard]] SizeT size(void) const noexcept
            { return glyphs.size(); }

        void draw(void) const noexcept final;

        ~Text(void) noexcept = default;
    private:
        typedef std::vector<uint16>                 IDArray;
        typedef DrawableCollection<Tetragon>        Lines;
        typedef std::tuple<uint8, float32,
            Matrix2f>                               ArgTuple;
        typedef std::tuple<Vector2f, Vector2f,
            Vector2f>                               VectorTuple;
        typedef typename ShadersContext::ProgramPtr ProgramPtr;
        typedef typename ShadersContext::Executable Executable;

        static constexpr SizeT                      ShiftBase
            = Subfont::shiftBase;
        static constexpr SizeT                      ShiftValue
            = log2N<SizeT, ShiftBase>();

        String                                      text;
        GlyphsVector                                glyphs;
        Font                                        font;
        Lines                                       underlines;
        Lines                                       strikethroughs;
        Color                                       color;
        Vector2f                                    position;
        SizeT                                       textSize;
        float32                                     angle;
        Style                                       style;
        Modifiers                                   mods;

        static const Executable                     shaderExec;

        void loadGlyphs(IDArray const& array);

        void loadGlyph(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index,
            Matrix2f const& rotation);

        void loadCharacter(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index,
            Matrix2f const& rotation);

        void loadTab(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            Matrix2f const& rotation);

        void loadNewline(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            Matrix2f const& rotation);

        VectorTuple getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale,
            Matrix2f const& rotation) const noexcept;

        void emplaceGlyph(
            Texture const& texture,
            Subfont::GlyphRef glpyh,
            float32 scale,
            Matrix2f const& rotation);

        void extendModifiers(
            Vector2f advance) noexcept;

        void extendUnderline(
            Vector2f advance) noexcept;

        void extendStrikethrough(
            Vector2f advance) noexcept;

        void emplaceModifiers(void);

        void emplaceUnderline(void);

        void emplaceStrikethrough(void);

        uint8 getLevel(void) const noexcept;

        ArgTuple glyphCoefficients(void) const noexcept;

        void reloadGlyphs(void);

        static IDArray parseString(String const& string);

        static Vector2f intersectionOf(
            Vector2f const& firstPoint,
            Vector2f const& firstVersor,
            Vector2f const& secondPoint,
            Vector2f const& secondVersor) noexcept;

        static String const shaderType(void);

        static uint8 maskTextMods(
            Modifiers const& left,
            Modifiers const& right) noexcept;

        static Tetragon generateUnderline(
            Vector2f position,
            float32 angle,
            SizeT textSize,
            Color const& color) noexcept;

        static Tetragon generateStrikethrough(
            Vector2f position,
            float32 angle,
            SizeT textSize,
            Color const& color) noexcept;
    };

    template class Text<true>;
    template class Text<false>;

}

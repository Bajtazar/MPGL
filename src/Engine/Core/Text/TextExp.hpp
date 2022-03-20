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
#include "../Shaders/Shadeable.hpp"
#include "../DrawableCollection.hpp"
#include "../Context/Buffers/VertexCast.hpp"
#include "../Figures/Primitives/Tetragon.hpp"

namespace mpgl::exp {

    template <bool IsConst>
    class TextGlyphView {
    public:
        typedef std::conditional_t<IsConst,
            GlyphSprite<true> const,
            GlyphSprite<true>>                      GlyphType;
        typedef std::reference_wrapper<GlyphType>   GlyphRef;
        typedef std::size_t                         SizeT;
        typedef std::conditional_t<IsConst,
            Color const, Color>                     value_type;
        typedef value_type&                         reference;
        typedef Color const&                        const_reference;

        explicit TextGlyphView(
            GlyphRef const& ref) noexcept
                : ref{ref} {}

        [[nodiscard]] reference operator[] (
            SizeT index) noexcept requires (!IsConst)
                { return ref.get()[index] & cast::color; }

        [[nodiscard]] const_reference operator[] (
            SizeT index) const noexcept
                { return ref.get()[index] & cast::color; }

        template <class BaseTp, std::random_access_iterator Iter>
        class Iterator : public std::iterator<
            std::random_access_iterator_tag, BaseTp>
        {
        public:
            typedef BaseTp                          value_type;
            typedef value_type*                     pointer;
            typedef value_type&                     reference;
            typedef std::ptrdiff_t                  difference_type;

            using compare =
                std::compare_three_way_result_t<Iter, Iter>;

            using iterator_category =
                std::random_access_iterator_tag;

            explicit Iterator(void) noexcept = default;
            explicit Iterator(Iter const& iter) noexcept
                : iter{iter} {}

            Iterator& operator++(void) noexcept
                { ++iter; return *this; }
            [[nodiscard]] Iterator operator++(int) noexcept
                { auto temp = *this; ++iter; return temp; }

            Iterator& operator--(void) noexcept
                { --iter; return *this; }
            [[nodiscard]] Iterator operator--(int) noexcept
                { auto temp = *this; --iter; return temp; }

            [[nodiscard]] reference operator*(void) const noexcept
                { return *iter & cast::color; }
            [[nodiscard]] pointer operator->(void) const noexcept
                { return &(*iter & cast::color); }

            Iterator& operator+= (difference_type offset) noexcept
                { iter += offset; return *this; };

            Iterator& operator-= (difference_type offset) noexcept
                { iter -= offset; return *this; };

            [[nodiscard]] reference operator[] (
                difference_type offset) noexcept
                    { return *(iter + offset); }

            [[nodiscard]] friend Iterator operator+(
                Iterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp.iter += right; return temp; }

            [[nodiscard]] friend Iterator operator+(
                difference_type left,
                Iterator const& right) noexcept
            { auto temp = right; temp.iter += left; return temp; }

            [[nodiscard]] friend Iterator operator-(
                Iterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp.iter -= right; return temp; }

            [[nodiscard]] friend difference_type operator-(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter - right.iter; }

            [[nodiscard]] friend bool operator==(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter == right.iter; }

            [[nodiscard]] friend compare operator<=>(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter <=> right.iter; }
        private:
            Iter                                    iter;
        };

        using iterator = Iterator<Color, typename
            GlyphSprite<true>::iterator>;
        using const_iterator = Iterator<Color const, typename
            GlyphSprite<true>::const_iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<
            const_iterator>;

        [[nodiscard]] iterator begin(void) noexcept requires (!IsConst)
            { return iterator{ ref.get().begin()}; }

        [[nodiscard]] const_iterator
            begin(void) const noexcept
                { return const_iterator{ ref.get().begin()}; }

        [[nodiscard]] iterator end(void) noexcept requires (!IsConst)
            { return iterator{ ref.get().end()}; }

        [[nodiscard]] const_iterator end(void) const noexcept
            { return const_iterator{ ref.get().end()}; }

        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return const_iterator{ ref.get().cbegin()}; }

        [[nodiscard]] const_iterator cend(void) const noexcept
            { return const_iterator{ ref.get().cend()}; }

        [[nodiscard]] reverse_iterator
            rbegin(void) noexcept requires (!IsConst)
                { return std::reverse_iterator{ end() - 1}; }

        [[nodiscard]] reverse_iterator
            rend(void) noexcept requires (!IsConst)
                { return std::reverse_iterator{ begin() - 1}; }

        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return std::reverse_iterator{ end() - 1}; }

        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return std::reverse_iterator{ begin() - 1}; }

        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return std::reverse_iterator{ end() - 1}; }

        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return std::reverse_iterator{ begin() - 1}; }
    private:
        GlyphRef                                    ref;
    };

    template class TextGlyphView<false>;
    template class TextGlyphView<true>;

    template <bool IsConst>
    class ColorableTextIterator {
    public:
        typedef GlyphSprite<true>                   FontGlyph;
        typedef DrawableCollection<FontGlyph>       GlyphsVector;
        typedef std::conditional_t<IsConst,
            typename GlyphsVector::const_iterator,
            typename GlyphsVector::iterator>        GlyphIter;
        typedef TextGlyphView<IsConst>              value_type;
        typedef std::ptrdiff_t                      difference_type;

        using compare =
            std::compare_three_way_result_t<GlyphIter, GlyphIter>;

        explicit ColorableTextIterator(
            GlyphIter const& iter) noexcept
                : iter{iter} {}

        ColorableTextIterator& operator++(void) noexcept
            { ++iter; return *this; }

        [[nodiscard]] ColorableTextIterator operator++(int) noexcept
            { auto temp = *this; ++iter; return temp; }

        ColorableTextIterator& operator--(void) noexcept
            { --iter; return *this; }

        [[nodiscard]] ColorableTextIterator operator--(int) noexcept
            { auto temp = *this; --iter; return temp; }

        [[nodiscard]] value_type operator* (void) const noexcept
            { return value_type{std::ref(*iter)}; }

        ColorableTextIterator& operator+= (
            difference_type offset) noexcept
                { iter += offset; return *this; };

        ColorableTextIterator& operator-= (
            difference_type offset) noexcept
                { iter -= offset; return *this; };

        [[nodiscard]] value_type operator[] (
            difference_type offset) noexcept
                { return value_type{std::ref(*(iter + offset))}; }

        [[nodiscard]] friend ColorableTextIterator operator+(
            ColorableTextIterator const& left,
            difference_type right) noexcept
                { auto temp = left; temp.iter += right; return temp; }

        [[nodiscard]] friend ColorableTextIterator operator+(
            difference_type left,
            ColorableTextIterator const& right) noexcept
                { auto temp = right; temp.iter += left; return temp; }

        [[nodiscard]] friend ColorableTextIterator operator-(
            ColorableTextIterator const& left,
            difference_type right) noexcept
                { auto temp = left; temp.iter -= right; return temp; }

        [[nodiscard]] friend difference_type operator-(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter - right.iter; }

        [[nodiscard]] friend bool operator==(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter == right.iter; }

        [[nodiscard]] friend compare operator<=>(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter <=> right.iter; }
    private:
        GlyphIter                                   iter;
    };

    struct TextOptions {
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;

        enum class Modifiers : uint8 {
            None                                  = 0x00,
            Underline                             = 0x01,
            Strikethrough                         = 0x02,
            UnderlineAndStrikethrough             = 0x03
        };

        SizeT                                   size = 18;
        Color                                   color = Color{};
        Style                                   style = Style::Regular;
        Modifiers                               mods = Modifiers::None;
        float32                                 angle = 0.f;
    };

    template <bool IsColorable = false>
    class Text : public Shadeable, public Transformable2D {
    public:
        typedef GlyphSprite<IsColorable>            FontGlyph;
        typedef DrawableCollection<FontGlyph>       GlyphsVector;
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;
        typedef std::string                         String;
        typedef TextOptions::Modifiers              Modifiers;

        explicit Text(
            Font const& font = {},
            Vector2f const& position = {},
            String const& text = {},
            TextOptions const& options = {});

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

        using iterator = ColorableTextIterator<false>;
        using const_iterator = ColorableTextIterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<
            const_iterator>;

        [[nodiscard]] iterator begin(
            void) noexcept requires IsColorable
                { return iterator{glyphs.begin()}; }

        [[nodiscard]] iterator end(
            void) noexcept requires IsColorable
                { return iterator{glyphs.end()}; }

        [[nodiscard]] const_iterator begin(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.begin()}; }

        [[nodiscard]] const_iterator end(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.end()}; }

        [[nodiscard]] const_iterator cbegin(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.begin()}; }

        [[nodiscard]] const_iterator cend(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.end()}; }

        [[nodiscard]] reverse_iterator rbegin(
            void) noexcept requires IsColorable
                { return reverse_iterator{end() - 1}; }

        [[nodiscard]] reverse_iterator rend(
            void) noexcept requires IsColorable
                { return reverse_iterator{begin() - 1}; }

        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{end() - 1}; }

        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{begin() - 1}; }

        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{end() - 1}; }

        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{begin() - 1}; }

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

    constexpr uint8 operator&(
        TextOptions::Modifiers const& left,
        TextOptions::Modifiers const& right) noexcept
    {
        /// change to std::to_underlying in C++23
        return static_cast<uint8>(left) & static_cast<uint8>(right);
    }

}

#pragma once

#include "Font.hpp"
#include "GlyphSprite.hpp"
#include "../Figures/Drawable.hpp"
#include "../Figures/Transformable.hpp"

namespace ge {

    // add colours view

    template <bool isPolichromatic = false>
    class Text : public Drawable, public Transformable {
    public:
        typedef std::shared_ptr<Vector2ui>      ScenePtr;

        explicit Text(ScenePtr const& scene,
            Font& font, std::size_t size,
            std::string const& text = {},
            Vector2f const& position = {},
            Color const& color = {},
            Font::Type const& type = Font::Type::Regular);

        Text(Text const& text) = default;
        Text(Text&& text) = default;

        Text& operator= (Text const& text) = default;
        Text& operator= (Text&& text) = default;

        Text& operator= (std::string const& text);
        Text& operator= (std::string&& text);

        operator std::string() const& noexcept { return text; }

        void setFont(Font& font);
        void setStyle(Font::Type const& type);
        void setString(std::string const& text);
        void setColor(Color const& color);
        void setSize(std::size_t size);

        void clear(void) noexcept;

        Text& operator+= (std::string const& left);

        std::string const& getString(void) const noexcept { return text; }

        Vector2f getDimmensions(void) const noexcept;
        Vector2f getPosition(void) const noexcept;
        float getAngle(void) const noexcept { return angle; }

        void setShaders(ShaderLibrary const& library) noexcept final;
        void copyToGPU(void) noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;

        ~Text(void) noexcept = default;
    private:
        typedef GlyphSprite<!isPolichromatic>       FontGlyph;
        typedef std::vector<FontGlyph>              GlyphsArray;
        typedef std::vector<uint16_t>               IDArray;
        typedef std::reference_wrapper<Font>        FontRef;
        typedef std::tuple<uint8_t, float, Matrix<float, 2>>
                                                    ArgTuple;

        std::string                 text;
        GlyphsArray                 glyphs;
        Color                       color;
        Vector2f                    position;
        std::size_t                 size;
        float                       angle;
        FontRef                     font;
        Font::Type                  type;

        uint8_t getLevel(void) const;

        IDArray parseString(std::string string);

        void drawGlyph(Subfont& subfont, uint8_t level, float scale,
            uint16_t const& index, Matrix<float, 2> const& rotation);

        void drawGlyphs(IDArray const& array);

        void redrawGlyphs(void);

        ArgTuple getArgs(void) const noexcept;
    };

    template class Text<true>;
    template class Text<false>;

}
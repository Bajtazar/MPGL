#pragma once

#include "Font.hpp"
#include "GlyphSprite.hpp"

namespace ge {

    template <bool IsConst>
    class GlyphColorView {
    public:
        typedef std::conditional_t<IsConst, GlyphSprite<false> const,
            GlyphSprite<false>>                             glyph_type;
        typedef std::reference_wrapper<glyph_type>          glyph_ref;
        typedef std::conditional_t<IsConst, Color const,
            Color>                                          value_type;
        typedef value_type&                                 reference;
        typedef Color const&                                const_reference;

        explicit GlyphColorView(glyph_ref const& ref) noexcept : ref{ref} {}

        reference operator[] (std::size_t index) noexcept requires (!IsConst) { return ref.get()[index].color; }
        const_reference operator[] (std::size_t index) const noexcept { return ref.get()[index].color; }

        template <class Base, std::random_access_iterator Iter>
        class Iterator : public std::iterator<
            std::random_access_iterator_tag, Base, void, void, void>
        {
        public:
            typedef Base                value_type;
            typedef value_type*         pointer;
            typedef value_type&         reference;
            typedef std::ptrdiff_t      difference_type;

            explicit Iterator(void) noexcept = default;
            explicit Iterator(Iter const& iter) noexcept : iter{iter} {}

            Iterator&  operator++(void) noexcept { ++iter; return *this; }
            Iterator   operator++(int)  noexcept { auto temp = *this; ++iter; return temp; }

            Iterator&  operator--(void) noexcept { --iter; return *this; }
            Iterator   operator--(int)  noexcept { auto temp = *this; --iter; return temp; }

            reference  operator*(void)  const noexcept { return iter->color; }
            pointer    operator->(void) const noexcept { return &iter->color; }

            Iterator&  operator+= (difference_type offset) noexcept { iter += offset; return *this; }
            Iterator&  operator-= (difference_type offset) noexcept { iter -= offset; return *this; }
            Iterator   operator[] (difference_type offset) noexcept { auto temp = *this; temp += offset; return temp; }

            friend bool operator== (Iterator const& left, Iterator const& right) noexcept { return left.iter == right.iter; }
            friend bool operator!= (Iterator const& left, Iterator const& right) noexcept { return left.iter != right.iter; }
            friend bool operator>  (Iterator const& left, Iterator const& right) noexcept { return left.iter > right.iter; }
            friend bool operator<  (Iterator const& left, Iterator const& right) noexcept { return left.iter < right.iter; }
            friend bool operator>= (Iterator const& left, Iterator const& right) noexcept { return left.iter >= right.iter; }
            friend bool operator<= (Iterator const& left, Iterator const& right) noexcept { return left.iter <= right.iter; }

            friend Iterator operator+ (Iterator const& right, difference_type left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
            friend Iterator operator+ (difference_type right, Iterator const& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
            friend Iterator operator- (Iterator const& right, difference_type left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
            friend difference_type operator- (Iterator const& right, Iterator const& left) noexcept { return right.iter - left.iter; }
        private:
            Iter                        iter;
        };

        using iterator = Iterator<Color, typename GlyphSprite<false>::iterator>;
        using const_iterator = Iterator<Color, typename GlyphSprite<false>::const_iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        iterator begin(void) noexcept requires (!IsConst) { return iterator{ref.get().begin()}; }
        const_iterator begin(void) const noexcept { return const_iterator{ref.get().cbegin()}; }

        iterator end(void) noexcept requires (!IsConst) { return iterator{ref.get().end()}; }
        const_iterator end(void) const noexcept { return const_iterator{ref.get().cend()}; }

        const_iterator cbegin(void) const noexcept { return const_iterator{ref.get().cbegin()}; }
        const_iterator cend(void) const noexcept { return const_iterator{ref.get().cend()}; }

        reverse_iterator rbegin(void) noexcept requires (!IsConst) { return std::reverse_iterator{ end() - 1}; }
        reverse_iterator rend(void) noexcept requires (!IsConst) { return std::reverse_iterator{ begin() - 1}; }

        const_reverse_iterator rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1}; }
        const_reverse_iterator rend(void) const noexcept { return std::reverse_iterator{ begin() - 1}; }

        const_reverse_iterator crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1}; }
        const_reverse_iterator crend(void) const noexcept { return std::reverse_iterator{ begin() - 1}; }
    private:
        glyph_ref                       ref;
    };

    template <bool IsConst>
    class PolichromaticTextIterator {
    public:
        typedef GlyphSprite<false>                          glyph_type;
        typedef std::vector<GlyphSprite<false>>             glyph_array;
        typedef std::conditional_t<IsConst, typename glyph_array::const_iterator,
                typename glyph_array::iterator>             glyph_iter;
        typedef GlyphColorView<IsConst>                     value_type;
        typedef std::ptrdiff_t                              difference_type;
        using iterator = PolichromaticTextIterator;

        explicit PolichromaticTextIterator(glyph_iter const& iter) noexcept : iter{iter} {}

        iterator&  operator++(void) noexcept { ++iter; return *this; }
        iterator   operator++(int)  noexcept { auto temp = *this; ++iter; return temp; }

        iterator&  operator--(void) noexcept { --iter; return *this; }
        iterator   operator--(int)  noexcept { auto temp = *this; --iter; return temp; }

        value_type  operator*(void) const noexcept { return value_type{std::ref(*iter)}; }

        iterator&  operator+= (difference_type offset) noexcept { iter += offset; return *this; }
        iterator&  operator-= (difference_type offset) noexcept { iter -= offset; return *this; }
        iterator   operator[] (difference_type offset) noexcept { auto temp = *this; temp += offset; return temp; }

        friend bool operator== (iterator const& left, iterator const& right) noexcept { return left.iter == right.iter; }
        friend bool operator!= (iterator const& left, iterator const& right) noexcept { return left.iter != right.iter; }
        friend bool operator>  (iterator const& left, iterator const& right) noexcept { return left.iter > right.iter; }
        friend bool operator<  (iterator const& left, iterator const& right) noexcept { return left.iter < right.iter; }
        friend bool operator>= (iterator const& left, iterator const& right) noexcept { return left.iter >= right.iter; }
        friend bool operator<= (iterator const& left, iterator const& right) noexcept { return left.iter <= right.iter; }

        friend iterator operator+ (iterator const& right, difference_type left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
        friend iterator operator+ (difference_type right, iterator const& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
        friend iterator operator- (iterator const& right, difference_type left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
        friend difference_type operator- (iterator const& right, iterator const& left) noexcept { return right.iter - left.iter; }
    private:
        glyph_iter                       iter;
    };

    template <bool isPolichromatic = false>
    class Text : public Drawable, public Transformable2D {
    public:
        typedef std::shared_ptr<Vector2ui>      ScenePtr;

        explicit Text(Font& font, std::size_t size,
            Vector2f const& position = {},
            std::string const& text = {},
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
        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;
        void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept final;

        using iterator = PolichromaticTextIterator<false>;
        using const_iterator = PolichromaticTextIterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        iterator begin(void) noexcept requires isPolichromatic
            { return iterator{glyphs.begin()}; }
        iterator end(void) noexcept requires isPolichromatic
            { return iterator{glyphs.end()}; }

        const_iterator begin(void) const noexcept requires isPolichromatic
            { return const_iterator{glyphs.begin()}; }
        const_iterator end(void) const noexcept requires isPolichromatic
            { return const_iterator{glyphs.end()}; }

        const_iterator cbegin(void) const noexcept requires isPolichromatic
            { return const_iterator{glyphs.begin()}; }
        const_iterator cend(void) const noexcept requires isPolichromatic
            { return const_iterator{glyphs.end()}; }

        reverse_iterator rbegin(void) noexcept requires isPolichromatic
            { return reverse_iterator{end() - 1}; }
        reverse_iterator rend(void) noexcept requires isPolichromatic
            { return reverse_iterator{begin() - 1}; }

        const_reverse_iterator rbegin(void) const noexcept requires isPolichromatic
            { return const_reverse_iterator{end() - 1}; }
        const_reverse_iterator rend(void) const noexcept requires isPolichromatic
            { return const_reverse_iterator{begin() - 1}; }

        const_reverse_iterator crbegin(void) const noexcept requires isPolichromatic
            { return const_reverse_iterator{end() - 1}; }
        const_reverse_iterator crend(void) const noexcept requires isPolichromatic
            { return const_reverse_iterator{begin() - 1}; }

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
        ShaderLibrary const*        library;
        Font::Type                  type;

        uint8_t getLevel(void) const;

        IDArray parseString(std::string string);

        void drawGlyph(Subfont& subfont, uint8_t level, float scale,
            uint16_t const& index, Matrix<float, 2> const& rotation);

        void drawGlyphs(IDArray const& array);

        void redrawGlyphs(void);

        void setShadersIfLibrary(void) noexcept;

        ArgTuple getArgs(void) const noexcept;
    };

    template class Text<true>;
    template class Text<false>;

}

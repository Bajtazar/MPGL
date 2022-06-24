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

#include <MPGL/Core/Figures/Primitives/Tetragon.hpp>
#include <MPGL/Core/Context/Buffers/VertexCast.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Core/DrawableCollection.hpp>
#include <MPGL/Core/Shaders/Shadeable.hpp>
#include <MPGL/Core/Text/GlyphSprite.hpp>
#include <MPGL/Core/Text/Font.hpp>

namespace mpgl {

    /**
     * Colored text glyph view that omits the position of the
     * glyph and allows to get only the color of the glyph's
     * vertices
     *
     * @tparam IsConst if the glyph sprite should be constant
     */
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

        /**
         * Constructs a new Text Glyph View object from a reference
         * wrapper to the underlying glyph sprite
         *
         * @param ref the reference wrapper to the glyph sprite
         */
        explicit TextGlyphView(
            GlyphRef const& ref) noexcept
                : ref{ref} {}

        /**
         * Returns the number of vertices in the glyph sprite
         *
         * @return the number of vertices in the glyph sprite
         */
        [[nodiscard]] SizeT size(void) const noexcept
            { return ref.get().size(); }

        /**
         * Returns a reference to the glyph sprite vertex's color
         *
         * @return the reference to the glyph sprite vertex's color
         */
        [[nodiscard]] reference operator[] (
            SizeT index) noexcept requires (!IsConst)
                { return ref.get()[index] & cast::color; }

        /**
         * Returns a constant reference to the glyph sprite
         * vertex's color
         *
         * @return the constant reference to the glyph sprite
         * vertex's color
         */
        [[nodiscard]] const_reference operator[] (
            SizeT index) const noexcept
                { return ref.get()[index] & cast::color; }

        /**
         * Iterator that returns only the color of the underlying
         * vertex sprite vertices
         *
         * @tparam BaseTp the base type of the operator
         * @tparam Iter the glyph sprite iterator
         */
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

            /**
             * Constructs a new Iterator object
             */
            explicit Iterator(void) noexcept = default;

            /**
             * Constructs a new Iterator object from the given
             * glyph sprite iterator
             *
             * @param iter the glyph sprite iterator
             */
            explicit Iterator(Iter const& iter) noexcept
                : iter{iter} {}

            /**
             * Increments iterator by one
             *
             * @return reference to this object
             */
            Iterator& operator++(void) noexcept
                { ++iter; return *this; }

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] Iterator operator++(int) noexcept
                { auto temp = *this; ++iter; return temp; }

            /**
             * Decrements iterator by one
             *
             * @return reference to this object
             */
            Iterator& operator--(void) noexcept
                { --iter; return *this; }

            /**
             * Post-decrements iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] Iterator operator--(int) noexcept
                { auto temp = *this; --iter; return temp; }

            /**
             * Returns a reference to the glyph sprite vertex's
             * color
             *
             * @return the reference to the glyph sprite vertex's
             * color
             */
            [[nodiscard]] reference operator*(void) const noexcept
                { return *iter & cast::color; }

            /**
             * Returns a pointer to the glyph sprite vertex's
             * color
             *
             * @return the pointer to the glyph sprite vertex's
             * color
             */
            [[nodiscard]] pointer operator->(void) const noexcept
                { return &(*iter & cast::color); }

            /**
             * Increments iterator by the given distance
             *
             * @param offset the incremented distance
             * @return reference to this object
             */
            Iterator& operator+= (difference_type offset) noexcept
                { iter += offset; return *this; };

            /**
             * Decrements iterator by the given distance
             *
             * @param offset the decremented distance
             * @return reference to this object
             */
            Iterator& operator-= (difference_type offset) noexcept
                { iter -= offset; return *this; };

            /**
             * Returns a reference to the color of the vertex shifted
             * by the given offset
             *
             * @param offset the incremented distance
             * @return the reference to the color of the vertex
             * shifted by the given offse
             */
            [[nodiscard]] reference operator[] (
                difference_type offset) noexcept
                    { return *(iter + offset) & cast::color; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted iterator
             */
            [[nodiscard]] friend Iterator operator+(
                Iterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp.iter += right; return temp; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the distance
             * @param right the iterator
             * @return the shifted iterator
             */
            [[nodiscard]] friend Iterator operator+(
                difference_type left,
                Iterator const& right) noexcept
            { auto temp = right; temp.iter += left; return temp; }

            /**
             * Subtracts given distance from iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted operator
             */
            [[nodiscard]] friend Iterator operator-(
                Iterator const& left,
                difference_type right) noexcept
            { auto temp = left; temp.iter -= right; return temp; }

            /**
             * Returns distance between iterators
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return difference_type
             */
            [[nodiscard]] friend difference_type operator-(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter - right.iter; }

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
            [[nodiscard]] friend bool operator==(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter == right.iter; }

            /**
             * Compares two iterators to each other
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return the result of compare
             */
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

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept requires (!IsConst)
            { return iterator{ ref.get().begin()}; }

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept requires (!IsConst)
            { return iterator{ ref.get().end()}; }


        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator
            begin(void) const noexcept
                { return const_iterator{ ref.get().begin()}; }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return const_iterator{ ref.get().end()}; }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return const_iterator{ ref.get().cbegin()}; }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return const_iterator{ ref.get().cend()}; }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator
            rbegin(void) noexcept requires (!IsConst)
                { return std::reverse_iterator{ end() - 1}; }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator
            rend(void) noexcept requires (!IsConst)
                { return std::reverse_iterator{ begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return std::reverse_iterator{ end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return std::reverse_iterator{ begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return std::reverse_iterator{ end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return std::reverse_iterator{ begin() - 1}; }
    private:
        GlyphRef                                    ref;
    };

    template class TextGlyphView<false>;
    template class TextGlyphView<true>;

    /**
     * The colorable text class iterator. Returns a views
     * to the glyph sprites contained in the text
     *
     * @tparam IsConst if the glyph sprite's should be constant
     */
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

        /**
         * Constructs a new Colorable Text Iterator object from
         * a constant reference to the glyph's vector iterator
         *
         * @param iter the constant reference to the glyph's
         * vector iterator
         */
        explicit ColorableTextIterator(
            GlyphIter const& iter) noexcept
                : iter{iter} {}

        /**
         * Increments iterator by one
         *
         * @return reference to this object
         */
        ColorableTextIterator& operator++(void) noexcept
            { ++iter; return *this; }

        /**
         * Post-increments iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] ColorableTextIterator operator++(int) noexcept
            { auto temp = *this; ++iter; return temp; }

        /**
         * Decrements iterator by one
         *
         * @return reference to this object
         */
        ColorableTextIterator& operator--(void) noexcept
            { --iter; return *this; }

        /**
         * Post-decrements iterator by one and returns copy
         * of the object
         *
         * @return the copied object
         */
        [[nodiscard]] ColorableTextIterator operator--(int) noexcept
            { auto temp = *this; --iter; return temp; }

        /**
         * Returns a view to the glyph sprite
         *
         * @return the view to the glyph sprite
         */
        [[nodiscard]] value_type operator* (void) const noexcept
            { return value_type{std::ref(*iter)}; }

        /**
         * Increments iterator by the given distance
         *
         * @param offset the incremented distance
         * @return reference to this object
         */
        ColorableTextIterator& operator+= (
            difference_type offset) noexcept
                { iter += offset; return *this; };

        /**
         * Decrements iterator by the given distance
         *
         * @param offset the decremented distance
         * @return reference to this object
         */
        ColorableTextIterator& operator-= (
            difference_type offset) noexcept
                { iter -= offset; return *this; };

        /**
         * Returns a glyph sprites's view shifted by
         * the given offset
         *
         * @param offset the incremented distance
         * @return the glyph sprites's view shifted by
         * the given offset
         */
        [[nodiscard]] value_type operator[] (
            difference_type offset) noexcept
                { return value_type{std::ref(*(iter + offset))}; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted iterator
         */
        [[nodiscard]] friend ColorableTextIterator operator+(
            ColorableTextIterator const& left,
            difference_type right) noexcept
                { auto temp = left; temp.iter += right; return temp; }

        /**
         * Adds given distance to an iterator
         *
         * @param left the distance
         * @param right the iterator
         * @return the shifted iterator
         */
        [[nodiscard]] friend ColorableTextIterator operator+(
            difference_type left,
            ColorableTextIterator const& right) noexcept
                { auto temp = right; temp.iter += left; return temp; }

        /**
         * Subtracts given distance from iterator
         *
         * @param left the iterator
         * @param right the distance
         * @return the shifted operator
         */
        [[nodiscard]] friend ColorableTextIterator operator-(
            ColorableTextIterator const& left,
            difference_type right) noexcept
                { auto temp = left; temp.iter -= right; return temp; }

        /**
         * Returns distance between iterators
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return difference_type
         */
        [[nodiscard]] friend difference_type operator-(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter - right.iter; }

        /**
         * Checks whether two iterators are equal
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return whether two iterators are equal
         */
        [[nodiscard]] friend bool operator==(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter == right.iter; }

        /**
         * Compares two iterators to each other
         *
         * @param left the left iterator
         * @param right the right iterator
         * @return the result of compare
         */
        [[nodiscard]] friend compare operator<=>(
            ColorableTextIterator const& left,
            ColorableTextIterator const& right) noexcept
                { return left.iter <=> right.iter; }
    private:
        GlyphIter                                   iter;
    };

    /**
     * The text initialization options
     */
    struct TextOptions {
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;

        /**
         * The text modifiers
         */
        enum class Modifiers : uint8 {
            /// No modifiers
            None                                  = 0x00,
            /// Text is underlined
            Underline                             = 0x01,
            /// Text is strikedthrough
            Strikethrough                         = 0x02,
            /// Text is underlined and strikedthrough
            UnderlineAndStrikethrough             = 0x03
        };

        /// The size of the text
        SizeT                                   size = 18;
        /// The color of the text
        Color                                   color = Color{};
        /// The style of the text
        Style                                   style = Style::Regular;
        /// The modifiers of the text
        Modifiers                               mods = Modifiers::None;
        /// The counterclockwise angle between x-axis and text base
        float32                                 angle = 0.f;
    };

    class MonochromaticTextBase {
    protected:
        explicit MonochromaticTextBase(void);

        typedef std::shared_ptr<ShaderLocation>     LocationPtr;

        LocationPtr                                 colorLoc;
    };

    template <bool IsColorable>
    using TextBase = std::conditional_t<IsColorable,
        std::monostate, MonochromaticTextBase>;

    /**
     * Represents a text on the screen
     *
     * @tparam IsColorable if text's glyph sprites vertices should
     * be colorable
     */
    template <bool IsColorable = false>
    class Text : public Shadeable, public Transformable2D,
        private TextBase<IsColorable>
    {
    public:
        typedef GlyphSprite<IsColorable>            FontGlyph;
        typedef DrawableCollection<FontGlyph>       GlyphsVector;
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;
        typedef std::string                         String;
        typedef TextOptions::Modifiers              Modifiers;

        /**
         * Constructs a new Text with given font, position,
         * text string and text options
         *
         * @param font the font used by text
         * @param position the position of the text
         * @param text the text string displayed by this object
         * @param options the text options
         */
        explicit Text(
            Font const& font = {},
            Vector2f const& position = {},
            String const& text = {},
            TextOptions const& options = {});

        Text(Text const& text) = default;
        Text(Text&& text) = default;

        Text& operator= (Text const& text) = default;
        Text& operator= (Text&& text) = default;

        /**
         * Displays the given string information
         *
         * @param text the constant reference to the string
         * @return the reference to this object
         */
        Text& operator= (String const& text);

        /**
         * Displays the given string information
         *
         * @param text the rvalue reference to the string
         * @return the reference to this object
         */
        Text& operator= (String&& text);

        /**
         * Returns a string represented by this object
         *
         * @return the string represented by this object
         */
        [[nodiscard]] operator String() const& noexcept
            { return text; }

        /**
         * Sets the font used by this text object
         *
         * @param font the constant reference to the font object
         */
        void setFont(Font const& font);

        /**
         * Sets the style used by this text object
         *
         * @param style the constant reference to the style object
         */
        void setStyle(Style const& style);

        /**
         * Sets the color used by this text object
         *
         * @param color the constant reference to the color object
         */
        void setColor(Color const& color);

        /**
         * Sets the modifiers used by this text object
         *
         * @param mods the constant reference to the modifiers object
         */
        void setModifiers(Modifiers const& mods);

        /**
         * Sets the text size
         *
         * @param size the new size of the text
         */
        void setSize(SizeT size);

        /**
         * Returns the font used by the text
         *
         * @return the constant reference to the font object
         */
        [[nodiscard]] Font const& getFont(void) const noexcept
            { return font; }

        /**
         * Returns the style of the text
         *
         * @return the constant reference to the style object
         */
        [[nodiscard]] Style const& getStyle(void) const noexcept
            { return style; }

        /**
         * Returns the color of the text glyphs
         *
         * @return the constant reference to the color object
         */
        [[nodiscard]] Color const& getColor(void) const noexcept
            { return color; }

        /**
         * Returns the size of the text glyphs
         *
         * @return the constant reference to the size object
         */
        [[nodiscard]] SizeT const& getTextSize(void) const noexcept
            { return textSize; }

        /**
         * Returns the modifiers of the text glyphs
         *
         * @return the constant reference to the color modifiers
         */
        [[nodiscard]] Modifiers const& getModifiers(void) const noexcept
            { return mods; }

        /**
         * Removes string displayed by the text
         */
        void clear(void) noexcept;

        /**
         * Adds the given string to the text
         *
         * @param left the constant reference to the string
         * @return the reference to this object
         */
        Text& operator+= (String const& left);

        /**
         * Sets the given string
         *
         * @param text the constant reference to the
         * new string object
         */
        void setString(String const& text);

        /**
         * Returns the string handled by the text object
         *
         * @return the constant reference to the string object
         */
        [[nodiscard]] String const& getString(
            void) const noexcept
                { return text; }

        /**
         * Returns the text's dimensions
         *
         * @return the text's dimensions
         */
        [[nodiscard]] Vector2f getDimensions(void) const noexcept;

        /**
         * Returns the text's position
         *
         * @return the text's position
         */
        [[nodiscard]] Vector2f getPosition(void) const noexcept;

        /**
         * Returns a counterclockwise angle between text base
         * and x-axis
         *
         * @return the angle between text base and x-axis
         */
        [[nodiscard]] float32 getAngle(void) const noexcept
            { return angle; }

        /**
         * Transforms the text during the screen
         * transformation event
         *
         * @param oldDimensions the old screen dimensions
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Translates the text by the given shift vector
         *
         * @param shift the shift vector
         */
        void translate(Vector2f const& shift) noexcept final;

        /**
         * Scales the text around given center by
         * the given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        void scale(
            Vector2f const& center,
            float32 factor) noexcept final;

        /**
         * Rotates the text around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        void rotate(
            Vector2f const& center,
            float32 angle) noexcept final;

        /**
         * Rotates the text around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        void rotate(
            Vector2f const& center,
            Matrix2f const& rotation) noexcept final;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram const& program) noexcept final override;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram&& program) noexcept final override;

        /**
         * Sets the given shader program from an internal
         * library
         *
         * @param name the name of the shader program
         */
        void setShader(String const& name) final override
            { Shadeable::setShader(name, shaderExec); }

        /**
         * Returns the size of the text [number of the nonwhite
         * characters]
         *
         * @return the size of the text
         */
        [[nodiscard]] SizeT size(void) const noexcept
            { return glyphs.size(); }

        /**
         * Draws the text on the screen
         */
        void draw(void) const noexcept final;

        using iterator = ColorableTextIterator<false>;
        using const_iterator = ColorableTextIterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<
            const_iterator>;

        /**
         * Returns the iterator to the begining of the glyph sprites
         *
         * @return the iterator to the begining of the glyph sprites
         */
        [[nodiscard]] iterator begin(
            void) noexcept requires IsColorable
                { return iterator{glyphs.begin()}; }

        /**
         * Returns the iterator to the end of the glyph sprites
         *
         * @return the iterator to the end of the glyph sprites
         */
        [[nodiscard]] iterator end(
            void) noexcept requires IsColorable
                { return iterator{glyphs.end()}; }

        /**
         * Returns the constant iterator to the begining
         * of the glyph sprites
         *
         * @return the constant iterator to the begining
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator begin(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.begin()}; }

        /**
         * Returns the constant iterator to the end
         * of the glyph sprites
         *
         * @return the constant iterator to the end
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator end(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.end()}; }

        /**
         * Returns the constant iterator to the begining
         * of the glyph sprites
         *
         * @return the constant iterator to the begining
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator cbegin(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.begin()}; }

        /**
         * Returns the constant iterator to the end
         * of the glyph sprites
         *
         * @return the constant iterator to the end
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator cend(
            void) const noexcept requires IsColorable
                { return const_iterator{glyphs.end()}; }

        /**
         * Returns the reverse iterator to the end of
         * the glyph sprites
         *
         * @return the reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] reverse_iterator rbegin(
            void) noexcept requires IsColorable
                { return reverse_iterator{end() - 1}; }

        /**
         * Returns the reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] reverse_iterator rend(
            void) noexcept requires IsColorable
                { return reverse_iterator{begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept requires IsColorable
                { return const_reverse_iterator{begin() - 1}; }

        /**
         * Destroys the Text object
         */
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

        /**
         * Loads the glyphs into the memory
         *
         * @param array the array with glyphs ids
         */
        void loadGlyphs(IDArray const& array);

        /**
         * Loads the glyph into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         * @param index the glyph index
         * @param rotation the rotation matrix
         */
        void loadGlyph(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index,
            Matrix2f const& rotation);

        /**
         * Loads the nonwhite character into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         * @param index the glyph index
         * @param rotation the rotation matrix
         */
        void loadCharacter(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index,
            Matrix2f const& rotation);

        /**
         * Loads the tabulator into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         * @param rotation the rotation matrix
         */
        void loadTab(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            Matrix2f const& rotation);

        /**
         * Loads the newline into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         * @param rotation the rotation matrix
         */
        void loadNewline(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            Matrix2f const& rotation);

        /**
         * Returns the dimensions of the glyph
         *
         * @param glyph the reference wrapper to the glyph object
         * @param scale the text's scale factor
         * @param rotation the rotation matrix
         * @return the tuple with glyph dimensions
         */
        VectorTuple getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale,
            Matrix2f const& rotation) const noexcept;

        /**
         * Emplaces glyph into the memory
         *
         * @param texture the glyph's texture
         * @param glpyh the reference wrapper to the glyph object
         * @param scale the text's scale factor
         * @param rotation the rotation matrix
         */
        void emplaceGlyph(
            Texture const& texture,
            Subfont::GlyphRef glpyh,
            float32 scale,
            Matrix2f const& rotation);

        /**
         * Extends text modifiers
         *
         * @param advance the glyph advance vector
         */
        void extendModifiers(
            Vector2f advance) noexcept;

        /**
         * Extends text underline
         *
         * @param advance the glyph advance vector
         */
        void extendUnderline(
            Vector2f advance) noexcept;

        /**
         * Extends text strikethrough
         *
         * @param advance the glyph advance vector
         */
        void extendStrikethrough(
            Vector2f advance) noexcept;

        /**
         * Emplaces modifiers into memory
         */
        void emplaceModifiers(void);

        /**
         * Emplaces underline into memory
         */
        void emplaceUnderline(void);

        /**
         * Emplaces strikethrough into memory
         */
        void emplaceStrikethrough(void);

        /**
         * Returns a text's projection level
         *
         * @return the text's projection level
         */
        uint8 getLevel(void) const noexcept;

        /**
         * Returns a glyph coefficients
         *
         * @return the glyph coefficients
         */
        ArgTuple glyphCoefficients(void) const noexcept;

        /**
         * Reloads glyphs displayed by the text
         */
        void reloadGlyphs(void);

        /**
         * Parses unicode string into IDs array
         *
         * @param string the unicode string
         * @return the array containing glyphs IDs
         */
        static IDArray parseString(String const& string);

        /**
         * Returns an intersection of two vectorized lines
         *
         * @param firstPoint the point on the first line
         * @param firstVersor the versor of the first line
         * @param secondPoint the point on the second line
         * @param secondVersor the versor of the second line
         * @return the intersection point of the two lines
         */
        static Vector2f intersectionOf(
            Vector2f const& firstPoint,
            Vector2f const& firstVersor,
            Vector2f const& secondPoint,
            Vector2f const& secondVersor) noexcept;

        /**
         * Returns a type of shader used by the text
         *
         * @return the type of shader used by the text
         */
        static String const shaderType(void);

        /**
         * Generates the underline tetragon
         *
         * @param position the position of the line
         * @param angle the rotation angle [in rads]
         * @param textSize the size of the text
         * @param color the color of the text
         * @return the underline tetragon
         */
        static Tetragon generateUnderline(
            Vector2f position,
            float32 angle,
            SizeT textSize,
            Color const& color) noexcept;

        /**
         * Generates the strikethrough tetragon
         *
         * @param position the position of the line
         * @param angle the rotation angle [in rads]
         * @param textSize the size of the text
         * @param color the color of the text
         * @return the strikethrough tetragon
         */
        static Tetragon generateStrikethrough(
            Vector2f position,
            float32 angle,
            SizeT textSize,
            Color const& color) noexcept;
    };

    template class Text<true>;
    template class Text<false>;

    /**
     * Bitwise adds two modifiers flags
     *
     * @param left the constant reference to the left flag
     * @param right the constant reference to the right flag
     * @return the result of the operation
     */
    constexpr uint8 operator&(
        TextOptions::Modifiers const& left,
        TextOptions::Modifiers const& right) noexcept;

}

#include <MPGL/Core/Text/Text.ipp>

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
#include <MPGL/Core/DrawableCollection.hpp>
#include <MPGL/Core/Text/GlyphSprite.hpp>
#include <MPGL/Traits/DeriveIf.hpp>
#include <MPGL/Core/Text/Font.hpp>
#include <MPGL/Core/Model.hpp>

namespace mpgl {

    /**
     * Colored text glyph view that omits the position of the
     * glyph and allows to get only the color of the glyph's
     * vertices
     *
     * @tparam Dim the dimension of the space where the glyph is
     * being drawn
     * @tparam IsConst if the glyph sprite should be constant
     */
    template <Dimension Dim, bool IsConst>
    class TextGlyphView {
    public:
        typedef std::conditional_t<IsConst,
            GlyphSprite<Dim> const,
            GlyphSprite<Dim>>                       GlyphType;
        typedef std::reference_wrapper<GlyphType>   GlyphRef;
        typedef std::size_t                         SizeT;
        typedef std::conditional_t<IsConst,
            Color const, Color>                     value_type;
        typedef value_type&                         reference;

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
            SizeT index) const noexcept
                { return ref.get()[index] | cast::color; }

        /**
         * Iterator that returns only the color of the underlying
         * vertex sprite vertices
         *
         * @tparam BaseTp the base type of the operator
         * @tparam Iter the glyph sprite iterator
         */
        template <class BaseTp, std::random_access_iterator Iter>
        class Iterator {
        public:
            typedef BaseTp                          value_type;
            typedef value_type*                     pointer;
            typedef value_type&                     reference;
            typedef std::ptrdiff_t                  difference_type;
            typedef std::random_access_iterator_tag iterator_category;
            typedef std::compare_three_way_result_t<Iter, Iter>
                                                    compare;

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
                { return *iter | cast::color; }

            /**
             * Returns a pointer to the glyph sprite vertex's
             * color
             *
             * @return the pointer to the glyph sprite vertex's
             * color
             */
            [[nodiscard]] pointer operator->(void) const noexcept
                { return &(*iter | cast::color); }

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
                    { return *(iter + offset) | cast::color; }

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
            GlyphSprite<Dim>::iterator>;
        using const_iterator = Iterator<Color const, typename
            GlyphSprite<Dim>::const_iterator>;
        using reverse_iterator = Iterator<Color, typename
            GlyphSprite<Dim>::reverse_iterator>;
        using const_reverse_iterator = Iterator<Color const, typename
            GlyphSprite<Dim>::const_reverse_iterator>;

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
                { return const_iterator{ ref.get().cbegin() }; }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return const_iterator{ ref.get().cend() }; }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return const_iterator{ ref.get().cbegin() }; }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return const_iterator{ ref.get().cend() }; }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator
            rbegin(void) noexcept requires (!IsConst)
                { return reverse_iterator{ ref.get().rbegin() }; }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator
            rend(void) noexcept requires (!IsConst)
                { return reverse_iterator{ ref.get().rend() }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return const_reverse_iterator{ ref.get().crbegin() }; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return const_reverse_iterator{ ref.get().crend() }; }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return const_reverse_iterator{ ref.get().crbegin() }; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return const_reverse_iterator{ ref.get().crend() }; }
    private:
        GlyphRef                                    ref;
    };

    template class TextGlyphView<dim::Dim2, false>;
    template class TextGlyphView<dim::Dim2, true>;
    template class TextGlyphView<dim::Dim3, false>;
    template class TextGlyphView<dim::Dim3, true>;

    /**
     * The text initialization options
     */
    struct TextOptions {
        typedef Font::Type                          Style;

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
        float32                                 size = 18.f;
        /// The color of the text
        Color                                   color = Color{};
        /// The style of the text
        Style                                   style = Style::Regular;
        /// The modifiers of the text
        Modifiers                               mods = Modifiers::None;
    };

    /**
     * Represents a text on the screen
     *
     * @tparam Dim the dimension of the space where the glyph is
     * being drawn
     */
    template <Dimension Dim>
        class Text :
        public Shadeable,
        public Transformable<Dim>,
        public Drawable<Dim>,
        public DeriveIfT<ThreeDimensional<Dim>, Model>
    {
    public:
        typedef GlyphSprite<Dim>                    FontGlyph;
        typedef DrawableCollection<FontGlyph>       GlyphsVector;
        typedef Font::Type                          Style;
        typedef std::size_t                         SizeT;
        typedef std::string                         String;
        typedef TextOptions::Modifiers              Modifiers;
        typedef typename FontGlyph::Vector          Vector;
        typedef typename FontGlyph::Adapter         Adapter;

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
            Vector const& position = {},
            String const& text = {},
            TextOptions const& options = {});

        Text(Text const& text) = default;
        Text(Text&& text) = default;

        Text& operator= (Text const& text) = default;
        Text& operator= (Text&& text) = default;

        /**
         * Adds the given string to the text
         *
         * @param left the constant reference to the string
         * @return the reference to this object
         */
        Text& operator+= (String const& left);

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
         * Returns the size of the text [number of the nonwhite
         * characters]
         *
         * @return the size of the text
         */
        [[nodiscard]] SizeT size(void) const noexcept
            { return glyphs.size(); }

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
        void setSize(float32 size);

        /**
         * Sets the given string
         *
         * @param text the constant reference to the
         * new string object
         */
        void setString(String const& text);

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
        [[nodiscard]] float32 const& getTextSize(void) const noexcept
            { return textSize; }

        /**
         * Returns the modifiers of the text glyphs
         *
         * @return the constant reference to the color modifiers
         */
        [[nodiscard]] Modifiers const& getModifiers(void) const noexcept
            { return mods; }

        /**
         * Returns the string handled by the text object
         *
         * @return the constant reference to the string object
         */
        [[nodiscard]] String const& getString(
            void) const noexcept
                { return text; }

        /**
         * Returns the text's position
         *
         * @return the text's position
         */
        [[nodiscard]] Vector getPosition(void) const noexcept;

        /**
         * Removes string displayed by the text
         */
        void clear(void) noexcept;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(
            Transformation<Dim> const& transformator) noexcept final;

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
        void setShader(String const& name) final override;

        /**
         * Draws the text on the screen
         */
        void draw(void) const noexcept final;

        /**
         * The text class iterator. Returns a views to the glyph
         * sprites contained in the text
         *
         * @tparam IsConst if the glyph sprite's should be constant
         */
        template <bool IsConst>
        class Iterator {
        public:
            typedef std::conditional_t<IsConst,
                typename GlyphsVector::const_iterator,
                typename GlyphsVector::iterator>        GlyphIter;
            typedef TextGlyphView<Dim, IsConst>         value_type;
            typedef std::ptrdiff_t                      difference_type;
            typedef std::output_iterator_tag            iterator_category;

            using compare =
                std::compare_three_way_result_t<GlyphIter, GlyphIter>;

            /**
             * Constructs a new Iterator object from a constant
             * reference to the glyph's vector iterator
             *
             * @param iter the constant reference to the glyph's
             * vector iterator
             */
            explicit Iterator(GlyphIter const& iter) noexcept
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
            Iterator& operator+= (
                difference_type offset) noexcept
                    { iter += offset; return *this; };

            /**
             * Decrements iterator by the given distance
             *
             * @param offset the decremented distance
             * @return reference to this object
             */
            Iterator& operator-= (
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
            GlyphIter                                   iter;
        };

        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<
            const_iterator>;

        /**
         * Returns the iterator to the begining of the glyph sprites
         *
         * @return the iterator to the begining of the glyph sprites
         */
        [[nodiscard]] iterator begin(void) noexcept
            { return iterator{glyphs.begin()}; }

        /**
         * Returns the iterator to the end of the glyph sprites
         *
         * @return the iterator to the end of the glyph sprites
         */
        [[nodiscard]] iterator end(void) noexcept
            { return iterator{glyphs.end()}; }

        /**
         * Returns the constant iterator to the begining
         * of the glyph sprites
         *
         * @return the constant iterator to the begining
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return const_iterator{glyphs.begin()}; }

        /**
         * Returns the constant iterator to the end
         * of the glyph sprites
         *
         * @return the constant iterator to the end
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return const_iterator{glyphs.end()}; }

        /**
         * Returns the constant iterator to the begining
         * of the glyph sprites
         *
         * @return the constant iterator to the begining
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return const_iterator{glyphs.begin()}; }

        /**
         * Returns the constant iterator to the end
         * of the glyph sprites
         *
         * @return the constant iterator to the end
         * of the glyph sprites
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return const_iterator{glyphs.end()}; }

        /**
         * Returns the reverse iterator to the end of
         * the glyph sprites
         *
         * @return the reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{end() - 1}; }

        /**
         * Returns the reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { return reverse_iterator{begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept
                { return const_reverse_iterator{end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept
                { return const_reverse_iterator{begin() - 1}; }

        /**
         * Returns the constant reverse iterator to the end of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the end of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept
                { return const_reverse_iterator{end() - 1}; }

        /**
         * Returns the constant reverse iterator to the begining of
         * the glyph sprites
         *
         * @return the constant reverse iterator to the begining of
         * the glyph sprites
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept
                { return const_reverse_iterator{begin() - 1}; }

        /**
         * Destroys the Text object
         */
        ~Text(void) noexcept = default;
    private:
        typedef std::vector<uint16>                 IDArray;
        typedef DrawableCollection<Tetragon<Dim>>   Lines;
        typedef std::pair<uint8, float32>           ArgTuple;
        typedef std::tuple<Vector, Vector,
            Vector>                                 VectorTuple;
        typedef std::tuple<float32, float32,
            Vector2f>                               GlyphDimensions;
        typedef std::pair<Vector2f, std::size_t>    GlyphPosPair;
        typedef typename ShadersContext::ProgramPtr ProgramPtr;
        typedef typename ShadersContext::Executable Executable;

        static constexpr SizeT                      ShiftBase
            = Subfont::shiftBase;
        static constexpr SizeT                      ShiftValue
            = log2N<SizeT, ShiftBase>();
        static constexpr uint16_t const             Newline = 10u;
        static constexpr uint16_t const             Tabulator = 9u;

        /**
         * Holds the special space that is used to determine a new
         * position of glyphs / modifiers in text. Rememberes all
         * of the linear transformations performed on it
         */
        class PositionHolder : public Transformable<Dim> {
        public:
            /**
             * Constructs a new Position Holder object
             *
             * @param position the constant reference to the
             * text's initial position vector object
             */
            explicit PositionHolder(Vector const& position) noexcept;

            PositionHolder(PositionHolder const&) noexcept = default;
            PositionHolder(PositionHolder&&) noexcept = default;

            PositionHolder& operator=(
                PositionHolder const&) noexcept = default;
            PositionHolder& operator=(
                PositionHolder&&) noexcept = default;

            /**
             * Performs transformation on the space
             *
             * @param transformator the constant reference to the
             * transforming object
             */
            void transform(
                Transformation<Dim> const& transformator
                ) noexcept final;

            /**
             * Advances space according to the advance vector
             *
             * @param advanceVector the constant reference to the
             * advance vector
             * @return the real advancement
             */
            Vector advance(Vector2f const& advanceVector) noexcept;

            /**
             * Calculates the position of the glyph sprite according
             * to the inner space
             *
             * @param bearing the constant reference to the glyph's
             * bearing
             * @param width the glyph's width
             * @param height the glyph's height
             * @return the glyph's position
             */
            [[nodiscard]] VectorTuple calculatePositon(
                Vector2f const& bearing,
                float32 width,
                float32 height) const noexcept;

            /**
             * Calculates the position of the strikethrough according
             * to the inner space
             *
             * @param midspan the strikethrough's midsppan
             * @param halfspan the strikethrough's halfspan
             * @return the strikethrough's position
             */
            [[nodiscard]] VectorTuple calculatePositon(
                float32 midspan,
                float32 halfspan) const noexcept;

            /**
             * Calculates the position of the underline according
             * to the inner space
             *
             * @param span the underline's span
             * @return the underline's position
             */
            [[nodiscard]] VectorTuple calculatePositon(
                float32 span) const noexcept;

            /**
             * Moves inner space to the given point
             *
             * @param vector the point
             */
            void move(Vector const& point) noexcept;

            /**
             * Returns the inner space's position vector
             *
             * @return the inner space's positon versor
             */
            [[nodiscard]] Vector getPosition(void) const noexcept;

            /**
             * Calculates the position of text based on the first
             * glyph
             *
             * @param position the constant reference to the first
             * glyph's position
             * @param bearing the constant reference to the glyph's
             * bearing
             * @return the original's position
             */
            [[nodiscard]] Vector findOrigin(
                Vector const& position,
                Vector2f const& bearing) const noexcept;

            /**
             * Destroys the Position Holder object
             */
            ~PositionHolder(void) noexcept = default;
        private:
            typedef std::array<Adapter, 3>          Vertices;

            Vertices                                vertices;
            Vector                                  xVersor;
            Vector                                  yVersor;
            Vector                                  position;

            /**
             * Actualizes the inner versors and position
             */
            void actualize(void) noexcept;

            /**
             * Transforms the position to the inner linear system
             *
             * @param position the position in euclidean system
             * @return the position in the inner system
             */
            [[nodiscard]] Vector changeSystem(
                Vector2f const& position) const noexcept;
        };

        PositionHolder                              positionSpace;
        String                                      text;
        GlyphsVector                                glyphs;
        Font                                        font;
        Lines                                       underlines;
        Lines                                       strikethroughs;
        Color                                       color;
        float32                                     textSize;
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
         */
        void loadGlyph(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index);

        /**
         * Loads the nonwhite character into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         * @param index the glyph index
         */
        void loadCharacter(
            Subfont& subfont,
            uint8 level,
            float32 scale,
            uint16 index);

        /**
         * Loads the tabulator into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         */
        void loadTab(
            Subfont& subfont,
            uint8 level,
            float32 scale);

        /**
         * Loads the newline into the memory
         *
         * @param subfont the reference to the subfont object
         * @param level the projection level
         * @param scale the text's scale factor
         */
        void loadNewline(
            Subfont& subfont,
            uint8 level,
            float32 scale);

        /**
         * Returns the dimensions of the glyph
         *
         * @param glyph the reference wrapper to the glyph object
         * @param scale the text's scale factor
         * @return the tuple with glyph dimensions
         */
        GlyphDimensions getGlyphDimensions(
            Subfont::GlyphRef glyph,
            float32 scale) const noexcept;

        /**
         * Emplaces glyph into the memory
         *
         * @param texture the glyph's texture
         * @param glpyh the reference wrapper to the glyph object
         * @param scale the text's scale factor
         */
        void emplaceGlyph(
            Texture const& texture,
            Subfont::GlyphRef glpyh,
            float32 scale);

        /**
         * Extends text modifiers
         *
         * @param advance the glyph advance vector
         */
        void extendModifiers(
            Vector const& advance) noexcept;

        /**
         * Extends text underline
         *
         * @param advance the glyph advance vector
         */
        void extendUnderline(
            Vector const& advance) noexcept;

        /**
         * Extends text strikethrough
         *
         * @param advance the glyph advance vector
         */
        void extendStrikethrough(
           Vector const& advance) noexcept;

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
         * Sets the shader locations
         */
        void setLocations(void);

        /**
         * Finds the first glyph that has an outline in the text
         * and returns its bearing and it's number
         *
         * @return the glyph's bearing and it's number
         */
        GlyphPosPair findFirstGlyphBearing(void) const noexcept;

        /**
         * Parses unicode string into IDs array
         *
         * @param string the unicode string
         * @return the array containing glyphs IDs
         */
        static IDArray parseString(String const& string);

        /**
         * Returns a type of shader used by the text
         *
         * @return the type of shader used by the text
         */
        static String const shaderType(void);

        /**
         * Generates the underline tetragon
         *
         * @param positionSpace the position holder of the text
         * @param textSize the size of the text
         * @param color the color of the text
         * @return the underline tetragon
         */
        static Tetragon<Dim> generateUnderline(
            PositionHolder const& positionSpace,
            float32 textSize,
            Color const& color) noexcept;

        /**
         * Generates the strikethrough tetragon
         *
         * @param positionSpace the position holder of the text
         * @param textSize the size of the text
         * @param color the color of the text
         * @return the strikethrough tetragon
         */
        static Tetragon<Dim> generateStrikethrough(
            PositionHolder const& positionSpace,
            float32 textSize,
            Color const& color) noexcept;

        /**
         * Sets the color on the joinable ranges
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         * @param color the constant reference to the color
         */
        static void setColorOnJoinableRange(
            std::ranges::forward_range auto&& range,
            Color const& color) noexcept;

    };

    template class Text<dim::Dim2>;
    template class Text<dim::Dim3>;

    using Text2D = Text<dim::Dim2>;
    using Text3D = Text<dim::Dim3>;

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
#include <MPGL/Core/Text/Text.tpp>

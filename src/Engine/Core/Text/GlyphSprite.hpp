#pragma once

#include "../Color.hpp"
#include "../Texture.hpp"
#include "../Drawable.hpp"
#include "../../Utility/Adapter.hpp"
#include "../../Mathematics/Systems.hpp"
#include "../Transformations/Transformable2D.hpp"

#include <utility>
#include <variant>
#include <array>

namespace ge {

    class MonochromaticFontVertex {
    public:
        explicit MonochromaticFontVertex(Vector2f const& position,
            Vector2f const& textureCoords) noexcept
            : position{position}, textureCoords{textureCoords} {}

        Adapter<Vector2f>                           position;
        Vector2f                                    textureCoords;

        template <std::size_t Index>
            requires (Index < 2)
        constexpr auto&& get(void) & noexcept
            { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 2)
        constexpr auto&& get(void) const& noexcept
            { return helper<Index>(*this); }
    private:
        template <std::size_t Index, class Base>
        constexpr auto&& helper(Base&& base) const noexcept {
            if constexpr (!Index)
                return std::forward<Base>(base).position;
            else
                return std::forward<Base>(base).textureCoords;
        }
    };

    class PolichromaticFontVertex {
    public:
        explicit PolichromaticFontVertex(Vector2f const& position,
            Color const& color,
            Vector2f const& textureCoords) noexcept
            : position{position}, color{color},
                textureCoords{textureCoords} {}

        Adapter<Vector2f>                           position;
        Vector2f                                    textureCoords;
        Color                                       color;

        template <std::size_t Index>
            requires (Index < 3)
        constexpr auto&& get(void) & noexcept
            { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 3)
        constexpr auto&& get(void) const& noexcept
            { return helper<Index>(*this); }
    private:
        template <std::size_t Index, class Base>
        constexpr auto&& helper(Base&& base) const noexcept {
            if constexpr (Index == 0)
                return std::forward<Base>(base).position;
            else if constexpr (Index == 2)
                return std::forward<Base>(base).color;
            else
                return std::forward<Base>(base).textureCoords;
        }
    };

    // custom texture allocators will be add
    template <bool IsMonochromatic>
    class GlyphSprite : public Transformable2D,
        private GraphicalObject
    {
    public:
        using Vertex = std::conditional_t<IsMonochromatic,
            MonochromaticFontVertex, PolichromaticFontVertex>;

        typedef std::array<Vertex, 4>               Vertices;
        typedef std::array<uint32, 6>               Indexes;

        GlyphSprite(Texture const& texture, Color const& color = {});
        // parallelogram
        GlyphSprite(Texture const& texture,
            Vector2f const& firstVertex, Vector2f const& secondVertex,
            Vector2f const& thirdVertex, Color const& color = {});
        //rectangle parallel to x and y axis
        GlyphSprite(Texture const& texture,
            Vector2f const& firstVertex, Vector2f const& dimensions,
            Color const& color = {});

        GlyphSprite(GlyphSprite const& sprite) noexcept;
        GlyphSprite(GlyphSprite&& sprite) noexcept;

        GlyphSprite& operator= (GlyphSprite const& sprite) noexcept;
        GlyphSprite& operator= (GlyphSprite&& sprite) noexcept;

        void copyToGPU(void) const noexcept;
        void draw(void) const noexcept;

        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;
        void translate(
            Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center,
            float32 factor) noexcept final;
        void rotate(Vector2f const& center,
            float32 angle) noexcept final;
        void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept final;

        Vertex& operator[] (std::size_t index) noexcept
            { return vertices[index]; }
        const Vertex& operator[] (std::size_t index) const noexcept
            { return vertices[index]; }

        void setColor(Color const& color = {}) noexcept
            requires (!IsMonochromatic);

        consteval std::size_t size(void) const noexcept
            { return 4; }

        using iterator = typename Vertices::iterator;
        using const_iterator = typename Vertices::const_iterator;
        using reverse_iterator = typename Vertices::reverse_iterator;
        using const_reverse_iterator =
            typename Vertices::const_reverse_iterator;

        iterator begin(void) noexcept
            { return vertices.begin(); }
        iterator end(void) noexcept
            { return vertices.end(); }

        const_iterator begin(void) const noexcept
            { return vertices.begin(); }
        const_iterator end(void) const noexcept
            { return vertices.end(); }

        const_iterator cbegin(void) const noexcept
            { return vertices.cbegin(); }
        const_iterator cend(void) const noexcept
            { return vertices.cend(); }

        reverse_iterator rbegin(void) noexcept
            { return vertices.rbegin(); }
        reverse_iterator rend(void) noexcept
            { return vertices.rend(); }

        const_reverse_iterator rbegin(void) const noexcept
            { return vertices.rbegin(); }
        const_reverse_iterator rend(void) const noexcept
            { return vertices.rend(); }

        const_reverse_iterator crbegin(void) const noexcept
            { return vertices.crbegin(); }
        const_reverse_iterator crend(void) const noexcept
            { return vertices.crend(); }

        ~GlyphSprite(void) noexcept;
    private:
        Vertices                                    vertices;
        Texture                                     texture;
        uint32                                      elementArrayBuffer;
        uint32                                      vertexBuffer;
        uint32                                      vertexArrayObject;

        Vertices makeVertexArray(Color const& color) noexcept;
        void bindBuffers(void) const noexcept;
        void copyBuffersToGPU(void) const noexcept;
        void unbindBuffers(void) const noexcept;

        static constexpr const Indexes              indexes = {
            {0, 1, 2, 0, 3, 2}};
    };

    template class GlyphSprite<true>;
    template class GlyphSprite<false>;

    typedef GlyphSprite<true>                       MonoGlyphSprite;
    typedef GlyphSprite<false>                      PoliGlyphSprite;

}

namespace std {

    template <>
    struct tuple_size<ge::MonochromaticFontVertex>
        : integral_constant<size_t, 2> {};

    template <>
    struct tuple_element<0, ge::MonochromaticFontVertex> {
        using type = ge::Adapter<ge::Vector2f>;
    };

    template <>
    struct tuple_element<1, ge::MonochromaticFontVertex> {
        using type = ge::Vector2f;
    };

    template <>
    struct tuple_size<ge::PolichromaticFontVertex>
        : integral_constant<size_t, 3> {};

    template <>
    struct tuple_element<0, ge::PolichromaticFontVertex> {
        using type = ge::Adapter<ge::Vector2f>;
    };

    template <>
    struct tuple_element<1, ge::PolichromaticFontVertex> {
        using type = ge::Color;
    };

    template <>
    struct tuple_element<2, ge::PolichromaticFontVertex> {
        using type = ge::Vector2f;
    };

}

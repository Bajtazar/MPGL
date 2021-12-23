#pragma once

#include "Shadeable.hpp"
#include "../Color.hpp"
#include "../Texture.hpp"
#include "../Drawable.hpp"
#include "../../Utility/Adapter.hpp"
#include "../Transformations/Transformable2D.hpp"

#include <utility>
#include <array>

namespace ge {

    struct ColorableVertex {
        explicit ColorableVertex(Vector2f const& position,
            Color const& color,
            Vector2f const& textureCoords) noexcept
                : position{position},
                textureCoords{textureCoords}, color{color} {}

        Adapter<Vector2f>                   position;
        Vector2f                            textureCoords;
        Color                               color;

        template <std::size_t Index>
            requires (Index < 3)
        constexpr auto&& get(void) & noexcept;

        template <std::size_t Index>
            requires (Index < 3)
        constexpr auto&& get(void) const& noexcept;
    };

    struct DefaultVertex {
        explicit DefaultVertex(Vector2f const& position,
            Vector2f const& textureCoords) noexcept
                : position{position},
                textureCoords{textureCoords} {}

        Adapter<Vector2f>                   position;
        Vector2f                            textureCoords;

        template <std::size_t Index>
            requires (Index < 2)
        constexpr auto&& get(void) & noexcept;

        template <std::size_t Index>
            requires (Index < 2)
        constexpr auto&& get(void) const& noexcept;
    };

    template <bool IsColorable = false>
    class Sprite : public Drawable, public Shadeable,
        public Transformable2D
    {
    public:
        using Vertex = std::conditional_t<IsColorable,
            ColorableVertex, DefaultVertex>;

        typedef std::array<Vertex, 4>       VertexArray;

        // Default constructor
        Sprite(Texture const& texture) noexcept;
        // Default constructor with color
        Sprite(Texture const& texture,
            Color const& color) noexcept requires IsColorable;
        // parallelogram
        Sprite(Texture const& texture, Vector2f const& firstVertex,
               Vector2f const& secondVertex,
               Vector2f const& thirdVertex) noexcept;
        // parallelogram for colorable version
        Sprite(Texture const& texture, Vector2f const& firstVertex,
               Vector2f const& secondVertex,
               Vector2f const& thirdVertex,
               Color const& color) noexcept requires IsColorable;
        // rectangle parallel to the x and y axis
        Sprite(Texture const& texture, Vector2f const& firstVertex,
               Vector2f const& dimmensions) noexcept;
        // rectangle parallel to the x and y axis for colorable version
        Sprite(Texture const& texture, Vector2f const& firstVertex,
               Vector2f const& dimmensions,
               Color const& color) noexcept requires IsColorable;

        Sprite(Sprite const& sprite) noexcept;
        Sprite(Sprite&& sprite) noexcept;

        Sprite& operator= (Sprite const& sprite) noexcept;
        Sprite& operator= (Sprite&& sprite) noexcept;

        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(
            Vector2ui const& oldDimmensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center,
            float factor) noexcept final;
        void rotate(Vector2f const& center,
            float angle) noexcept final;
        void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept final;

        Vertex& operator[] (std::size_t index) noexcept
            { return vertices[index]; }
        Vertex const& operator[] (std::size_t index) const noexcept
            { return vertices[index]; }

        consteval std::size_t size(void) const noexcept
            { return 4; }

        void replaceTexture(Texture const& texture);

        using iterator = VertexArray::iterator;
        using const_iterator = VertexArray::const_iterator;
        using reverse_iterator = VertexArray::reverse_iterator;
        using const_reverse_iterator =
            VertexArray::const_reverse_iterator;

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

        ~Sprite(void) noexcept;
    protected:
        typedef typename ShadersContext::ProgramPtr ProgramPtr;
        typedef typename ShadersContext::Executable Executable;
        typedef std::array<uint32_t, 6>       Indexes;

        static constexpr const Indexes          indexes{
            {0, 1, 2, 0, 3, 2}};
        static const Executable                 shaderExec;

        VertexArray                             vertices;
        Texture                                 texture;
        uint32_t                                elementArrayBuffer;
        uint32_t                                vertexBuffer;
        uint32_t                                vertexArrayObject;

        void setVerticesPoisition(Vector2f const& firstVertex,
            Vector2f const& dimmensions) noexcept;
        void setVerticesPoisition(Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex) noexcept;
        void generateBuffers(void) noexcept;
        void bindBuffers(void) const noexcept;
        void copyBuffersToGPU(void) const noexcept;
        void unbindBuffers(void) const noexcept;

        static VertexArray makeVertexArray(void) noexcept;
        static VertexArray makeVertexArray(
            Color const& color) noexcept requires IsColorable;
        static inline constexpr std::string shaderType(void) noexcept;
    };

    template <bool IsColorable>
    inline constexpr std::string Sprite<IsColorable>::shaderType(
        void) noexcept
    {
        if constexpr (IsColorable)
            return "2DCTexture";
        else
            return "2DTexture";
    }

    template class Sprite<true>;
    template class Sprite<false>;

    typedef Sprite<false>                       DefaultSprite;
    typedef Sprite<true>                        ColorableSprite;

    template <std::size_t Index>
        requires (Index < 3)
    constexpr auto&& ColorableVertex::get(void) & noexcept {
        if constexpr (Index == 2)
            return color;
        else if constexpr (Index == 1)
            return textureCoords;
        else
            return position;
    }

    template <std::size_t Index>
        requires (Index < 3)
    constexpr auto&& ColorableVertex::get(void) const& noexcept {
        if constexpr (Index == 2)
            return color;
        else if constexpr (Index == 1)
            return textureCoords;
        else
            return position;
    }

    template <std::size_t Index>
        requires (Index < 2)
    constexpr auto&& DefaultVertex::get(void) & noexcept {
        if constexpr (Index == 1)
            return textureCoords;
        else
            return position;
    }

    template <std::size_t Index>
        requires (Index < 2)
    constexpr auto&& DefaultVertex::get(void) const& noexcept {
        if constexpr (Index == 1)
            return textureCoords;
        else
            return position;
    }

}

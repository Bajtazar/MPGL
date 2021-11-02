#pragma once

#include "../../Color.hpp"
#include "../../Drawable.hpp"
#include "../../../Utility/Adapter.hpp"
#include "../../Transformations/Transformable2D.hpp"

namespace ge {

    class Ellipse : public Drawable, public Transformable2D {
    public:
        Ellipse(Vector2f const& center, Vector2f const& semiAxis,
                float angle = 0.f, Color const& color = {}) noexcept;
        Ellipse(Vector2f const& center, float radius,
                Color const& color = {}) noexcept;
        Ellipse(Ellipse const& ellipse) noexcept;
        Ellipse(Ellipse&& ellipse) noexcept;

        Ellipse& operator= (Ellipse const& ellipse) noexcept;
        Ellipse& operator= (Ellipse&& ellipse) noexcept;

        void setShaders(ShaderLibrary const& shaderLibrary) noexcept final;
        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;
        void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept final;

        Vector2f getCenter(void) const noexcept;
        Vector2f getSemiAxis(void) const noexcept;

        void setColor(Color const& color = {}) noexcept
            { this->color = color; }
        Color const& getColor(void) const noexcept
            { return color; }

        ~Ellipse(void) noexcept;
    private:
        typedef std::array<uint32_t, 6>             Indexes;
        typedef std::array<Adapter<Vector2f>, 4>    Vertices;

        Vertices                                    vertices;
        Matrix2f                                    transform;
        Color                                       color;
        ShaderProgram                               shaderProgram;
        uint32_t                                    vertexBuffer;
        uint32_t                                    vertexArrayObject;
        uint32_t                                    elementArrayBuffer;


        static Vertices ellipseVertices(Vector2f const& center,
            Vector2f const& semiAxis, float angle) noexcept;
        static Vertices circleVertices(Vector2f const& center,
            float radius) noexcept;

        void generateBuffers(void) noexcept;
        void bindBuffers(void) const noexcept;
        void copyBuffersToGPU(void) const noexcept;
        void unbindBuffers(void) const noexcept;

        void recalculateUniforms(void) noexcept;

        static constexpr const Indexes              indexes {0, 1, 2, 0, 3, 2};
    };

}
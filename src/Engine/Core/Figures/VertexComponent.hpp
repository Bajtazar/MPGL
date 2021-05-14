#pragma once

#include "FigureType.hpp"
#include "Drawable.hpp"

#include <vector>

namespace ge {

    class VertexComponent : public Drawable {
    public:
        explicit VertexComponent(const Vector2i&  scene, FigureTypes::PrimitveType auto type) noexcept;
        explicit VertexComponent(const Vector2i&  scene, FigureTypes::LineType auto type) noexcept;


        ~VertexComponent(void) noexcept = default;
    private:
        explicit VertexComponent(const Vector2i&  scene) noexcept;

        std::vector<float> vertices;
        uint32_t verticesBuffer;

        virtual void setShaders(const ShaderLibrary&) const noexcept final;
        virtual void copyToGPU(void) const noexcept final;
        virtual void draw(void) const noexcept final;
    };

}

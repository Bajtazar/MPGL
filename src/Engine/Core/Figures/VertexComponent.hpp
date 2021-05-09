#pragma once

#include "FigureType.hpp"
#include "Drawable.hpp"

#include <vector>

namespace ge {

    class VertexComponent : public Drawable {
    public:
        template <FigureTypes::PrimitveType Type>
        explicit VertexComponent(const Vector2i&  scene, Type type) noexcept;
        template <FigureTypes::LineType Type>
        explicit VertexComponent(const Vector2i&  scene, Type type) noexcept;


        ~VertexComponent(void) noexcept = default;
    private:
        explicit VertexComponent(const Vector2i&  scene) noexcept;

        std::vector<float> vertices;
        uint32_t verticesBuffer;

        virtual void setShaders() const noexcept final;
        virtual void copyToGPU(void) const noexcept final;
        virtual void draw(RenderWindow& target) const noexcept final;
    };

}

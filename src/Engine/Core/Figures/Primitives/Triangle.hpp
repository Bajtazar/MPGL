#pragma once

#include "../VertexComponent.hpp"

namespace ge {

    template <bool PolygonMode = false>
    class Triangle : public VertexComponent<PolygonMode> {
    public:
        explicit Triangle(void) noexcept : vertices(3) {}
        explicit Triangle(const Vector2i& scene, const Vector2i& firstVerticie,
                          const Vector2i& secondVerticie, const Vector2i& thirdVerticie) noexcept;
    };

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Vector2i& scene, const Vector2i& firstVerticie, const Vector2i& secondVerticie, const Vector2i& thirdVerticie) noexcept : VertexComponent(scene, 3) {
        //auto __iter = vertices.begin();
    }

}

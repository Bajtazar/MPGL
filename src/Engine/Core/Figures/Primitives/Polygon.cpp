#include "Polygon.hpp"

#include "../../Context/Buffers/BindGuard.hpp"
#include "../../../Mathematics/Systems.hpp"
#include "../Views.hpp"

#include <numbers>
#include <iterator>

namespace mpgl {

    Polygon::Polygon(Vector2f const& center,
        float32 radius, std::size_t segments,
        Color const& color)
            : ResizableAngular{segments + 1, color}
    {
        float32 increment = 2.f *
            std::numbers::pi_v<float32> / (segments - 1), angle = 0.f;
        get<"position">(vertices.front()) = center;
        for (auto& position : vertices | std::views::drop(1) |
            views::position)
        {
            position = polarToCartesian(Vector2f{radius, angle}) + center;
            angle += increment;
        }
    }

    void Polygon::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawArrays(VertexArray::DrawMode::TriangleFan,
            vertices.size());
    }

}
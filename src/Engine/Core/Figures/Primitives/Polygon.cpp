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
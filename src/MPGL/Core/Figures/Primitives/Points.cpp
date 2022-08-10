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
#include <MPGL/Core/Figures/Primitives/Points.hpp>

#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Views.hpp>
#include <MPGL/Utility/Ranges.hpp>

namespace mpgl {

    Points::Points(std::size_t vertices, Color const& color)
        : ResizableAngular{vertices, color} {}

    void Points::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawArrays(VertexArray::DrawMode::Points,
            vertices.size());
    }

    [[nodiscard]] bool Points::contains(
        Vector2u const& position) const noexcept
    {
        Vector2f normalized = Adapter2D{position}.get();
        for (auto const& pointPos : vertices | views::position)
            if (std::ranges::equal(pointPos.get(), normalized))
                return true;
        return false;
    }

}

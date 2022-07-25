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
#include <MPGL/Core/Figures/Primitives/LineStrip.hpp>

#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Mathematics/Systems.hpp>

#include <limits>

namespace mpgl {

    LineStrip::LineStrip(std::size_t vertices,
        Color const& color) : ResizableAngular{vertices, color} {}

    void LineStrip::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawArrays(VertexArray::DrawMode::LineStrip,
            vertices.size());
    }

    [[nodiscard]] bool LineStrip::insideInterval(
        Vector2f const& position,
        std::size_t index) const noexcept
    {
        return between(get<"position">(vertices[index - 1]).get(),
            get<"position">(vertices[index]).get(), position);
    }

    [[nodiscard]] bool LineStrip::onLine(
        Vector2f const& position,
        std::size_t index) const noexcept
    {
        Vector2f begin = get<"position">(vertices[index - 1]).get();
        Vector2f end = get<"position">(vertices[index]).get();
        return std::fabs(cross(position - begin, end - begin))
            < std::numeric_limits<float>::epsilon();
    }

    [[nodiscard]] bool LineStrip::contains(
        Vector2f const& position) const noexcept
    {
        Vector2f normalized = Adapter2D{position}.get();
        for (std::size_t i = 1; i < size(); ++i)
            if (insideInterval(normalized, i) && onLine(normalized, i))
                return true;
        return false;
    }

}

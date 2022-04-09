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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

#include <MPGL/Core/Figures/Elliptic.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    Elliptic::Vertices Elliptic::ellipseVertices(Vector2f const& center,
        Vector2f const& semiAxis, float32 angle)
    {
        Matrix2f rotation = rotationMatrix<float32>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            Vertex{center - rot2},
            Vertex{center + rot1},
            Vertex{center + rot2},
            Vertex{center - rot1}
        };
    }

    Elliptic::Vertices Elliptic::circleVertices(Vector2f const& center,
        float32 radius)
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            Vertex{center - semiMajor + semiMinor},
            Vertex{center + semiMajor + semiMinor},
            Vertex{center + semiMajor - semiMinor},
            Vertex{center - semiMajor - semiMinor}
        };
    }

    Elliptic::Elliptic(Vertices vertices,
        std::string const& programName, Executable exec,
        Color const& color)
            : Figure{programName, std::move(exec)},
            color{color}, vertices{std::move(vertices)}
    {
        initializeBuffers();
    }

    Elliptic::Elliptic(Elliptic const& shape)
        : Figure{shape}, vertices{shape.vertices}
    {
        initializeBuffers();
    }

    void Elliptic::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        BindGuard<VertexBuffer> vboGuard{vertexBuffer};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
        vertexBuffer.setBufferData(vertices);
        vertexArray.setArrayData(vertices.front());
    }

    Elliptic& Elliptic::operator=(Elliptic const& shape) {
        Figure::operator=(shape);
        color = shape.color;
        vertices.clear();
        vertices.reserve(shape.vertices.size());
        std::ranges::copy(shape.vertices, std::back_inserter(vertices));
        return *this;
    }

    void Elliptic::actualizeBufferBeforeDraw(void) const noexcept {
        if (isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{vertexBuffer};
                vertexBuffer.changeBufferData(vertices);
            }
            isModified = false;
        }
    }

}

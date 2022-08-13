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
#include <MPGL/Core/Figures/Primitives/Tetragon.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>

namespace mpgl {

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Tetragon<Dim, Spec>::Tetragon(Color const& color)
        : Angular<Dim, Spec>{4, color}
    {
        initElementBuffer();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Tetragon<Dim, Spec>::Tetragon(
        Vector const& firstVertex,
        Vector const& secondVertex,
        Vector const& thirdVertex,
        Color const& color)
            : Angular<Dim, Spec>{{
                VertexTraits::buildVertex(firstVertex, color),
                VertexTraits::buildVertex(secondVertex, color),
                VertexTraits::buildVertex(thirdVertex, color),
                VertexTraits::buildVertex(firstVertex - secondVertex
                    + thirdVertex, color)}}
    {
        initElementBuffer();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    Tetragon<Dim, Spec>::Tetragon(Tetragon const& tetragon)
        : Angular<Dim, Spec>{tetragon}
    {
        initElementBuffer();
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Tetragon<Dim, Spec>::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    void Tetragon<Dim, Spec>::draw(void) const noexcept {
        drawer(*this);
    }

    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    [[nodiscard]] bool Tetragon<Dim, Spec>::contains(
        Vector2u const& position) const noexcept
    {
        return clicker(*this, position);
    }

    [[nodiscard]] Tetragon2D makeTetragon(
        Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color)
    {
        return Tetragon2D{
            firstVertex,
            firstVertex + 1._y * dimensions[1],
            firstVertex + dimensions,
            color
        };
    }

}

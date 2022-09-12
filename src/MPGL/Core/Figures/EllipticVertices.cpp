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
#include <MPGL/Core/Figures/EllipticVertices.hpp>

namespace mpgl {

    [[nodiscard]] EllipticVertices<dim::Dim2, void>::Vertex
        EllipticVertices<dim::Dim2, void>::buildVertex(
            Vector const& position,
            Color const& color) noexcept
    {
        return Vertex{position, color};
    }

    [[nodiscard]] EllipticVertices<dim::Dim3, void>::Vertex
        EllipticVertices<dim::Dim3, void>::buildVertex(
            Vector const& position,
            Color const& color) noexcept
    {
        return Vertex{position, color};
    }

    [[nodiscard]] EllipticVertices<dim::Dim2, uint8>::Vertex
        EllipticVertices<dim::Dim2, uint8>::buildVertex(
            Vector const& position,
            Color const& color) noexcept
    {
        return Vertex{position, Vector2f{}, color};
    }

    [[nodiscard]] EllipticVertices<dim::Dim3, uint8>::Vertex
        EllipticVertices<dim::Dim3, uint8>::buildVertex(
            Vector const& position,
            Color const& color) noexcept
    {
        return Vertex{position, Vector2f{}, color};
    }

}

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
#include <MPGL/Core/Transformations/Rotation.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    Rotation<dim::Dim2>::Rotation(
        Matrix2f const& rotationMatrix,
        Vector2f const& rotationCenter) noexcept
            : rotationMatrix{rotationMatrix},
            rotationCenter{rotationCenter} {}

    Rotation<dim::Dim2>::Rotation(
        Matrix2f&& rotationMatrix,
        Vector2f&& rotationCenter) noexcept
            : rotationMatrix{std::move(rotationMatrix)},
            rotationCenter{std::move(rotationCenter)} {}

    Rotation<dim::Dim2>::Rotation(
        float32 angle,
        Vector2f const& rotationCenter) noexcept
            : rotationMatrix{mpgl::rotationMatrix<float32>(angle)},
            rotationCenter{rotationCenter} {}

    Rotation<dim::Dim2>::Rotation(
        float32 angle,
        Vector2f&& rotationCenter) noexcept
            : rotationMatrix{mpgl::rotationMatrix<float32>(angle)},
            rotationCenter{std::move(rotationCenter)} {}

    void Rotation<dim::Dim2>::operator() (
        any::InputRange<TransformedType>& coords) const noexcept
    {
        for (auto& coord : coords)
            (*this)(coord);
    }

    void Rotation<dim::Dim2>::operator() (
        TransformedType& coord) const noexcept
    {
        Vector2f radius = Vector2f(coord) - rotationCenter;
        coord = rotationMatrix * radius + rotationCenter;
    }

}

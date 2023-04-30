/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    template <Dimension Dim>
    Figure<Dim>::Figure(std::string const& programName)
        : Shadeable{programName}
    {
        setLocations();
    }

    template <>
    Figure<dim::Dim2>::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram}
    {
        setLocations();
    }

    template <>
    Figure<dim::Dim3>::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram}, Model{shape}
    {
        setLocations();
    }

    template <Dimension Dim>
    Figure<Dim>& Figure<Dim>::operator=(Figure const& shape) {
        if constexpr (ThreeDimensional<Dim>)
            Model::operator=(shape);
        this->shaderProgram = shape.shaderProgram;
        this->isModified = true;
        return *this;
    }

    template <Dimension Dim>
    Figure<Dim>& Figure<Dim>::operator=(Figure&& shape) noexcept {
        if constexpr (ThreeDimensional<Dim>)
            Model::operator=(std::move(shape));
        this->shaderProgram = std::move(shape.shaderProgram);
        this->isModified = shape.isModified;
        return *this;
    }

    template <Dimension Dim>
    void Figure<Dim>::setLocations(void) {
        if constexpr (ThreeDimensional<Dim>) {
            this->locationSetterBuilder(
                this->shaderProgram,
                this->locations);
        }
    }

}

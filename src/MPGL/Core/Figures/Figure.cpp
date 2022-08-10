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
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    Figure<dim::Dim2>::Figure(std::string const& programName)
        : Shadeable{programName} {}

    Figure<dim::Dim2>::Figure(std::string const& programName,
        Executable exec) : Shadeable{programName, exec} {}

    Figure<dim::Dim2>::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram} {}

    Figure<dim::Dim2>& Figure<dim::Dim2>::operator=(
        Figure const& shape)
    {
        shaderProgram = shape.shaderProgram;
        isModified = true;
        return *this;
    }

    Figure<dim::Dim2>& Figure<dim::Dim2>::operator=(
        Figure&& shape) noexcept
    {
        vertexArray = std::move(shape.vertexArray);
        vertexBuffer = std::move(shape.vertexBuffer);
        isModified = std::move(shape.isModified);
        return *this;
    }

    Matrix4f const Figure<dim::Dim3>::defaultModel =
        identityMatrix<float32, 4>();

    Figure<dim::Dim3>::Figure(std::string const& programName)
        : Shadeable{programName} {}

    Figure<dim::Dim3>::Figure(std::string const& programName,
        Executable exec) : Shadeable{programName, exec} {}

    Figure<dim::Dim3>::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram} {}

    Figure<dim::Dim3>& Figure<dim::Dim3>::operator=(
        Figure const& shape)
    {
        shaderProgram = shape.shaderProgram;
        model = shape.model;
        hasModelChanged = isModified = true;
        return *this;
    }

    Figure<dim::Dim3>& Figure<dim::Dim3>::operator=(
        Figure&& shape) noexcept
    {
        vertexArray = std::move(shape.vertexArray);
        vertexBuffer = std::move(shape.vertexBuffer);
        model = std::move(shape.model);
        isModified = std::move(shape.isModified);
        hasModelChanged = std::move(shape.hasModelChanged);
        return *this;
    }

    void Figure<dim::Dim3>::setModel(Matrix4f const& model) noexcept {
        this->model = model;
        hasModelChanged = true;
    }

}

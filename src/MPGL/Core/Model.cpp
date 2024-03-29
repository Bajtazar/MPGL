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
#include <MPGL/Mathematics/Tensors/MatrixFunctions.hpp>
#include <MPGL/Core/Model.hpp>

namespace mpgl {

    Matrix4f const Model::defaultModel = identityMatrix<float32, 4>();

    Model::LocationSetterBuilder const Model::locationSetterBuilder =
        [](ProgramPtr const& program, Locations& locations)
            -> void
    {
        locations.model = ShaderLocation{*program, "model"};
        locations.viewProjection = ShaderLocation{
            *program, "viewProjection"};
    };

    Model::Model(Model const& model)
        : locations{model.locations} {}

    Model& Model::operator=(Model const& model) {
        this->model = model.model;
        locations = model.locations;
        return *this;
    }

    void Model::setModel(Matrix4f const& model) noexcept {
        this->model = model;
    }

    void Model::actualizeLocations(void) const noexcept {
        locations.model(model.get());
        if (context.hasVPChanges())
            locations.viewProjection(context.getViewProjection());
    }

}

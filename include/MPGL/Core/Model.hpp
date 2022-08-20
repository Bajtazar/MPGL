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
#pragma once

#include <MPGL/Utility/Deferred/DelegatePointer.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Mathematics/Tensors/Matrix.hpp>
#include <MPGL/Core/Shaders/Shadeable.hpp>
#include <MPGL/Core/Context/Context.hpp>

namespace mpgl {

    /**
     * Interface for all 3D objects that can be drawn
     */
    class Model : private GraphicalObject {
    public:
        /**
         * Sets the inner figure system model. Matrix has to exist
         * along the figure otherwise using the draw method will
         * result in undefined behaviour
         *
         * @param model a constant reference to the model matrix
         */
        virtual void setModel(Matrix4f const& model) noexcept;

        /**
         * Returns a reference to the inner system model matrix
         *
         * @return the reference to the inner system model matrix
         */
        [[nodiscard]] Matrix4f const& getModel(void) const noexcept
            { return model; }

        /**
         * Virtual destructor. Destroys the model object
         */
        virtual ~Model(void) noexcept = default;
    protected:
        using ProgramPtr = typename Shadeable::ProgramPtr;

        /**
         * Constructs a new model object
         */
        explicit Model(void);

        /**
         * Construct a new model object from the given
         * constant reference
         *
         * @param shape the given shape constant reference
         */
        Model(Model const& model);

        Model(Model&& model) noexcept = default;

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param shape the given figure rvalue reference
         * @return the reference to this object
         */
        Model& operator=(Model const& model);

        Model& operator=(Model&& model) noexcept = default;

        /**
         * Actualizes shader locations' values
         */
        void actualizeLocations(void) const noexcept;

        struct Locations {
            ShaderLocation                      model;
            ShaderLocation                      viewProjection;
        };

        using LocationPtr = DelegatePointer<Locations>;
        using Matrix4fCRef = std::reference_wrapper<Matrix4f const>;
        using LocationSetter = std::function<void(void)>;
        using LocationSetterBuilder = std::function<
            LocationSetter(ProgramPtr const&, LocationPtr const&)>;

        static Matrix4f const                   defaultModel;
        static LocationSetterBuilder const      locationSetterBuilder;

        Matrix4fCRef                            model = defaultModel;
        DelegatePointer<Locations>              locations;
        bool mutable                            hasModelChanged = true;
    };

}

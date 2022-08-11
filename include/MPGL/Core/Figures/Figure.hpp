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

#include <MPGL/Core/Transformations/Transformable.hpp>
#include <MPGL/Utility/Deferred/DelegatePointer.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Core/Figures/Clickable.hpp>
#include <MPGL/Core/Shaders/Shadeable.hpp>
#include <MPGL/Core/Shape.hpp>

namespace mpgl {

    /**
     * Base class for figures
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    class Figure;

    /**
     * Base class for the two-dimensional figures
     */
    template <>
    class Figure<dim::Dim2> :
        public virtual Shape2D,
        public virtual Transformable2D,
        public Shadeable,
        public Clickable
    {
    public:
        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Checks whether given point position is located
         * inside of the figure [boundry is concidered
         * as a part of the figure]
         *
         * @param position the point position
         * @return if point is inside figure
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation2D const& transformator) noexcept = 0;

        /**
         * Destroys the Figure object
         */
        virtual ~Figure(void) noexcept = default;
    protected:
        /**
         * Construct a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         */
        explicit Figure(std::string const& programName);

        /**
         * Constructs a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         * @param exec the shader's executable
         */
        explicit Figure(
            std::string const& programName,
            Executable exec);

        /**
         *  Constructs a new Figure object
         */
        explicit Figure(void) noexcept = default;

        /**
         * Construct a new Figure object from the given
         * constant reference
         *
         * @param shape the given shape constant reference
         */
        explicit Figure(Figure const& shape);

        explicit Figure(Figure&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given figure constant reference
         * @return the reference to this object
         */
        Figure& operator=(Figure const& shape);

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param shape the given figure rvalue reference
         * @return the reference to this object
         */
        Figure& operator=(Figure&& shape) noexcept;
    };

    /**
     * Base class for the three-dimensional figures
     */
    template <>
    class Figure<dim::Dim3> :
        public virtual Shape3D,
        public virtual Transformable3D,
        public Shadeable
    {
    public:
        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

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
         * Pure virtual method. Has to be overloaded.
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation3D const& transformator) noexcept = 0;

        /**
         * Destroys the Figure object
         */
        virtual ~Figure(void) noexcept = default;
    protected:
        /**
         * Construct a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         */
        explicit Figure(std::string const& programName);

        /**
         * Constructs a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         * @param exec the shader's executable
         */
        explicit Figure(
            std::string const& programName,
            Executable exec);

        /**
         *  Constructs a new Figure object
         */
        explicit Figure(void);

        /**
         * Construct a new Figure object from the given
         * constant reference
         *
         * @param shape the given shape constant reference
         */
        explicit Figure(Figure const& shape);

        explicit Figure(Figure&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given figure constant reference
         * @return the reference to this object
         */
        Figure& operator=(Figure const& shape);

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param shape the given figure rvalue reference
         * @return the reference to this object
         */
        Figure& operator=(Figure&& shape) noexcept;

        /**
         * Sets the shader locations
         */
        void setLocations(void);

        /**
         * Actualizes shader locations' values
         */
        void actualizeLocations(void) const noexcept;

        using Matrix4fCRef = std::reference_wrapper<Matrix4f const>;

        static Matrix4f const                   defaultModel;

        struct Locations {
            ShaderLocation                      model;
            ShaderLocation                      viewProjection;
        };

        Matrix4fCRef                            model = defaultModel;
        DelegatePointer<Locations>              locations;
        bool mutable                            hasModelChanged = true;
    };

    typedef Figure<dim::Dim2>                   Figure2D;
    typedef Figure<dim::Dim3>                   Figure3D;

}

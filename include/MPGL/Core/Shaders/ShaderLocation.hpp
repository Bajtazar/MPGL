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

#include <MPGL/Mathematics/Tensors/Matrix.hpp>
#include <MPGL/Core/Shaders/ShaderProgram.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Core/Color.hpp>

namespace mpgl {

    /**
     * Grants access to the location in the shader
     */
    class ShaderLocation {
    public:
        /**
         * Constructs a new Shader Location object
         */
        explicit ShaderLocation(void) noexcept;

        /**
         * Construct a new Shader Location object
         *
         * @param program the constant reference to the shader program
         * @param uniform the location name
         * @throws ShaderLocationInvalidProgramException
         * when the given shader program is not linked
         * or does not exist
         * @throws ShaderLocationUniformException when the given
         * uniform does not exist
         */
        explicit ShaderLocation(
            ShaderProgram const& program,
            std::string const& uniform);

        /**
         * Sets the integer uniform
         *
         * @tparam Ints the number of setted integers
         * @param values the uniform's values
         */
        template <AllIntegrals... Ints>
            requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
                && !AllUnsignedIntegrals<Ints...>)
        inline void operator()(Ints... values) const noexcept;

        /**
         * Sets the unsigned integer uniform
         *
         * @tparam UInts the number of setted unsigned integers
         * @param values the uniform's values
         */
        template <AllUnsignedIntegrals... UInts>
            requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
        inline void operator()(UInts... values) const noexcept;

        /**
         * Sets the floating point uniform
         *
         * @tparam UInts the number of setted floating points
         * @param values the uniform's values
         */
        template <AllFloatingPoints... Floats>
            requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
        inline void operator()(Floats... values) const noexcept;

        /**
         * Sets the color unifrom
         *
         * @param color the uniform's color
         */
        inline void operator()(Color const& color) const noexcept;

        /**
         * Sets the vector unifrom
         *
         * @param Tp the vector's type
         * @param Size the vector's size
         * @param vector the uniform's vector
         */
        template <Arithmetic Tp, std::size_t Size>
            requires (Size <= 4 && Size > 1)
        inline void operator()(
            Vector<Tp, Size> const& vector) const noexcept;

        /**
         * Sets the matrix uniform
         *
         * @param Tp the matrix's type
         * @param Rows the matrix's rows
         * @param Columns the matrix's columns
         * @param matrix the uniform's matrix
         */
        template <std::size_t Rows, std::size_t Columns>
            requires (Rows <= 4 && Rows > 1
                && Columns <= 4 && Columns > 1)
        inline void operator()(
            Matrix<float32, Rows, Columns> const& matrix) const noexcept;

        /**
         * Returns whether the given shader location is valid
         *
         * @return if the given shader location is valid
         */
        [[nodiscard]] bool isValid(void) const noexcept
            { return location == 0xFFFFFFFF; }

        /**
         * Returns whether the given shader location is valid
         *
         * @return if the given shader location is valid
         */
        [[nodiscard]] operator bool() const noexcept
            { return isValid(); }
    private:
        uint32                                      location;

        /**
         * Sets the value of the location
         *
         * @param program the constant reference to the shader
         * program
         * @param uniform the location name
         * @throws ShaderLocationInvalidProgramException
         * when the given shader program is not linked
         * or does not exist
         * @throws ShaderLocationUniformException when the given
         * uniform does not exist
         */
        void setLocation(
            ShaderProgram const& program,
            std::string const& uniform);
    };

}

#include <MPGL/Core/Shaders/ShaderLocation.ipp>
#include <MPGL/Core/Shaders/ShaderLocation.tpp>

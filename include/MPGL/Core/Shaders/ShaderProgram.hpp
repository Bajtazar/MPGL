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

#include <MPGL/Core/Shaders/Shader.hpp>
#include <MPGL/Mathematics/Matrix.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Core/Color.hpp>

#include <memory>

namespace mpgl {

    /**
     * Manages the OpenGL shader program object
     */
    class ShaderProgram {
    public:
        /**
         * Constructs a new Shader Program object
         */
        explicit ShaderProgram(void) noexcept;

        /**
         * Constructs a new Shader Program object from the
         * given vertex and fragment shader
         *
         * @param vertex the constant reference to the vertex
         * shader
         * @param fragment the constant reference to the fragment
         * shader
         */
        explicit ShaderProgram(
            VertexShader const& vertex,
            FragmentShader const& fragment) noexcept;

        /**
         * Attaches the shader of the given type to the shader program
         *
         * @tparam Type the type of the shader
         * @param shader the constant reference to the shader object
         */
        template <bool Type>
        void attachShader(Shader<Type> const& shader) const noexcept;

        /**
         * Links the shader program
         *
         * @throw ShaderProgramLinkingException when the linking
         * cannot be completed
         * @param programName the constant reference to the string
         * with the shader program name [for the debugging purposes]
         */
        void link(std::string const& programName = {}) const;

        /**
         * Returns whether the shader program exist
         *
         * @return if the shader program exist
         */
        [[nodiscard]] bool exist(void) const noexcept
            { return *shaderProgramID != 0; }

        /**
         * Returns whether the shader program exist
         *
         * @return if the shader program exist
         */
        [[nodiscard]] operator bool() const noexcept
            { return exist(); }

        /**
         * Uses the shader program
         */
        inline void use(void) const noexcept;

        /**
         * Sets the integer uniform
         *
         * @tparam Ints the number of setted integers
         * @param uniform the uniform's name
         * @param values the uniform's values
         */
        template <AllIntegrals... Ints>
            requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
                && !AllUnsignedIntegrals<Ints...>)
        inline void setUniform(
            std::string const& uniform,
            Ints... values) const noexcept;

        /**
         * Sets the unsigned integer uniform
         *
         * @tparam UInts the number of setted unsigned integers
         * @param uniform the uniform's name
         * @param values the uniform's values
         */
        template <AllUnsignedIntegrals... UInts>
            requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
        inline void setUniform(
            std::string const& uniform,
            UInts... values) const noexcept;

        /**
         * Sets the floating point uniform
         *
         * @tparam UInts the number of setted floating points
         * @param uniform the uniform's name
         * @param values the uniform's values
         */
        template <AllFloatingPoints... Floats>
            requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
        inline void setUniform(
            std::string const& uniform,
            Floats... values) const noexcept;

        /**
         * Sets the color unifrom
         *
         * @param uniform the uniform's name
         * @param color the uniform's color
         */
        inline void setUniform(
            std::string const& uniform,
            Color const& color) const noexcept;

        /**
         * Sets the vector unifrom
         *
         * @param Tp the vector's type
         * @param Size the vector's size
         * @param uniform the uniform's name
         * @param vector the uniform's vector
         */
        template <Arithmetic Tp, std::size_t Size>
            requires (Size <= 4 && Size > 1)
        inline void setUniform(
            std::string const& uniform,
            Vector<Tp, Size> const& vector) const noexcept;

        /**
         * Sets the matrix uniform
         *
         * @param Tp the matrix's type
         * @param Size the matrix's size
         * @param uniform the uniform's name
         * @param matrix the uniform's matrix
         */
        template <std::size_t Size>
            requires (Size <= 4 && Size > 1)
        inline void setUniform(
            std::string const& uniform,
            Matrix<float32, Size, Size> const& matrix) const noexcept;
    private:
        /**
         * Program called upon shader program id destruction
         */
        class ProgramDeleter {
        public:
            /**
             * Constructs a new Program Deleter object
             */
            explicit ProgramDeleter(void) noexcept = default;

            /**
             * Called upon the destruction of the shader program id
             * pointer. Deletes the OpenGL shader program and deletes
             * pointer
             *
             * @param ptr the pointer with shader program id
             */
            void operator()(uint32* ptr) const noexcept;
        };

        /**
         * Returns the location of the uniform
         *
         * @param uniform the uniform name
         * @return the location of the uniform
         */
        inline uint32 location(
            std::string const& uniform) const noexcept;

        /**
         * Verifies the shader program linking status
         *
         * @throw ShaderProgramLinkingException when linking
         * failed
         * @param filePath the path to the shader file
         */
        void verifyLinkingStatus(std::string const& filePath) const;

        std::shared_ptr<uint32>             shaderProgramID;

        static uint32                       lastProgramID;
    };

    inline void ShaderProgram::use(void) const noexcept {
        if (lastProgramID != *shaderProgramID) {
            glUseProgram(*shaderProgramID);
            lastProgramID = *shaderProgramID;
        }
    }

    template <bool Type>
    void ShaderProgram::attachShader(
        Shader<Type> const& shader) const noexcept
    {
        glAttachShader(*shaderProgramID, shader.getShader());
    }

    inline uint32 ShaderProgram::location(
        std::string const& uniform) const noexcept
    {
        return glGetUniformLocation(*shaderProgramID, uniform.c_str());
    }

    template <AllIntegrals... Ints>
        requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
            && !AllUnsignedIntegrals<Ints...>)
    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        Ints... values) const noexcept
    {
        if constexpr (sizeof...(Ints) == 1)
            glUniform1i(location(uniform), values...);
        else if constexpr (sizeof...(Ints) == 2)
            glUniform2i(location(uniform), values...);
        else if constexpr (sizeof...(Ints) == 3)
            glUniform3i(location(uniform), values...);
        else
            glUniform4i(location(uniform), values...);
    }

    template <AllUnsignedIntegrals... UInts>
        requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        UInts... values) const noexcept
    {
        if constexpr (sizeof...(UInts) == 1)
            glUniform1ui(location(uniform), values...);
        else if constexpr (sizeof...(UInts) == 2)
            glUniform2ui(location(uniform), values...);
        else if constexpr (sizeof...(UInts) == 3)
            glUniform3ui(location(uniform), values...);
        else
            glUniform4ui(location(uniform), values...);
    }

    template <AllFloatingPoints... Floats>
        requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        Floats... values) const noexcept
    {
        if constexpr (sizeof...(Floats) == 1)
            glUniform1f(location(uniform), values...);
        else if constexpr (sizeof...(Floats) == 2)
            glUniform2f(location(uniform), values...);
        else if constexpr (sizeof...(Floats) == 3)
            glUniform3f(location(uniform), values...);
        else
            glUniform4f(location(uniform), values...);
    }

    template <Arithmetic Tp, std::size_t Size>
        requires (Size <= 4 && Size > 1)
    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        Vector<Tp, Size> const& vector) const noexcept
    {
        [&]<std::size_t... Index>(std::index_sequence<Index...>) {
            setUniform(uniform, std::get<Size - Index - 1>(
                static_cast<UniformTuple<Tp, Size> const&>(vector))...);
        }(std::make_index_sequence<Size>{});
    }

    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        Color const& color) const noexcept
    {
        setUniform(uniform, static_cast<Vector4f const&>(color));
    }

    template <std::size_t Size>
        requires (Size <= 4 && Size > 1)
    inline void ShaderProgram::setUniform(
        std::string const& uniform,
        Matrix<float32, Size, Size> const& matrix) const noexcept
    {
        float32 const* const memory = reinterpret_cast<
            float32 const* const>(&matrix);
        if constexpr (Size == 4)
            glUniformMatrix4fv(location(uniform), 1, GL_FALSE, memory);
        else if constexpr (Size == 3)
            glUniformMatrix3fv(location(uniform), 1, GL_FALSE, memory);
        else
            glUniformMatrix2fv(location(uniform), 1, GL_FALSE, memory);
    }

}

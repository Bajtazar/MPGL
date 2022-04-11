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

namespace mpgl {

    template <bool Type>
    void ShaderProgram::attachShader(
        Shader<Type> const& shader) const noexcept
    {
        glAttachShader(*shaderProgramID, shader.getShader());
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

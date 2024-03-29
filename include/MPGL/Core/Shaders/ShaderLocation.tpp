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
#pragma once

namespace mpgl {

   template <AllIntegrals... Ints>
        requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
            && !AllUnsignedIntegrals<Ints...>)
    inline void ShaderLocation::operator() (
        Ints... values) const noexcept
    {
        if constexpr (sizeof...(Ints) == 1)
            glUniform1i(location, values...);
        else if constexpr (sizeof...(Ints) == 2)
            glUniform2i(location, values...);
        else if constexpr (sizeof...(Ints) == 3)
            glUniform3i(location, values...);
        else
            glUniform4i(location, values...);
    }

    template <AllUnsignedIntegrals... UInts>
        requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
    inline void ShaderLocation::operator() (
        UInts... values) const noexcept
    {
        if constexpr (sizeof...(UInts) == 1)
            glUniform1ui(location, values...);
        else if constexpr (sizeof...(UInts) == 2)
            glUniform2ui(location, values...);
        else if constexpr (sizeof...(UInts) == 3)
            glUniform3ui(location, values...);
        else
            glUniform4ui(location, values...);
    }

    template <AllFloatingPoints... Floats>
        requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
    inline void ShaderLocation::operator() (
        Floats... values) const noexcept
    {
        if constexpr (sizeof...(Floats) == 1)
            glUniform1f(location, values...);
        else if constexpr (sizeof...(Floats) == 2)
            glUniform2f(location, values...);
        else if constexpr (sizeof...(Floats) == 3)
            glUniform3f(location, values...);
        else
            glUniform4f(location, values...);
    }

    template <Arithmetic Tp, std::size_t Size>
        requires (Size <= 4 && Size > 1)
    inline void ShaderLocation::operator() (
        Vector<Tp, Size> const& vector) const noexcept
    {
        [&]<std::size_t... Index>(std::index_sequence<Index...>) {
            (*this)(vector[Index]...);
        }(std::make_index_sequence<Size>{});
    }

    template <std::size_t Rows, std::size_t Columns>
        requires (Rows <= 4 && Rows > 1
            && Columns <= 4 && Columns > 1)
    inline void ShaderLocation::operator() (
        Matrix<float32, Rows, Columns> const& matrix) const noexcept
    {
        float32 const* const memory = reinterpret_cast<
            float32 const* const>(&matrix);
        if constexpr (Columns == 2 && Rows == 2)
            glUniformMatrix2fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 3 && Rows == 2)
            glUniformMatrix3x2fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 4 && Rows == 2)
            glUniformMatrix4x2fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 2 && Rows == 3)
            glUniformMatrix2x3fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 3 && Rows == 3)
            glUniformMatrix3fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 4 && Rows == 3)
            glUniformMatrix4x3fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 2 && Rows == 4)
            glUniformMatrix2x4fv(location, 1, GL_TRUE, memory);
        else if constexpr (Columns == 3 && Rows == 4)
            glUniformMatrix3x4fv(location, 1, GL_TRUE, memory);
        else
            glUniformMatrix4fv(location, 1, GL_TRUE, memory);
    }

}

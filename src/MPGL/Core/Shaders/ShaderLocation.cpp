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
#include <MPGL/Exceptions/Shader/ShaderLocationInvalidProgramException.hpp>
#include <MPGL/Exceptions/Shader/ShaderLocationUniformException.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>

namespace mpgl {

    static constexpr mpgl::uint32 UninitializedLocation = 0xFFFFFFFF;

    ShaderLocation::ShaderLocation(void) noexcept
        : location{UninitializedLocation} {}

    ShaderLocation::ShaderLocation(
        ShaderProgram const& program,
        std::string const& uniform)
            : location{UninitializedLocation}
    {
        setLocation(program, uniform);
    }

    void ShaderLocation::setLocation(
        ShaderProgram const& program,
        std::string const& uniform)
    {
        if (!program)
            throw ShaderLocationInvalidProgramException{};
        program.use();
        location = glGetUniformLocation(*program.shaderProgramID,
            uniform.c_str());
        if (location == UninitializedLocation)
            throw ShaderLocationUniformException{uniform};
    }

}

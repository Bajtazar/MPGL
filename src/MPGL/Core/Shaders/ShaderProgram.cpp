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
#include <MPGL/Exceptions/Shader/ShaderProgramLinkingException.hpp>
#include <MPGL/Core/Shaders/ShaderProgram.hpp>
#include <MPGL/Utility/Ranges.hpp>

namespace mpgl {

    uint32 ShaderProgram::lastProgramID = 0u;

    ShaderProgram::ShaderProgram(void) noexcept
        : shaderProgramID{new uint32{0}, ProgramDeleter{}} {}

    ShaderProgram::ShaderProgram(
        VertexShader const& vertex,
        FragmentShader const& fragment) noexcept
            : shaderProgramID{new uint32{glCreateProgram()},
                ProgramDeleter{}}
    {
        attachShader(vertex);
        attachShader(fragment);
    }

    void ShaderProgram::ProgramDeleter::operator() (
        uint32* ptr) const noexcept
    {
        if (*ptr)
            glDeleteProgram(*ptr);
        delete ptr;
    }

    bool ShaderProgram::isLinked(void) const noexcept {
        int32 status = 0;
        glGetProgramiv(*shaderProgramID, GL_LINK_STATUS, &status);
        return status != 0;
    }

    void ShaderProgram::verifyLinkingStatus(
        std::string const& programName) const
    {
        if (!isLinked()) {
            std::string info;
            info.resize(512);
            glGetProgramInfoLog(*shaderProgramID, 512, nullptr,
                info.data());
            throw ShaderProgramLinkingException{
                "[" + programName + "]\t" + info};
        }
    }

    void ShaderProgram::link(std::string const& programName) const {
        glLinkProgram(*shaderProgramID);
        verifyLinkingStatus(programName);
    }

}

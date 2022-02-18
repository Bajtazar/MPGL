#include "ShaderProgram.hpp"

#include "../../IO/Logger.hpp"
#include "../../Exceptions/ShaderProgramLinkingException.hpp"

namespace mpgl {

    uint32 ShaderProgram::lastProgramID = 0u;

    ShaderProgram::ShaderProgram(void) noexcept : shaderProgramID{
        new uint32{0}, ProgramDeleter{}} {}

    ShaderProgram::ShaderProgram(VertexShader const& vertex,
        FragmentShader const& fragment) noexcept : shaderProgramID{
            new uint32{glCreateProgram()}, ProgramDeleter{}}
    {
        attachShader(vertex);
        attachShader(fragment);
    }

    void ShaderProgram::ProgramDeleter::operator() (uint32* ptr) const noexcept {
        glDeleteProgram(*ptr);
        delete ptr;
    }

    void ShaderProgram::link(std::string const& programName) const {
        glLinkProgram(*shaderProgramID);
        Logger::checkCompilationStatus<ShaderProgramLinkingException>(
            *shaderProgramID, GL_LINK_STATUS, "Shader Program][" + programName);
    }

}

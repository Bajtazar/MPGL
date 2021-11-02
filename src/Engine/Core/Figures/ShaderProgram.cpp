#include "ShaderProgram.hpp"

#include "../../IO/Logger.hpp"
#include "../../Exceptions/ShaderProgramLinkingException.hpp"

namespace ge {

    uint32_t ShaderProgram::lastProgramID = 0u;

    ShaderProgram::ShaderProgram(void) noexcept : shaderProgramID{
        new uint32_t{0}, ProgramDeleter{}} {}

    ShaderProgram::ShaderProgram(VertexShader const& vertex,
        FragmentShader const& fragment) noexcept : shaderProgramID{
            new uint32_t{glCreateProgram()}, ProgramDeleter{}}
    {
        attachShader(vertex);
        attachShader(fragment);
    }

    void ShaderProgram::ProgramDeleter::operator() (uint32_t* ptr) const noexcept {
        glDeleteProgram(*ptr);
        delete ptr;
    }

    void ShaderProgram::link(void) const {
        glLinkProgram(*shaderProgramID);
        Logger::checkCompilationStatus<ShaderProgramLinkingException>(
            *shaderProgramID, GL_LINK_STATUS, "ShaderProgram");
    }

}

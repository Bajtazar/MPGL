#include "ShaderProgram.hpp"

#include "../../IO/Logger.hpp"
#include "../../Exceptions/ShaderProgramLinkingException.hpp"

namespace ge {

    ShaderProgram::ShaderProgram(void) noexcept : shaderProgramID{
        std::make_shared<uint32_t>(0)} {}

    ShaderProgram::ShaderProgram(VertexShader const& vertex,
        FragmentShader const& fragment) noexcept : shaderProgramID{
            std::make_shared<uint32_t>(glCreateProgram())}
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

#include "ShaderLibrary.hpp"
#include "../../Utility/FileIO.hpp"
#include "../../Utility/Logger.hpp"
#include "../../Exceptions/ShaderLibraryInvalidShadersException.hpp"
#include "../../Exceptions/ShaderProgramLinkingException.hpp"

#include <algorithm>

namespace ge {

    ShaderLibrary::ShaderLibrary(void) {
        for (const std::string& shader : getShaderList()) {
            VertexShader vertex{"shaders/Vertex/" + shader};
            FragmentShader fragment{"shaders/Fragment/" + shader};
            uint32_t programID = glCreateProgram();
            glAttachShader(programID, vertex.getShader());
            glAttachShader(programID, fragment.getShader());
            glLinkProgram(programID);
            int32_t success;
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                std::string info = Logger::loggingString(512, 0);
                glGetProgramInfoLog(programID, 512, nullptr, info.data());
                Logger::saveOpenGl(info, "Shader linker");
                throw ShaderProgramLinkingException{info};
            }
            programs[shader.substr(0, shader.find('.'))] = programID;
        }
    }

    ShaderLibrary& ShaderLibrary::operator=(ShaderLibrary&& shaderLib) noexcept {
        programs = std::move(shaderLib.programs);
        return *this;
    }

    std::vector<std::string> ShaderLibrary::getShaderList(void) const {
        auto vertex = FileIO::getAllDirectoryFiles("shaders/Vertex");
        auto fragment = FileIO::getAllDirectoryFiles("shaders/Fragment");
        if (!std::ranges::equal(vertex, fragment, [](const auto& left, const auto& right){ return left.substr(15) == right.substr(17); }))
            throw ShaderLibraryInvalidShadersException{vertex, fragment};
        std::vector<std::string> shaders;
        std::ranges::transform(vertex, std::back_inserter(shaders), [](const auto& path){ return path.substr(15); });
        return shaders;
    }

    ShaderLibrary::~ShaderLibrary(void) noexcept {
        for (const auto& [_, program] : programs)
            glDeleteProgram(program);
    }

}

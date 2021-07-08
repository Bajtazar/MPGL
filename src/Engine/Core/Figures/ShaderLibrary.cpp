#include "ShaderLibrary.hpp"
#include "../../Utility/FileIO.hpp"
#include "../../Utility/Logger.hpp"

using namespace std::literals;

namespace ge {

    ShaderLibrary::ShaderLibrary(void) noexcept {
        if (std::optional<FileIO::stream> handle = FileIO::readFile("shaders/shaders.si")) {
            for (std::string s1, s2, s3; *handle >> s1 && *handle >> s2 && *handle >> s3;) {
                VertexShader vertex("shaders/Vertex/"s + s2);
                FragmentShader fragment("shaders/Fragment/" + s3);
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
                }
                programs[s1] = programID;
            }
        }
    }

    ShaderLibrary& ShaderLibrary::operator=(ShaderLibrary&& shaderLib) noexcept {
        programs = std::move(shaderLib.programs);
        return *this;
    }

    ShaderLibrary::~ShaderLibrary(void) noexcept {
        for (const auto& [_, program] : programs)
            glDeleteProgram(program);
    }

}

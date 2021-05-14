#include "Shader.hpp"
#include "../../Utility/Logger.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>

namespace ge {

    template <bool ShaderType>
    Shader<ShaderType>::Shader(std::string shaderPath) noexcept {
        if constexpr (ShaderType)
            shaderID = glCreateShader(GL_VERTEX_SHADER);
        else
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        loadShaderFromFile(std::move(shaderPath));
        glShaderSource(shaderID, 1, &handle, nullptr);
        glCompileShader(shaderID);
        int32_t success;
        glGetProgramiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[512];
            glGetProgramInfoLog(shaderID, 512, nullptr, info);
            Logger::saveOpenGl(info, 512);
        }
    }

    template <bool ShaderType>
    Shader<ShaderType>& Shader<ShaderType>::operator= (Shader&& shader) noexcept {
        shaderID = std::move(shaderID);
        return *this;
    }

    template <bool ShaderType>
    Shader<ShaderType>::~Shader(void) noexcept {
        glDeleteShader(shaderID);
    }

    template <bool ShaderType>
    void Shader<ShaderType>::loadShaderFromFile(std::string&& shaderPath) noexcept {
        std::ifstream file(shaderPath.c_str(), std::ios_base::binary);
        if (file.is_open() && file.good()) {
            std::stringstream oss;
            std::copy(std::istreambuf_iterator<char>(file), {}, std::ostreambuf_iterator<char>(oss));
            shaderCode = std::move(oss.str());
            handle = shaderCode.c_str();
        }
    }

}
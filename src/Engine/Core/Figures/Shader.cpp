#include "Shader.hpp"

#include "../../IO/FileIO.hpp"
#include "../../Utility/Logger.hpp"
#include "../../Exceptions/ShaderCompilationException.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>

namespace ge {

    template <bool ShaderType>
    constexpr auto Shader<ShaderType>::shaderType(void) const noexcept {
        if constexpr (ShaderType)
            return GL_VERTEX_SHADER;
        else
            return GL_FRAGMENT_SHADER;
    }

    template <bool ShaderType>
    Shader<ShaderType>::Shader(std::string shaderPath) : shaderID{glCreateShader(shaderType())} {
        if (auto shaderCode = FileIO::readFile(shaderPath)) {
            std::string stream = std::move(shaderCode->str());
            const char* codePointer = stream.c_str();
            glShaderSource(shaderID, 1, &codePointer, nullptr);
            glCompileShader(shaderID);
            Logger::checkCompilationStatus<ShaderCompilationException>(
                shaderID, GL_COMPILE_STATUS, "Shader compiler");
        } else
            throw ShaderCompilationException{"Shader cannot be loaded from a file"};
    }

    template <bool ShaderType>
    Shader<ShaderType>& Shader<ShaderType>::operator= (Shader&& shader) noexcept {
        shaderID = shader.shaderID;
        shader.shaderID = 0;
        return *this;
    }

    template <bool ShaderType>
    Shader<ShaderType>::~Shader(void) noexcept {
        glDeleteShader(shaderID);
    }

}
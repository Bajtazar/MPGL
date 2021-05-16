#pragma once

#include <glad/glad.h>
#include <string>

namespace ge {

    template <bool ShaderType>
    class Shader {
    public:
        explicit Shader(std::string shaderPath) noexcept;
        Shader(const Shader& shader) noexcept = delete;
        Shader(Shader&& shader) noexcept : shaderID(std::move(shader.shaderID)) {}

        Shader& operator= (const Shader& shader) noexcept = delete;
        Shader& operator= (Shader&& shader) noexcept;

        const uint32_t& getShader(void) const noexcept { return shaderID; }
        ~Shader(void) noexcept;
    private:
        std::string shaderCode;
        uint32_t shaderID;
        const char* handle;
        void loadShaderFromFile(std::string&& shaderPath) noexcept;
    };

    template class Shader<true>;
    template class Shader<false>;

    typedef Shader<true> VertexShader;
    typedef Shader<false> FragmentShader;

}
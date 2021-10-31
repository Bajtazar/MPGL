#pragma once

#include <glad/glad.h>
#include <string>

namespace ge {

    template <bool ShaderType>
    class Shader {
    public:
        explicit Shader(std::string shaderPath);
        Shader(Shader const& shader) noexcept = delete;
        Shader(Shader&& shader) noexcept
            : shaderID{shader.shaderID} { shader.shaderID = 0; }

        Shader& operator= (Shader const& shader) noexcept = delete;
        Shader& operator= (Shader&& shader) noexcept;

        uint32_t const& getShader(void) const noexcept
            { return shaderID; }
        ~Shader(void) noexcept;
    private:
        uint32_t                        shaderID;

        constexpr auto shaderType(void) const noexcept;
    };

    template class Shader<true>;
    template class Shader<false>;

    typedef Shader<true>                VertexShader;
    typedef Shader<false>               FragmentShader;

}
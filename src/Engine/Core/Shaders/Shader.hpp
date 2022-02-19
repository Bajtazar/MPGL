#pragma once

#include <glad/glad.h>
#include <string>

#include "../../Traits/Types.hpp"

namespace mpgl {

    template <bool ShaderType>
    class Shader {
    public:
        explicit Shader(std::string shaderPath);
        Shader(Shader const& shader) noexcept = delete;
        Shader(Shader&& shader) noexcept
            : shaderID{shader.shaderID} { shader.shaderID = 0; }

        Shader& operator= (Shader const& shader) noexcept = delete;
        Shader& operator= (Shader&& shader) noexcept;

        uint32 const& getShader(void) const noexcept
            { return shaderID; }
        ~Shader(void) noexcept;
    private:
        uint32                          shaderID;

        constexpr auto shaderType(void) const noexcept;
    };

    template class Shader<true>;
    template class Shader<false>;

    typedef Shader<true>                VertexShader;
    typedef Shader<false>               FragmentShader;

}
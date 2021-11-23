#include "Shadeable.hpp"

namespace ge {

    Shadeable::Shadeable(void) noexcept
        : shaderProgram{new ShaderProgram{}} {}

    Shadeable::Shadeable(std::string const& name) : Shadeable{} {
        context.shaders.setOrQueue(shaderProgram, name);
    }

    Shadeable::Shadeable(std::string const& name,
        ShadersContext::Executable exec) : Shadeable{}
    {
        context.shaders.setOrQueue(shaderProgram, name, std::move(exec));
    }

}

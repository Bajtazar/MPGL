#include "ShaderLibrary.hpp"

#include "../../IO/FileIO.hpp"
#include "../../IO/Logger.hpp"
#include "../../Exceptions/ShaderLibraryInvalidShadersException.hpp"
#include "../../Exceptions/ShaderProgramLinkingException.hpp"

#include <algorithm>

namespace ge {

    ShaderLibrary::ShaderLibrary(void) {
        for (const std::string& shader : getShaderList()) {
            VertexShader vertex{"shaders/Vertex/" + shader};
            FragmentShader fragment{"shaders/Fragment/" + shader};
            ShaderProgram program{vertex, fragment};
            program.link();
            programs.emplace(shader.substr(0, shader.find('.')), std::move(program));
        }
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

}

/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include <MPGL/Exceptions/ShaderLibraryInvalidShadersException.hpp>

#include <MPGL/Exceptions/ShaderProgramLinkingException.hpp>
#include <MPGL/Core/Shaders/ShaderLibrary.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <algorithm>

#ifndef MPGL_SOURCE_DIR
#define MPGL_SOURCE_DIR "."
#endif

namespace mpgl {

    std::string const ShaderLibrary::vertexShadersPath
        = std::string(MPGL_SOURCE_DIR) + "/shaders/Vertex";

    std::string const ShaderLibrary::fragmentShadersPath
        = std::string(MPGL_SOURCE_DIR) + "/shaders/Fragment";

    ShaderLibrary::ShaderLibrary(void) {
        for (std::string const& shader : getShaderList()) {
            VertexShader vertex{vertexShadersPath + "/" + shader};
            FragmentShader fragment{fragmentShadersPath + "/" + shader};
            ShaderProgram program{vertex, fragment};
            program.link(shader);
            programs.emplace(shader.substr(0,
                shader.find_last_of('.')), std::move(program));
        }
    }

    bool ShaderLibrary::sameShaders(
        Paths const& vertexShaders,
        Paths const& fragmentShaders) noexcept
    {
        auto compare = [](auto const& left, auto const& right) -> bool {
            return left.substr(vertexShadersPath.size() + 1)
                == right.substr(fragmentShadersPath.size() + 1);
        };
        return std::ranges::equal(vertexShaders, fragmentShaders,
            compare);
    }

    ShaderLibrary::Paths ShaderLibrary::getShaderList(void) const {
        auto vertex = FileIO::getRecursiveDirFiles(vertexShadersPath);
        auto fragment = FileIO::getRecursiveDirFiles(fragmentShadersPath);
        if (!sameShaders(vertex, fragment))
            throw ShaderLibraryInvalidShadersException{vertex, fragment};
        Paths shaders;
        std::ranges::transform(vertex, std::back_inserter(shaders),
            [](auto const& path)
                { return path.substr(vertexShadersPath.size() + 1); });
        return shaders;
    }

}

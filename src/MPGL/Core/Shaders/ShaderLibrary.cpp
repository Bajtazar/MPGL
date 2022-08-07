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
#include <MPGL/Exceptions/Shader/ShaderLibraryInvalidShadersException.hpp>
#include <MPGL/Exceptions/Shader/ShaderProgramLinkingException.hpp>
#include <MPGL/Core/Shaders/ShaderLibrary.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <algorithm>

namespace mpgl {

    ShaderLibrary::Path ShaderLibrary::vertexShaders(
        Path const& path)
    {
        return path + "/Vertex";
    }

    ShaderLibrary::Path ShaderLibrary::fragmentShaders(
        Path const& path)
    {
        return path + "/Fragment";
    }

    ShaderLibrary::ShaderLibrary(Paths const& locations) {
        for (Path const& path : locations) {
            for (std::string const& shader : getShaderList(path)) {
                VertexShader vertex{vertexShaders(path) + "/" + shader};
                FragmentShader fragment{fragmentShaders(path) + "/" + shader};
                ShaderProgram program{vertex, fragment};
                program.link(shader);
                programs.emplace(shader.substr(0,
                    shader.find_last_of('.')), std::move(program));
            }
        }
    }

    bool ShaderLibrary::sameShaders(
        Paths const& vertexShadersPaths,
        Paths const& fragmentShadersPaths,
        Path const& path) noexcept
    {
        auto compare = [&path](auto const& left,
            auto const& right) -> bool
        {
            return left.substr(vertexShaders(path).size() + 1)
                == right.substr(fragmentShaders(path).size() + 1);
        };
        return std::ranges::equal(vertexShadersPaths,
            fragmentShadersPaths, compare);
    }

    ShaderLibrary::Paths ShaderLibrary::getShaderList(
        Path const& path) const
    {
        auto vertex = FileIO::getRecursiveDirFiles(vertexShaders(path));
        auto fragment = FileIO::getRecursiveDirFiles(fragmentShaders(path));
        if (!sameShaders(vertex, fragment, path))
            throw ShaderLibraryInvalidShadersException{vertex, fragment};
        Paths shaders;
        std::ranges::transform(vertex, std::back_inserter(shaders),
            [dir=path](auto const& path)
                { return path.substr(vertexShaders(dir).size() + 1); });
        return shaders;
    }

}

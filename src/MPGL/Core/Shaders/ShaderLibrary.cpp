/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Core/Shaders/ShaderLibrary.hpp>

#include <filesystem>
#include <algorithm>

namespace mpgl {

    ShaderLibrary::Path ShaderLibrary::vertexShaders(
        Path const& path)
    {
        return path.size() ? path + "/Vertex" : "Vertex";
    }

    ShaderLibrary::Path ShaderLibrary::fragmentShaders(
        Path const& path)
    {
        return path.size() ? path + "/Fragment" : "Fragment";
    }

    ShaderLibrary::ShaderLibrary(Paths const& locations) {
        namespace fs = std::filesystem;

        for (Path const& path : locations) {
            if (isPackage(path) && fs::exists(path))
                loadPackage(path);
            else
                loadShaderDirectory(path);
        }
    }

    bool ShaderLibrary::isPackage(Path const& path) noexcept {
        auto size = path.size();
        return size > 6 && path[size - 6] == '.' &&
            path[size - 5] == 's' && path[size - 4] == 'l' &&
            path[size - 3] == '.' && path[size - 2] == 'g' &&
            path[size - 1] == 'z';
    }

    void ShaderLibrary::loadShaderDirectory(Path const& path) {
        for (std::string const& shader : getShaderList(path)) {
            VertexShader vertex{vertexShaders(path) + "/" + shader};
            FragmentShader fragment{fragmentShaders(path) + "/" + shader};
            ShaderProgram program{vertex, fragment};
            program.link(shader);
            programs.emplace(shader.substr(0,
                shader.find_last_of('.')), std::move(program));
        }
    }

    void ShaderLibrary::loadPackage(Path const& path) {
        auto shaderMap = SLGZLoader{security::secured}(path);
        for (std::string const& shader : getShaderList(shaderMap)) {
            VertexShader vertex{shaderMap["Vertex/" + shader]};
            FragmentShader fragment{shaderMap["Fragment/" + shader]};
            ShaderProgram program{vertex, fragment};
            program.link(shader);
            programs.emplace(shader, std::move(program));
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

    ShaderLibrary::Paths ShaderLibrary::getShaderList(
        ShaderMap const& map) const
    {
        Paths vertex, fragment;
        for (auto const& [shader, _] : map) {
            if (shader[0] == 'V')
                vertex.push_back(shader);
            else
                fragment.push_back(shader);
        }
        if (!sameShaders(vertex, fragment, ""))
            throw ShaderLibraryInvalidShadersException{vertex, fragment};
        Paths shaders;
        std::ranges::transform(vertex, std::back_inserter(shaders),
            [](auto const& path)
                { return path.substr(vertexShaders("").size() + 1); });
        return shaders;
    }

}

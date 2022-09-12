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
#include <MPGL/Exceptions/Shader/ShaderCompilationException.hpp>
#include <MPGL/Core/Shaders/Shader.hpp>
#include <MPGL/Utility/Ranges.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <algorithm>
#include <iterator>
#include <fstream>

namespace mpgl {

    template <bool ShaderType>
    void Shader<ShaderType>::verifyCompilationStatus(void) const {
        int32 status = 0;
        glGetProgramiv(shaderID, GL_COMPILE_STATUS, &status);
        if (!status) {
            std::string info;
            info.resize(512);
            glGetProgramInfoLog(shaderID, 512, nullptr, info.data());
            if (!accumulate(info, 0u))
                return;
            throw ShaderCompilationException{info};
        }
    }

    template <bool ShaderType>
    Shader<ShaderType>::Shader(std::string shaderPath)
        : shaderID{glCreateShader(shaderType())}
    {
        if (auto shaderCode = FileIO::readFileToVec(shaderPath))
            loadShader((shaderCode->push_back(0), *shaderCode));
        else
            throw ShaderCompilationException{
                "Shader cannot be loaded from a file"};
    }

    template <bool ShaderType>
    Shader<ShaderType>::Shader(Shader&& shader) noexcept
        : shaderID{shader.shaderID}
    {
        shader.shaderID = 0;
    }

    template <bool ShaderType>
    Shader<ShaderType>& Shader<ShaderType>::operator= (
        Shader&& shader) noexcept
    {
        shaderID = shader.shaderID;
        shader.shaderID = 0;
        return *this;
    }

    template <bool ShaderType>
    Shader<ShaderType>::~Shader(void) noexcept {
        if (shaderID)
            glDeleteShader(shaderID);
    }

}

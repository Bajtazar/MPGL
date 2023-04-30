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
#pragma once

#include <MPGL/Exceptions/Shader/ShaderMissingSentinelException.hpp>

namespace mpgl {

    template <bool ShaderType>
    template <std::ranges::contiguous_range Range>
        requires std::same_as<std::ranges::range_value_t<Range>, char>
    Shader<ShaderType>::Shader(Range&& range)
        : shaderID{glCreateShader(shaderType())}
    {
        if (*(std::ranges::end(range) - 1))
            throw ShaderMissingSentinelException{};
        loadShader(std::forward<Range>(range));
    }

    template <bool ShaderType>
    template <std::ranges::contiguous_range Range>
        requires std::same_as<std::ranges::range_value_t<Range>,
            char>
    void Shader<ShaderType>::loadShader(Range&& range) {
        const char* codePointer = std::ranges::data(range);
        glShaderSource(shaderID, 1, &codePointer, nullptr);
        glCompileShader(shaderID);
        verifyCompilationStatus();
    }

}

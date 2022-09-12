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

#include <algorithm>
#include <ranges>

namespace mpgl {

    ShaderLibraryInvalidShadersException::ShaderLibraryInvalidShadersException(
        Paths vertex,
        Paths fragment) noexcept
            : message{std::string{
                "Shaders without pairs have been detected."}
            + " List of invalid shaders:\n"}
    {
        std::ranges::sort(vertex);
        std::ranges::sort(fragment);
        auto&& [invalidShaders, vIter, fIter]
            = findAloneShaders(vertex, fragment);
        std::ranges::copy(vIter, vertex.end(),
            std::back_inserter(invalidShaders));
        std::ranges::copy(fIter, fragment.end(),
            std::back_inserter(invalidShaders));
        for (const auto& shader : invalidShaders)
            message += shader + '\n';
    }

    ShaderLibraryInvalidShadersException::AloneArgs
        ShaderLibraryInvalidShadersException::findAloneShaders(
            Paths const& vertex,
            Paths const& fragment) const
    {
        Paths invalidShaders;
        auto vIter = vertex.begin(), fIter = fragment.begin();
        while (vIter != vertex.end() && fIter != fragment.end()) {
            if (vIter->substr(15) == fIter->substr(17)) {
                ++vIter;
                ++fIter;
            } else if (vIter->substr(15) < fIter->substr(17))
                invalidShaders.push_back(*vIter++);
            else
                invalidShaders.push_back(*fIter++);
        }
        return {invalidShaders, vIter, fIter};
    }

}

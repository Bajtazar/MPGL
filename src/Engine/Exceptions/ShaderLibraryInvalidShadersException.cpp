#include "ShaderLibraryInvalidShadersException.hpp"

#include <algorithm>

namespace ge {

    ShaderLibraryInvalidShadersException::ShaderLibraryInvalidShadersException(std::vector<std::string> vertex, std::vector<std::string> fragment) noexcept : message{"Shaders without pairs have been detected. List of invalid shaders:\n"}{
        std::ranges::sort(vertex);
        std::ranges::sort(fragment);
        std::vector<std::string> invalidShaders;
        auto vIter = vertex.begin(), fIter = fragment.begin();
        while (vIter != vertex.end() && fIter != fragment.end()) {
            if (vIter->substr(15) == fIter->substr(17)) {
                ++vIter;
                ++fIter;
            } else if (vIter->substr(15) < fIter->substr(17)) {
                invalidShaders.push_back(*vIter);
                ++vIter;
            } else {
                invalidShaders.push_back(*fIter);
                ++fIter;
            }
        }
        std::ranges::copy(vIter, vertex.end(), std::back_inserter(invalidShaders));
        std::ranges::copy(fIter, fragment.end(), std::back_inserter(invalidShaders));
        for (const auto& shader : invalidShaders)
            message += shader + '\n';
    }

}

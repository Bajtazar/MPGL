#include "StringAlgorithm.hpp"

#include <algorithm>

namespace mpgl {

    std::string toLower(std::string const& string) {
        std::string output;
        output.reserve(string.size());
        std::ranges::transform(string, std::back_inserter(output),
            [](char const& x) { return std::tolower(x); });
        return output;
    }

    std::string toUpper(std::string const& string) {
        std::string output;
        output.reserve(string.size());
        std::ranges::transform(string, std::back_inserter(output),
            [](char const& x) { return std::toupper(x); });
        return output;
    }

}

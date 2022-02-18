#pragma once

#include "ShaderProgram.hpp"

#include <string>
#include <vector>
#include <map>

namespace mpgl {

    class ShaderLibrary {
    public:
        explicit ShaderLibrary(void);

        using iterator = std::map<std::string, ShaderProgram>::const_iterator;

        iterator begin(void) const noexcept { return programs.begin(); }
        iterator end(void) const noexcept { return programs.end(); }

        ShaderProgram const& operator[] (std::string const& name) const { return programs.at(name); }
    private:
        std::map<std::string, ShaderProgram>        programs;

        std::vector<std::string> getShaderList(void) const;
    };

}

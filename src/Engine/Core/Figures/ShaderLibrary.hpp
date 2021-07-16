#pragma once

#include "Shader.hpp"

#include <string>
#include <vector>
#include <map>

namespace ge {

    class ShaderLibrary {
    public:
        explicit ShaderLibrary(void);
        ShaderLibrary(const ShaderLibrary& shaderLib) noexcept = delete;
        ShaderLibrary(ShaderLibrary&& shaderLib) noexcept : programs(std::move(shaderLib.programs)) {}

        ShaderLibrary& operator=(const ShaderLibrary& shaderLib) noexcept = delete;
        ShaderLibrary& operator=(ShaderLibrary&& shaderLib) noexcept;

        using iterator = std::map<std::string, uint32_t>::const_iterator;

        iterator begin(void) const noexcept { return programs.begin(); }
        iterator end(void) const noexcept { return programs.end(); }

        uint32_t operator[] (const std::string& name) const noexcept { return programs.at(name); }

        ~ShaderLibrary(void) noexcept;
    private:
        std::map<std::string, uint32_t> programs;

        std::vector<std::string> getShaderList(void) const;
    };

}

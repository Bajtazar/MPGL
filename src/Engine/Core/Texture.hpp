#pragma once

#include <string>

namespace ge {

    class Texture {
    public:
        struct Options {

        };
        explicit Texture(const std::string& fileName, Options options = {});

        const uint32_t& getTexture(void) const noexcept { return textureID; }
    private:
        uint32_t textureID;
    };

}

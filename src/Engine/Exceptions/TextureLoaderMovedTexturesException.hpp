#pragma once

#include <exception>

namespace ge {

    class TextureLoaderMovedTexturesException : public std::exception {
    public:
        explicit TextureLoaderMovedTexturesException(void) noexcept = default;
        virtual const char* what (void) const noexcept final { return "Texture pack has been allready given"; }
    };

}

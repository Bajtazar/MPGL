#pragma once

#include "../Figures/Sprite.hpp"

namespace ge {

    // will be changed
    class GlyphSprite : public ColorableSprite {
    public:
        using Sprite::Sprite;

        void setShaders(ShaderLibrary const& library) noexcept final;

        ~GlyphSprite(void) noexcept = default;
    };

}

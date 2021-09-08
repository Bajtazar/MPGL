#include "GlyphSprite.hpp"

namespace ge {

    void GlyphSprite::setShaders(ShaderLibrary const& library) noexcept {
        shaderProgram = library["2DGlyph"];
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    }

}

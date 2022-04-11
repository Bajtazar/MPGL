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
#pragma once

namespace mpgl {

    constexpr Texture::Options::Options(
        TextureWrapper verticalWrapping,
        TextureWrapper horizontalWrapping,
        MinifyingTextureFilter minifyingFilter,
        MagnifyingTextureFilter magnifyingFilter,
        Color const& color,
        bool mipmaps) noexcept
            : verticalWrapping{verticalWrapping},
            horizontalWrapping{horizontalWrapping},
            minifyingFilter{minifyingFilter},
            magnifyingFilter{magnifyingFilter},
            borderColor{color},
            mipmaps{mipmaps} {}

    constexpr Texture::Options::Underlying
        Texture::Options::getOptions(void) const noexcept
    {
        // change static_cast to std::to_underlying in C++23
        return {
            Filter{GL_TEXTURE_WRAP_S,
                static_cast<GLint>(verticalWrapping)},
            Filter{GL_TEXTURE_WRAP_T,
                static_cast<GLint>(horizontalWrapping)},
            Filter{GL_TEXTURE_MIN_FILTER,
                static_cast<GLint>(minifyingFilter)},
            Filter{GL_TEXTURE_MAG_FILTER,
                static_cast<GLint>(magnifyingFilter)},
        };
    }

    constexpr bool Texture::Options::isBorder(void) const noexcept {
        return verticalWrapping == TextureWrapper::ClampToBorder ||
            horizontalWrapping == TextureWrapper::ClampToBorder;
    }

}

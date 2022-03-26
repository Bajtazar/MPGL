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
#include "Texture.hpp"

namespace mpgl {

    Texture::Texture(Options const& options)
        : texturePtr{new Holder{}}
    {
        texturePtr->textureBuffer.bind();
        for (auto const& [filter, mode] : options.getOptions())
            texturePtr->textureBuffer.setParameter(filter, mode);
        if (options.isBorder())
            texturePtr->textureBuffer.setBorderColor(
                options.borderColor);
    }

    Texture::Texture(
        std::string const& fileName,
        Options const& options) : Texture{
            ImageLoader<>{fileName}.getImage(), options} {}

    Texture::Texture(
        Image const& image,
        Options const& options) : Texture{options}
    {
        texturePtr->textureBuffer.loadImage(
            TextureBuffer::PixelFormat::RGBA,
            image.getWidth(), image.getHeight(),
            image.data());
        texturePtr->textureSize = image.size();
        if (options.mipmaps)
            texturePtr->textureBuffer.generateMipmaps();
    }

    Texture::Texture(
        Bitmap const& bitmap,
        Options const& options) : Texture{options}
    {
        texturePtr->textureBuffer.loadImage(
            TextureBuffer::PixelFormat::R,
            bitmap.getWidth(), bitmap.getHeight(),
            bitmap.data());
        texturePtr->textureSize = bitmap.size();
        if (options.mipmaps)
            texturePtr->textureBuffer.generateMipmaps();
    }

    [[nodiscard]] Texture Texture::defaultTexture(
        Options const& options)
    {
        Image image{8, 8};
        for (uint8 i = 0; i != 4; ++i) {
            for (uint8 j = 0; j != 4; ++j) {
                image[2*i][2*j] = Pixel{0x7F, 0x7F, 0x7F, 0xFF};
                image[2*i][2*j+1] = Pixel{0x99, 0xD9, 0xEA, 0xFF};
                image[2*i+1][2*j] = Pixel{0x99, 0xD9, 0xEA, 0xFF};
                image[2*i+1][2*j+1] = Pixel{0x7F, 0x7F, 0x7F, 0xFF};
            }
        }
        return Texture{image, options};
    }

}

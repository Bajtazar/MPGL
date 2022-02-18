/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022 Grzegorz Czarnecki (wahadlowiec@interia.pl)
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
#include "TextureLoader.hpp"

#include "../Exceptions/StackedExceptions.hpp"

namespace ge {

    [[nodiscard]] TexturePack
        TextureLoaderBase::getTextures(void) const
    {
        if (!exceptions.empty())
            throw StackedExceptions{exceptions};
        return TexturePack{textures};
    }

    void TextureLoaderBase::tryLoad(void) {
        if (loadingStatus() != 1.)
            load();
    }

    void TextureLoaderBase::loadAll(void) {
        while (loadingStatus() != 1.)
            load();
    }

}

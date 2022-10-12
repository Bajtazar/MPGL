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
#include <MPGL/Core/Textures/TexturePack.hpp>

namespace mpgl {

    TexturePack::TexturePack(Value const& defaultTexture)
        : defaultValue{defaultTexture} {}

    TexturePack& TexturePack::operator=(TexturePack&& other) noexcept {
        textureMap = std::move(other.textureMap);
        const_cast<Value&>(defaultValue) = std::move(
            other.defaultValue);
        return *this;
    }

    TexturePack::Value const&
        TexturePack::operator[] (KeyboardKey const& key) const noexcept
    {
        if (auto iter = textureMap.find(key); iter != textureMap.end())
            return iter->second;
        return defaultValue;
    }

}

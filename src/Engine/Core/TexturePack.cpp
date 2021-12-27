#include "TexturePack.hpp"

namespace ge {

    TexturePack::Value const&
        TexturePack::operator[] (Key const& key) const noexcept
    {
        if (auto iter = textureMap.find(key); iter != textureMap.end())
            return iter->second;
        return defaultValue;
    }

}
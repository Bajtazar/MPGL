#pragma once

#include "../Exceptions/ExecusionUnknownPolicyException.hpp"
#include "Texture.hpp"

#include <unordered_map>
#include <algorithm>
#include <vector>

namespace ge {

    class TexturePack {
    public:
        typedef std::string                         Key;
        typedef Texture                             Value;
        typedef std::pair<Key const, Value>         ValuePair;

        template <std::ranges::random_access_range Range>
            requires SameRangeType<Range, ValuePair>
        explicit TexturePack(Range&& range,
            Value const& defaultTexture = Value::defaultTexture());

        Value const& operator[] (Key const& key) const noexcept;
    private:
        typedef std::unordered_map<Key, Value>      TextureMap;

        TextureMap                                  textureMap;
        Value const                                 defaultValue;
    };

    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range,
            typename TexturePack::ValuePair>
    TexturePack::TexturePack(Range&& range,
        Value const& defaultTexture)
            : defaultValue{defaultTexture},
                textureMap{range.begin(), range.end()} {}

    TexturePack::Value const&
        TexturePack::operator[] (Key const& key) const noexcept
    {
        if (auto iter = textureMap.find(key); iter != textureMap.end())
            return iter->second;
        return defaultValue;
    }

}

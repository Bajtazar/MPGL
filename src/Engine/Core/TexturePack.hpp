#pragma once

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
        typedef std::unordered_map<Key, Value>      TextureMap;
        typedef typename TextureMap::const_iterator ConstIterator;

        template <std::ranges::random_access_range Range>
            requires SameRangeType<Range, ValuePair>
        explicit TexturePack(Range&& range,
            Value const& defaultTexture = Value::defaultTexture());

        ConstIterator begin(void) const noexcept
            { return textureMap.begin(); }

        ConstIterator end(void) const noexcept
            { return textureMap.end(); }

        ConstIterator cbegin(void) const noexcept
            { return textureMap.cbegin(); }

        ConstIterator cend(void) const noexcept
            { return textureMap.cend(); }

        Value const& operator[] (Key const& key) const noexcept;
    private:


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

}
